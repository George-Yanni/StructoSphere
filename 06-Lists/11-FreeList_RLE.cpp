#include <iostream>
#include <cstring>  // For memcpy and memset

using namespace std;

class MemorySimulator {
public:
    struct MemoryBlock {
        int size;
        bool isFree;
        MemoryBlock* next;
    };

private:
    char memory[1024];  // A block of memory (1 KB)
    MemoryBlock* freeList;  // Head of the free list

    // Function to initialize the free list with the entire memory block
    void initializeFreeList() {
        freeList = reinterpret_cast<MemoryBlock*>(memory);
        freeList->size = sizeof(memory) - sizeof(MemoryBlock);  // Corrected size
        freeList->isFree = true;
        freeList->next = nullptr;
    }

    // Function to find a suitable free block for allocation
    MemoryBlock* findFreeBlock(int size) {
        MemoryBlock* current = freeList;
        while (current) {
            if (current->isFree && current->size >= size) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Function to split a block into two if the requested size is smaller
    void splitBlock(MemoryBlock* block, int size) {
        // Only split if we can fit a new block header plus some usable space
        int minSplitSize = sizeof(MemoryBlock) + 1;  // At least 1 byte of usable space
        
        if (block->size >= size + minSplitSize) {
            char* blockStart = reinterpret_cast<char*>(block);
            char* newBlockStart = blockStart + sizeof(MemoryBlock) + size;
            
            MemoryBlock* newBlock = reinterpret_cast<MemoryBlock*>(newBlockStart);
            newBlock->size = block->size - size - sizeof(MemoryBlock);
            newBlock->isFree = true;
            newBlock->next = block->next;
            
            block->size = size;
            block->next = newBlock;
        }
    }

    // Function to merge adjacent free blocks
    void mergeFreeBlocks() {
        MemoryBlock* current = freeList;
        while (current && current->next) {
            // Check if current and next blocks are adjacent in memory
            char* currentEnd = reinterpret_cast<char*>(current) + sizeof(MemoryBlock) + current->size;
            
            if (current->isFree && current->next->isFree && 
                currentEnd == reinterpret_cast<char*>(current->next)) {
                // Blocks are adjacent, merge them
                current->size += sizeof(MemoryBlock) + current->next->size;
                current->next = current->next->next;
            } else {
                current = current->next;
            }
        }
    }

public:
    // Constructor: Initialize all memory to 0 (representing empty) and set up the free list
    MemorySimulator() {
        memset(memory, 0, sizeof(memory));
        initializeFreeList();
    }

    // Public method to retrieve the memory block pointer
    void* allocate(int size) {
        MemoryBlock* block = findFreeBlock(size);
        if (!block) {
            cout << "Not enough free memory to allocate data!" << endl;
            return nullptr;
        }

        splitBlock(block, size);
        block->isFree = false;
        
        // Return pointer to the data area (after the block header)
        return reinterpret_cast<char*>(block) + sizeof(MemoryBlock);
    }

    // Function to simulate writing to a memory area
    void* writeToArea(const string& data) {
        int size = data.length() + 1;  // +1 for null terminator
        void* dataPtr = allocate(size);
        
        if (dataPtr) {
            // Write the data into the allocated block
            memcpy(dataPtr, data.c_str(), size);
        }
        
        return dataPtr;
    }

    // Function to simulate freeing a memory area
    void freeArea(void* dataPtr) {
        if (!dataPtr) {
            cout << "Invalid pointer!" << endl;
            return;
        }
        
        // Calculate the block address from the data pointer
        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(
            reinterpret_cast<char*>(dataPtr) - sizeof(MemoryBlock)
        );
        
        if (block->isFree) {
            cout << "Block is already free!" << endl;
            return;
        }

        block->isFree = true;
        mergeFreeBlocks();
    }

    // Function to simulate printing the memory areas
    void printMemoryAreas() {
        MemoryBlock* current = freeList;
        int totalFree = 0;
        int blockIndex = 0;

        cout << "Memory Areas:" << endl;
        while (current) {
            if (current->isFree) {
                cout << "Block " << blockIndex << ": Free, Size = " << current->size << " bytes" << endl;
                totalFree += current->size;
            } else {
                cout << "Block " << blockIndex << ": Used, Size = " << current->size << " bytes, Content = '";
                char* content = reinterpret_cast<char*>(current) + sizeof(MemoryBlock);
                cout << content << "'" << endl;
            }
            current = current->next;
            blockIndex++;
        }

        cout << "Total Free Memory: " << totalFree << " bytes" << endl;
    }
};

int main() {
    MemorySimulator memorySimulator;

    // Print initial memory areas (empty)
    cout << "Initial memory areas:" << endl;
    memorySimulator.printMemoryAreas();
    cout << endl;

    // Write data to memory and store the pointers
    void* ptr1 = memorySimulator.writeToArea("Hello, Area 0!");
    void* ptr2 = memorySimulator.writeToArea("Hello, Area 1!");

    // Print memory areas after writing
    cout << "Memory areas after writing:" << endl;
    memorySimulator.printMemoryAreas();
    cout << endl;

    // Free the first memory area
    memorySimulator.freeArea(ptr1);

    // Print memory areas after freeing
    cout << "Memory areas after freeing:" << endl;
    memorySimulator.printMemoryAreas();
    cout << endl;

    // Write more data to memory
    void* ptr3 = memorySimulator.writeToArea("Hello, Area 2!");

    // Print memory areas after writing more data
    cout << "Memory areas after writing more data:" << endl;
    memorySimulator.printMemoryAreas();

    return 0;
}