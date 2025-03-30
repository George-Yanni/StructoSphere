#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// Structure to represent a Process
struct Process {
    int processId;      // Unique identifier for the process
    int burstTime;      // Time required to complete the process
    Process(int id, int bt) : processId(id), burstTime(bt) {}
};

// Queue class to manage processes
class ProcessQueue {
private:
    static const int MAX_SIZE = 100;  // Maximum size of the queue
    Process* queue[MAX_SIZE];         // Array to store process pointers
    int front;                        // Index of the front element
    int rear;                         // Index of the rear element
    int currentSize;                  // Current number of elements in the queue

public:
    // Constructor
    ProcessQueue() : front(0), rear(-1), currentSize(0) {}

    // Destructor to free dynamically allocated memory
    ~ProcessQueue() {
        while (!isEmpty()) {
            dequeue();  // Dequeue all elements to free memory
        }
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return currentSize == 0;
    }

    // Check if the queue is full
    bool isFull() const {
        return currentSize == MAX_SIZE;
    }

    // Add a process to the queue (enqueue)
    void enqueue(int processId, int burstTime) {
        if (isFull()) {
            throw overflow_error("Queue is full! Cannot enqueue more processes.");
        }
        rear = (rear + 1) % MAX_SIZE;  // Circular increment
        queue[rear] = new Process(processId, burstTime);
        currentSize++;
        cout << "Enqueued Process " << processId << " with burst time " << burstTime << "ms\n";
    }

    // Remove and return the front process from the queue (dequeue)
    Process* dequeue() {
        if (isEmpty()) {
            throw underflow_error("Queue is empty! Cannot dequeue.");
        }
        Process* process = queue[front];
        queue[front] = nullptr;  // Clear the pointer
        front = (front + 1) % MAX_SIZE;  // Circular increment
        currentSize--;
        return process;
    }

    // Peek at the front process without removing it
    Process* peek() const {
        if (isEmpty()) {
            throw underflow_error("Queue is empty! Nothing to peek.");
        }
        return queue[front];
    }

    // Get the current size of the queue
    int size() const {
        return currentSize;
    }

    // Display the current state of the queue
    void display() const {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }
        cout << "Current Queue (Front to Rear):\n";
        int count = 0;
        int index = front;
        while (count < currentSize) {
            cout << "Process " << queue[index]->processId 
                 << " (Burst Time: " << queue[index]->burstTime << "ms)\n";
            index = (index + 1) % MAX_SIZE;
            count++;
        }
    }
};

// CPU Scheduler class to simulate process execution
class CPUScheduler {
private:
    ProcessQueue processQueue;  // Queue to hold processes
    int totalWaitingTime;       // Total waiting time for all processes
    int totalTurnaroundTime;    // Total turnaround time for all processes
    int processCount;           // Number of processes executed

public:
    CPUScheduler() : totalWaitingTime(0), totalTurnaroundTime(0), processCount(0) {}

    // Add a process to the queue
    void addProcess(int processId, int burstTime) {
        processQueue.enqueue(processId, burstTime);
    }

    // Simulate CPU execution using FCFS
    void executeProcesses() {
        cout << "\nStarting CPU Execution (FCFS Scheduling):\n";
        int currentTime = 0;  // Current time in the simulation

        while (!processQueue.isEmpty()) {
            Process* currentProcess = processQueue.dequeue();
            int waitingTime = currentTime;  // Time the process waited before execution
            int turnaroundTime = waitingTime + currentProcess->burstTime;  // Total time from arrival to completion

            // Update totals
            totalWaitingTime += waitingTime;
            totalTurnaroundTime += turnaroundTime;
            processCount++;

            // Display execution details
            cout << "Executing Process " << currentProcess->processId 
                 << " | Burst Time: " << currentProcess->burstTime 
                 << "ms | Waiting Time: " << waitingTime 
                 << "ms | Turnaround Time: " << turnaroundTime << "ms\n";

            currentTime += currentProcess->burstTime;  // Increment time
            delete currentProcess;  // Free memory
        }

        // Display performance metrics
        displayMetrics();
    }

    // Display average waiting and turnaround times
    void displayMetrics() const {
        if (processCount == 0) {
            cout << "No processes executed.\n";
            return;
        }
        double avgWaitingTime = static_cast<double>(totalWaitingTime) / processCount;
        double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / processCount;
        cout << "\nPerformance Metrics:\n";
        cout << "Average Waiting Time: " << avgWaitingTime << "ms\n";
        cout << "Average Turnaround Time: " << avgTurnaroundTime << "ms\n";
    }

    // Display the current queue state
    void displayQueue() const {
        processQueue.display();
    }
};

// Main function to demonstrate the queue and CPU scheduling
int main() {
    try {
        CPUScheduler scheduler;

        // Adding processes to the queue
        cout << "Adding processes to the CPU scheduler...\n";
        scheduler.addProcess(1, 10);  // Process 1 with 10ms burst time
        scheduler.addProcess(2, 5);   // Process 2 with 5ms burst time
        scheduler.addProcess(3, 8);   // Process 3 with 8ms burst time
        scheduler.addProcess(4, 3);   // Process 4 with 3ms burst time

        // Display the queue before execution
        cout << "\nQueue before execution:\n";
        scheduler.displayQueue();

        // Execute the processes
        scheduler.executeProcesses();

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}