#include <iostream>
#include <bitset>

using namespace std;

#define NUM_FEATURES 8  // Number of features
// Explanation:
// Bit Array as Feature Flags: Each bit represents whether a feature is enabled (1) or disabled (0).

// You can set, reset, or check individual flags using bit operations.

// This is useful in software systems where many options or settings need to be toggled on/off.
void demonstrateFlags() {
    // Create a bitset (bit array) to store flags for features
    bitset<NUM_FEATURES> features;

    // Set flags for features: Enable features at index 2, 4, and 6
    features.set(2);  // Feature 3 enabled
    features.set(4);  // Feature 5 enabled
    features.set(6);  // Feature 7 enabled

    cout << "Feature Flags: " << features << endl;  // Output: 010100100

    // Check if a specific feature is enabled
    int featureIndex = 4;
    if (features.test(featureIndex)) {
        cout << "Feature " << featureIndex + 1 << " is enabled." << endl;
    } else {
        cout << "Feature " << featureIndex + 1 << " is disabled." << endl;
    }

    // Disable a feature (reset bit)
    features.reset(4);  // Disable Feature 5
    cout << "Feature Flags after disabling Feature 5: " << features << endl;  // Output: 010100000
}

int main() {
    cout << "Demonstrating Feature Flags with Bit Array:" << endl << endl;
    demonstrateFlags();
    return 0;
}
