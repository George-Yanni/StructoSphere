#include <iostream>
#include <bitset>
#include <type_traits>

class BitFieldExample {
public:
    // Defining bit fields within a struct
    struct StatusFlags {
        unsigned int flag1 : 1;  // 1 bit
        unsigned int flag2 : 3;  // 3 bits
        unsigned int flag3 : 2;  // 2 bits
        unsigned int reserved : 10; // 10 bits reserved
        unsigned int flag4 : 4;  // 4 bits
    };

    StatusFlags status;

    // Constructor to initialize all flags to 0
    BitFieldExample() {
        status.flag1 = 0;
        status.flag2 = 0;
        status.flag3 = 0;
        status.reserved = 0;
        status.flag4 = 0;
    }

    void setFlags(unsigned int f1, unsigned int f2, unsigned int f3, unsigned int f4) {
        status.flag1 = f1;
        status.flag2 = f2;
        status.flag3 = f3;
        status.flag4 = f4;
    }

    void printFlags() {
        std::cout << "Flag1: " << status.flag1 << std::endl;
        std::cout << "Flag2: " << status.flag2 << std::endl;
        std::cout << "Flag3: " << status.flag3 << std::endl;
        std::cout << "Reserved: " << status.reserved << std::endl;
        std::cout << "Flag4: " << status.flag4 << std::endl;
    }

    // Function to display the entire status structure in binary
    void displayBinary() {
        std::bitset<20> binary(
            (status.flag1) | 
            (status.flag2 << 1) | 
            (status.flag3 << 4) | 
            (status.reserved << 6) |
            (status.flag4 << 16)
        );
        std::cout << "Binary Representation of Status Flags: " << binary << std::endl;
    }

    // Function to show memory size and layout of bitfields
    void showSizeAndLayout() {
        std::cout << "Size of StatusFlags struct: " << sizeof(StatusFlags) << " bytes" << std::endl;
        std::cout << "Size of 'status' bit field: " << sizeof(status) << " bytes" << std::endl;
    }
};

int main() {
    // Create an instance of BitFieldExample
    BitFieldExample bfe;

    // Set flags with values
    bfe.setFlags(1, 5, 2, 10);  // Example values for the flags

    // Print individual flags
    bfe.printFlags();

    // Display binary representation of the entire structure
    bfe.displayBinary();

    // Show memory layout and size
    bfe.showSizeAndLayout();

    return 0;
}
