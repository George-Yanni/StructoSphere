#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

class Bitmap {
private:
    vector<uint8_t> bits;
    size_t size;

public:
    // Constructor to initialize the bitmap of given size (in bits)
    Bitmap(size_t bitSize) : size(bitSize) {
        size_t byteCount = (bitSize + 7) / 8; // ceil(bitSize / 8)
        bits.resize(byteCount, 0);
    }

    // Set a bit to 1
    void set(size_t index) {
        if (index >= size) throw out_of_range("Index out of bounds");
        bits[index / 8] |= (1 << (index % 8));
    }

    // Clear a bit to 0
    void clear(size_t index) {
        if (index >= size) throw out_of_range("Index out of bounds");
        bits[index / 8] &= ~(1 << (index % 8));
    }

    // Toggle a bit
    void toggle(size_t index) {
        if (index >= size) throw out_of_range("Index out of bounds");
        bits[index / 8] ^= (1 << (index % 8));
    }

    // Check if a bit is set
    bool test(size_t index) const {
        if (index >= size) throw out_of_range("Index out of bounds");
        return bits[index / 8] & (1 << (index % 8));
    }

    // Print the bitmap (for visualization)
    void print() const {
        cout << "Bitmap bits: ";
        for (size_t i = 0; i < size; ++i) {
            cout << test(size - 1 - i);
        }
        cout << "\n";
    }

    // Return total size in bits
    size_t bitSize() const { return size; }

    // Return total size in bytes
    size_t byteSize() const { return bits.size(); }
};

int main() {
    Bitmap bitmap(16); // A bitmap of 16 bits (2 bytes)

    bitmap.set(1);
    bitmap.set(3);
    bitmap.set(8);
    bitmap.toggle(3);
    bitmap.clear(8);

    bitmap.print();

    cout << "Bit at index 1: " << bitmap.test(1) << "\n";
    cout << "Bit at index 3: " << bitmap.test(3) << "\n";
    cout << "Size in bits: " << bitmap.bitSize() << "\n";
    cout << "Size in bytes: " << bitmap.byteSize() << "\n";

    return 0;
}
