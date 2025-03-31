#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Job structure to hold job details
struct Job {
    int jobId;
    int priority;  // 0 (highest) to 3 (lowest)
    
    Job(int id, int pri) : jobId(id), priority(pri) {}
};

// Comparator for priority queue (min-heap based on priority)
struct ComparePriority {
    bool operator()(const Job& j1, const Job& j2) {
        // Lower priority number = higher priority
        return j1.priority > j2.priority;
    }
};

int main() {
    // Seed random number generator
    srand(time(0));
    
    // Create priority queue and vector for initial display
    priority_queue<Job, vector<Job>, ComparePriority> jobQueue;
    vector<Job> jobList;
    
    // Generate 10 jobs with random priorities
    cout << "Generating 10 jobs with random priorities...\n";
    cout << "Initial Job List:\n";
    cout << "Job ID\tPriority\n";
    cout << "----------------\n";
    
    for (int i = 1; i <= 10; i++) {
        // Random priority between 0 and 3
        int randomPriority = rand() % 4;  // Generates 0, 1, 2, or 3
        Job newJob(i, randomPriority);
        jobList.push_back(newJob);        // Store for initial display
        jobQueue.push(newJob);           // Add to priority queue
    }
    
    // Print initial job list
    for (const Job& job : jobList) {
        cout << job.jobId << "\t" << job.priority << "\n";
    }
    
    // Process jobs based on priority
    cout << "\nExecuting jobs in priority order (0 = highest, 3 = lowest):\n";
    cout << "Job ID\tPriority\n";
    cout << "----------------\n";
    
    while (!jobQueue.empty()) {
        Job currentJob = jobQueue.top();
        cout << currentJob.jobId << "\t" << currentJob.priority << "\n";
        jobQueue.pop();
    }
    
    return 0;
}