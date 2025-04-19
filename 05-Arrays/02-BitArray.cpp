#include <iostream>
#include <bitset>
#include <string>
#include <sstream>

using namespace std;

#define BIT_ARRAY_SIZE 16  // Define the size of the bit array (16 bits)

void demonstrateBitArray() {
    // Create a bitset (bit array) of fixed size 16
    bitset<BIT_ARRAY_SIZE> bitArray;

    cout << "Initial Bit Array: " << bitArray << endl;  // Default: 0000000000000000

    // Set specific bits using set() method
    bitArray.set(3);  // Set bit at index 3 to 1
    bitArray.set(7);  // Set bit at index 7 to 1

    cout << "After setting bits at index 3 and 7: " << bitArray << endl;  // 0000101000000000

    // Reset a specific bit to 0 using reset()
    bitArray.reset(7);  // Reset bit at index 7 to 0

    cout << "After resetting bit at index 7: " << bitArray << endl;  // 0000001000000000

    // Flip a specific bit using flip()
    bitArray.flip(5);  // Flip bit at index 5 (0 becomes 1)
    cout << "After flipping bit at index 5: " << bitArray << endl;  // 0010001000000000

    // Access individual bits using the operator[]
    cout << "Bit at index 3: " << bitArray[3] << endl;  // Output: 0 (bit at index 3 is 0)

    // Access a range of bits using the to_string() function
    string bitString = bitArray.to_string();
    cout << "Bit Array as String: " << bitString << endl;  // Output: "0010001000000000"

    // Count the number of set bits (1s) using count()
    int setBits = bitArray.count();
    cout << "Number of set bits (1s): " << setBits << endl;  // Output: 3

    // Perform a bitwise AND operation with another bitset
    bitset<BIT_ARRAY_SIZE> anotherBitArray("1101101010101010");
    bitset<BIT_ARRAY_SIZE> andResult = bitArray & anotherBitArray;
    cout << "Result of AND operation: " << andResult << endl;  // Output: 0000001000000000

    // Perform a bitwise OR operation with another bitset
    bitset<BIT_ARRAY_SIZE> orResult = bitArray | anotherBitArray;
    cout << "Result of OR operation: " << orResult << endl;  // Output: 1111101010101010

    // Perform a bitwise XOR operation with another bitset
    bitset<BIT_ARRAY_SIZE> xorResult = bitArray ^ anotherBitArray;
    cout << "Result of XOR operation: " << xorResult << endl;  // Output: 1111100000101010

    // Perform a NOT operation (flip all bits)
    bitset<BIT_ARRAY_SIZE> notResult = ~bitArray;
    cout << "Result of NOT operation: " << notResult << endl;  // Output: 1101110111111111

    // Shift bits left (<<) and right (>>)
    bitset<BIT_ARRAY_SIZE> leftShiftResult = bitArray << 3;  // Shift left by 3 positions
    cout << "Result of left shift by 3: " << leftShiftResult << endl;  // Output: 1001000000000000

    bitset<BIT_ARRAY_SIZE> rightShiftResult = bitArray >> 2;  // Shift right by 2 positions
    cout << "Result of right shift by 2: " << rightShiftResult << endl;  // Output: 0000100100000000

    // Bit array to hexadecimal representation (useful for debugging or printing)
    unsigned long long hexValue = bitArray.to_ullong();  // Convert bitset to unsigned long long
    cout << "Bit array as hexadecimal: " << hex << hexValue << endl;  // Output: 0x9040
}

int main() {
    cout << "Demonstrating Bit Array Operations in C++:" << endl << endl;
    demonstrateBitArray();
    return 0;
}
