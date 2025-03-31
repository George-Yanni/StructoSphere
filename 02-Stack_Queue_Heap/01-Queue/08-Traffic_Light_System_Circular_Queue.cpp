#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <windows.h> // For colored output in Windows CMD

using namespace std;

class TrafficLight {
private:
    enum LightColor { RED, YELLOW, GREEN };
    static const int QUEUE_SIZE = 3;
    LightColor currentColor;
    int duration[QUEUE_SIZE] = {3, 1, 3}; // Durations for RED, YELLOW, GREEN
    string colorNames[QUEUE_SIZE] = {"RED", "YELLOW", "GREEN"};

    // Handle for console output
    HANDLE hConsole;

    // Function to set console text color based on current light
    void setColor(LightColor color) {
        switch (color) {
            case RED:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                break;
            case YELLOW:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            case GREEN:
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
        }
    }

    // Function to reset console color to default (white)
    void resetColor() {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

public:
    TrafficLight() : currentColor(RED) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle
    }

    void cycle() {
        while (true) {
            // Display current light status
            setColor(currentColor);
            cout << "\n=== TRAFFIC LIGHT ===\n";
            
            // Set color for "Current" line and display it
            setColor(currentColor);
            cout << "Current: " << colorNames[currentColor];
            
            cout << endl;
            
            // Set color for "Message" line and display it
            setColor(currentColor);

            cout << endl;
            
            // Display duration in default color
            cout << "Duration: " << duration[currentColor] << " seconds\n";
            
            // Simulate light duration
            this_thread::sleep_for(chrono::seconds(duration[currentColor]));
            
            // Move to next light in circular manner
            currentColor = static_cast<LightColor>((currentColor + 1) % QUEUE_SIZE);
        }
    }
};

int main() {
    TrafficLight trafficLight;
    cout << "Starting Traffic Light Simulation (Ctrl+C to stop)\n";
    trafficLight.cycle();
    return 0;
}