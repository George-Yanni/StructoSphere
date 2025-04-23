#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

// Node structure for each block of memory in the free list
struct Node {
    size_t size;    // Size of the block of memory
    Node* next;     // Pointer to the next free block

    // Constructor
    Node(size_t s) : size(s), next(nullptr) {}
};

class FreeList {
private:
    Node* head;  // Head pointer to the free list (top of the list)

public:
    // Constructor
    FreeList() : head(nullptr) {}

    // Insert a block of memory back to the free list
    void insert(size_t blockSize) {
        cout << "Inserting block of size " << blockSize << " into the free list." << endl;
        Node* newBlock = new Node(blockSize);  // Create a new block

        // Insert the new block at the beginning of the free list
        newBlock->next = head;
        head = newBlock;
    }

    // Remove (allocate) a block of memory from the free list
    void* remove() {
        if (!head) {
            cout << "Free list is empty. Cannot allocate memory." << endl;
            return nullptr;
        }

        // Allocate the memory block by removing it from the free list
        Node* blockToAllocate = head;
        head = head->next;  // Move head pointer to the next block in the list

        cout << "Allocating block of size " << blockToAllocate->size << " from the free list." << endl;
        void* allocatedMemory = malloc(blockToAllocate->size);  // Simulating memory allocation

        delete blockToAllocate;  // Delete the block as it is no longer needed
        return allocatedMemory;  // Return the pointer to the allocated memory
    }

    // Print the current free list (for visualization)
    void printFreeList() {
        cout << "Current Free List: ";
        Node* current = head;
        while (current) {
            cout << "[" << current->size << "] ";
            current = current->next;
        }
        cout << endl;
    }

    // Destructor to clean up the allocated blocks in the free list
    ~FreeList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;  // Clean up each node
        }
    }
};

int main() {
    FreeList freeList;

    // Insert blocks of memory back to the free list
    freeList.insert(128);  // Inserting a block of 128 bytes
    freeList.insert(64);   // Inserting a block of 64 bytes
    freeList.insert(256);  // Inserting a block of 256 bytes

    // Print the free list after insertions
    freeList.printFreeList();

    // Remove (allocate) a block of memory from the free list
    freeList.remove();  // Allocate a block from the free list

    // Print the free list after allocation
    freeList.printFreeList();

    // Insert more blocks to the free list
    freeList.insert(512); // Insert a block of 512 bytes
    freeList.insert(32);  // Insert a block of 32 bytes

    // Print the free list after more insertions
    freeList.printFreeList();

    // Remove more blocks from the free list
    freeList.remove();  // Allocate another block
    freeList.remove();  // Allocate another block

    // Print the final free list
    freeList.printFreeList();

    return 0;
}
