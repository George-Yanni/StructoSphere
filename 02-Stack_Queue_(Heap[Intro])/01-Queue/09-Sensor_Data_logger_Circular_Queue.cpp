#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cmath>    // For sin() function (simulating sensor data)
#include <unistd.h> // For usleep() (microsecond delays)

using namespace std;

class SensorDataLogger {
private:
    vector<float> buffer;
    int capacity;
    int head = -1;  // Points to latest data
    int tail = 0;   // Points to oldest data (when buffer is full)
    int count = 0;
    bool isFull = false;

    // ANSI color codes for visualization
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string BLUE = "\033[34m";
    const string RESET = "\033[0m";

public:
    SensorDataLogger(int size) : capacity(size) {
        buffer.resize(size, NAN); // Initialize with NaN (empty slots)
    }

    void addData(float value) {
        head = (head + 1) % capacity;
        
        if (isFull) {
            tail = (tail + 1) % capacity; // Move tail if buffer is full
        } else if (count == capacity - 1) {
            isFull = true;
        }

        buffer[head] = value;
        if (!isFull) count++;
        
        visualize("Added: " + to_string(value));
    }

    void visualize(const string& action) {
        cout << "\n\n=== Circular Buffer State ===" << endl;
        cout << "Action: " << BLUE << action << RESET << endl;
        cout << "Head: " << head << " | Tail: " << tail << " | Count: " << count << endl;

        // Print buffer slots with readable symbols
        cout << "Buffer: [";
        for (int i = 0; i < capacity; i++) {
            if (i == head && i == tail && isFull) {
                cout << RED << "|" << RESET; // Head and tail overlap (full buffer)
            } else if (i == head) {
                cout << GREEN << "->" << RESET; // Head position (right arrow)
            } else if (i == tail && isFull) {
                cout << RED << "<-" << RESET;   // Tail position (left arrow)
            } else if (!isnan(buffer[i])) {
                cout << GREEN << "." << RESET; // Filled slot (filled circle)
            } else {
                cout << ".";                  // Empty slot (empty circle)
            }
            if (i < capacity - 1) cout << " ";
        }
        cout << "]" << endl;

        // Print values
        cout << "Values: [";
        for (int i = 0; i < capacity; i++) {
            if (!isnan(buffer[i])) {
                printf("%.1f", buffer[i]); // Show 1 decimal place
            } else {
                cout << "   "; // Space for alignment when empty
            }
            if (i < capacity - 1) cout << " ";
        }
        cout << "]" << endl;
    }

    void plotReadings() {
        cout << "\n=== Sensor Readings Plot ===" << endl;
        cout << setw(6) << "Index" << " | " << "Temperature" << endl;
        cout << string(30, '-') << endl;
        for (int i = 0; i < capacity; i++) {
            int idx = (head - i + capacity) % capacity;
            if (!isnan(buffer[idx])) {
                cout << setw(6) << idx << ": ";
                int val = (int)(buffer[idx] - 20); // Scale for plotting (20-30°C)
                for (int j = 0; j < val; j++) cout << "."; // Bar symbol for plot
                cout << " " << fixed << setprecision(1) << buffer[idx] << "°C" << endl;
            }
        }
    }
};

int main() {
    SensorDataLogger logger(10); // 10-slot buffer (smaller for better visualization)

    // Simulate sensor data (sine wave + noise)
    for (float t = 0; t < 6.28; t += 0.3) { // 6.28 ≈ 2π (complete cycle)
        float temp = 25.0 + 5.0 * sin(t);   // Oscillates between 20-30°C
        logger.addData(temp);
        usleep(500000); // 0.5s delay between readings
    }

    return 0;
}