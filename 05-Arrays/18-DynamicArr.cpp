#include <iostream>
using namespace std;

// Custom DynamicArray class to simulate basic dynamic array behavior
class DynamicArray {
private:
    int* data;        // Pointer to the actual array
    int capacity;     // Total allocated memory (number of elements it can hold)
    int size;         // Current number of elements in the array

public:
    // Constructor to initialize with an initial capacity
    DynamicArray(int initialCapacity = 2) {
        capacity = initialCapacity;
        size = 0;
        data = new int[capacity];  // Allocate memory for initial capacity
    }

    // Destructor to free allocated memory
    ~DynamicArray() {
        delete[] data;
    }

    // Function to add an element at the end (like push_back)
    void push_back(int value) {
        // If size exceeds capacity, we need to resize the array
        if (size == capacity) {
            resize();  // Double the capacity
        }
        data[size] = value;  // Add element at the end
        size++;              // Increase current size
    }

    // Function to get an element at a specific index
    int get(int index) const {
        if (index < 0 || index >= size) {
            cerr << "Index out of bounds\n";
            return -1;  // Handle error safely
        }
        return data[index];
    }

    // Function to get current size
    int getSize() const {
        return size;
    }

    // Function to get current capacity
    int getCapacity() const {
        return capacity;
    }

    // Print all elements in the array
    void print() const {
        for (int i = 0; i < size; ++i)
            cout << data[i] << " ";
        cout << endl;
    }

private:
    // Private function to resize the array (usually doubling capacity)
    void resize() {
        int newCapacity = capacity * 2;
        int* newData = new int[newCapacity];  // Allocate new memory

        // Copy elements from old array to new one
        for (int i = 0; i < size; ++i)
            newData[i] = data[i];

        delete[] data;     // Free old memory
        data = newData;    // Point to new memory
        capacity = newCapacity;  // Update capacity
    }
};

// Driver code to test the DynamicArray
int main() {
    DynamicArray arr;

    // Adding elements
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    arr.push_back(40);

    // Output current state
    cout << "Array contents: ";
    arr.print();

    cout << "Size: " << arr.getSize() << endl;
    cout << "Capacity: " << arr.getCapacity() << endl;

    // Accessing element by index
    cout << "Element at index 2: " << arr.get(2) << endl;

    return 0;
}
