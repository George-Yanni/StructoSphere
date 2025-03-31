#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <condition_variable>

using namespace std;

// Structure to represent a Process
struct Process {
    int processId;
    int burstTime;       // Time required to complete the process
    int arrivalTime;     // Time when process arrives
    int waitingTime;     // Time spent waiting before execution
    int turnaroundTime;  // Time from arrival to completion
    int completionTime;  // Time when process finishes

    Process(int id, int bt, int at) 
        : processId(id), burstTime(bt), arrivalTime(at), waitingTime(0), 
          turnaroundTime(0), completionTime(0) {}
};

// Class to manage the multi-core CPU scheduler with a priority queue
class MultiCoreScheduler {
private:
    // Custom comparator for priority queue (earliest arrival first)
    struct CompareArrival {
        bool operator()(const Process& a, const Process& b) {
            return a.arrivalTime > b.arrivalTime;
        }
    };
    
    priority_queue<Process, vector<Process>, CompareArrival> processQueue;
    vector<Process> completedProcesses;
    int numCores;
    mutex queueMtx;
    mutable mutex outputMtx;
    mutex metricsMtx;
    condition_variable cv;
    bool done = false;
    int totalWaitingTime;
    int totalTurnaroundTime;
    int totalBurstTime;
    int simulationTime;

public:
    MultiCoreScheduler(int cores) 
        : numCores(cores), totalWaitingTime(0), totalTurnaroundTime(0), 
          totalBurstTime(0), simulationTime(0) {
        if (numCores <= 0) {
            throw invalid_argument("Number of cores must be positive.");
        }
    }

    // Add a process to the priority queue
    void addProcess(int processId, int burstTime, int arrivalTime) {
        lock_guard<mutex> lock(queueMtx);
        processQueue.push(Process(processId, burstTime, arrivalTime));
        totalBurstTime += burstTime;
        lock_guard<mutex> outputLock(outputMtx);
        cout << "Added Process " << processId << " (Burst: " << burstTime 
             << "ms, Arrival: " << arrivalTime << "ms)\n";
        cv.notify_one();
    }

    // Simulate a single core's execution
    void coreWorker(int coreId, vector<Process>& localCompleted, int& coreEndTime) {
        int currentTime = 0;
        unique_lock<mutex> lock(queueMtx);
        while (!done || !processQueue.empty()) {
            cv.wait(lock, [this] { return done || !processQueue.empty(); });

            if (processQueue.empty()) {
                break;
            }

            Process process = processQueue.top();
            processQueue.pop();
            lock.unlock();

            // Simulate execution time (scaled down for demo)
            this_thread::sleep_for(chrono::milliseconds(10));

            // Calculate metrics
            int startTime = max(currentTime, process.arrivalTime);
            process.waitingTime = startTime - process.arrivalTime;
            process.turnaroundTime = process.waitingTime + process.burstTime;
            process.completionTime = startTime + process.burstTime;
            currentTime = process.completionTime;

            // Update global metrics
            {
                lock_guard<mutex> metricsLock(metricsMtx);
                totalWaitingTime += process.waitingTime;
                totalTurnaroundTime += process.turnaroundTime;
            }

            // Thread-safe output
            {
                lock_guard<mutex> outputLock(outputMtx);
                cout << "Core " << coreId << " executed Process " << process.processId 
                     << " | Burst: " << process.burstTime << "ms | Waiting: " 
                     << process.waitingTime << "ms | Turnaround: " << process.turnaroundTime 
                     << "ms | Completed: " << process.completionTime << "ms\n";
            }

            localCompleted.push_back(process);
            lock.lock();
        }
        coreEndTime = currentTime;
    }

    // Execute processes across all cores
    void executeProcesses() {
        {
            lock_guard<mutex> lock(outputMtx);
            cout << "\nStarting Multi-Core Execution (FCFS Scheduling):\n";
        }

        vector<thread> threads;
        vector<vector<Process>> threadResults(numCores);
        vector<int> coreEndTimes(numCores, 0);

        for (int i = 0; i < numCores; i++) {
            threads.emplace_back(&MultiCoreScheduler::coreWorker, this, i, 
                                 ref(threadResults[i]), ref(coreEndTimes[i]));
        }

        for (auto& t : threads) {
            t.join();
        }

        for (const auto& result : threadResults) {
            completedProcesses.insert(completedProcesses.end(), result.begin(), result.end());
        }

        simulationTime = *max_element(coreEndTimes.begin(), coreEndTimes.end());
        displayMetrics();
    }

    void displayMetrics() const {
        lock_guard<mutex> lock(outputMtx);
        if (completedProcesses.empty()) {
            cout << "No processes executed.\n";
            return;
        }

        double avgWaitingTime = static_cast<double>(totalWaitingTime) / completedProcesses.size();
        double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / completedProcesses.size();
        double cpuUtilization = (static_cast<double>(totalBurstTime) / (simulationTime * numCores)) * 100;

        cout << "\nScheduling Metrics:\n";
        cout << "Total Processes: " << completedProcesses.size() << "\n";
        cout << "Average Waiting Time: " << avgWaitingTime << "ms\n";
        cout << "Average Turnaround Time: " << avgTurnaroundTime << "ms\n";
        cout << "Simulation Time: " << simulationTime << "ms\n";
        cout << "CPU Utilization: " << cpuUtilization << "%\n";

        cout << "\nCompleted Processes:\n";
        for (const auto& p : completedProcesses) {
            cout << "Process " << p.processId << " | Waiting: " << p.waitingTime 
                 << "ms | Turnaround: " << p.turnaroundTime << "ms | Completed: " 
                 << p.completionTime << "ms\n";
        }
    }

    void signalDone() {
        lock_guard<mutex> lock(queueMtx);
        done = true;
        cv.notify_all();
    }
};

int main() {
    try {
        int numCores;
        cout << "Enter the number of CPU cores: ";
        cin >> numCores;

        MultiCoreScheduler scheduler(numCores);

        // Adding sample processes (processId, burstTime, arrivalTime)
        scheduler.addProcess(1, 10, 0);   // Process 1: 10ms burst, arrives at 0ms
        scheduler.addProcess(2, 5, 2);    // Process 2: 5ms burst, arrives at 2ms
        scheduler.addProcess(3, 8, 1);    // Process 3: 8ms burst, arrives at 1ms
        scheduler.addProcess(4, 3, 3);    // Process 4: 3ms burst, arrives at 3ms
        scheduler.addProcess(5, 15, 0);   // Process 5: 15ms burst, arrives at 0ms

        scheduler.signalDone();
        scheduler.executeProcesses();

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}