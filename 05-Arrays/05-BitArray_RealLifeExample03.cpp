#include <iostream>
#include <vector>

using namespace std;

void demonstrateCompression() {
    // Original sequence of boolean values (0 or 1)
    vector<bool> originalData = {1, 0, 1, 1, 0, 1, 0, 1};

    // Dynamically determine the size of the bit array required
    size_t size = originalData.size();

    // Compress the data by storing it as a bit array (using an integer representation for simplicity)
    uint8_t compressedData = 0;  // A byte can hold 8 bits (assuming original data fits in a byte)

    for (size_t i = 0; i < size; ++i) {
        if (originalData[i]) {
            compressedData |= (1 << (size - 1 - i));  // Set the bit at the correct position
        }
    }

    // Output the compressed data
    cout << "Compressed Data (Bit Array): ";
    for (size_t i = 0; i < size; ++i) {
        cout << ((compressedData >> (size - 1 - i)) & 1);
    }
    cout << endl;

    // Decompress the data (convert back to the original sequence)
    vector<bool> decompressedData(size);
    for (size_t i = 0; i < size; ++i) {
        decompressedData[i] = (compressedData >> (size - 1 - i)) & 1;
    }

    // Display decompressed data
    cout << "Decompressed Data: ";
    for (bool bit : decompressedData) {
        cout << bit << " ";
    }
    cout << endl;
}

int main() {
    cout << "Demonstrating Compression using Bit Array:" << endl << endl;
    demonstrateCompression();
    return 0;
}
