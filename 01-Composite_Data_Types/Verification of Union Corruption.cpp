#include <iostream>
#include <bitset>
#include <cstring> // For memcpy

using namespace std;

// Define a union to store an int and float in the same memory
union Data {
    int intValue;
    float floatValue;
};

int main() {
    Data d; // Declare a union variable

    // Step 1: Assign an integer value and display its binary representation
    d.intValue = 42;
    cout << "Step 1: Assign intValue = " << d.intValue << endl;
    cout << "Binary Representation (int): " << bitset<32>(d.intValue) << endl;

    // Step 2: Assign a floating-point value (3.14f), overwriting the memory
    d.floatValue = 3.14f;
    cout << "\nStep 2: Assign floatValue = " << d.floatValue << endl;
    cout << "Binary Representation (float as int): " << bitset<32>(d.intValue) << endl;
    cout << "intValue after overwriting: " << d.intValue << " (interpreted from float)" << endl;

    // Step 3: Verify that the float value has the same bit pattern as the new int value
    int floatAsInt;
    memcpy(&floatAsInt, &d.floatValue, sizeof(float)); // Copy float bits to an int
    cout << "\nStep 3: Verifying float binary representation" << endl;
    cout << "Float 3.14f as raw binary: " << bitset<32>(floatAsInt) << endl;
    cout << "Float 3.14f as int: " << floatAsInt << endl;

    return 0;
}
