#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iomanip>

class MinHeap {
private:
    std::vector<int> heap;

    // Helper functions
    int parent(int index) const { return (index - 1) / 2; }
    int leftChild(int index) const { return 2 * index + 1; }
    int rightChild(int index) const { return 2 * index + 2; }

    void swap(int& a, int& b) { 
        int temp = a; 
        a = b; 
        b = temp; 
    }

    void heapifyUp(int index) {
        while (index > 0 && heap[parent(index)] > heap[index]) {
            std::cout << "Heapify Up: Swapping " << heap[parent(index)] << " and " << heap[index] << "\n";
            swap(heap[parent(index)], heap[index]);
            printHeap();
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        int minIndex = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < heap.size() && heap[left] < heap[minIndex]) 
            minIndex = left;
        if (right < heap.size() && heap[right] < heap[minIndex]) 
            minIndex = right;

        if (minIndex != index) {
            std::cout << "Heapify Down: Swapping " << heap[index] << " and " << heap[minIndex] << "\n";
            swap(heap[index], heap[minIndex]);
            printHeap();
            heapifyDown(minIndex);
        }
    }

public:
    MinHeap() {}

void printHeap() const {
    if (isEmpty()) {
        std::cout << "Heap is empty\n";
        return;
    }

    int levels = std::ceil(std::log2(heap.size() + 1));
    int maxWidth = std::pow(2, levels - 1) * 4;

    int index = 0;
    for (int level = 0; level < levels; ++level) {
        int nodes = std::pow(2, level);
        int spacing = maxWidth / (nodes + 1);

        // Print nodes
        for (int i = 0; i < nodes && index < heap.size(); ++i) {
            std::cout << std::setw(spacing) << heap[index++];
        }
        std::cout << "\n";

        // Print arrows (only if not the last level and there are children)
        if (level < levels - 1 && index < heap.size()) {
            int arrowNodes = std::min(nodes, static_cast<int>(heap.size()) - nodes);
            for (int i = 0; i < arrowNodes; ++i) {
                std::cout << std::setw(spacing - 1) << "/" << std::setw(2) << "\\";
            }
            std::cout << "\n";
        }
    }
    std::cout << "\nArray: [ ";
    for (int num : heap) std::cout << num << " ";
    std::cout << "]\n\n";
}

    bool isEmpty() const { return heap.empty(); }

    void insert(int value) {
        std::cout << "\n--- Inserting " << value << " ---\n";
        heap.push_back(value);
        std::cout << "Added to end of heap:\n";
        printHeap();
        heapifyUp(heap.size() - 1);
    }

    int extractMin() {
        if (isEmpty()) throw std::runtime_error("Heap is empty");
        
        std::cout << "\n--- Extracting Minimum ---\n";
        int min = heap[0];
        std::cout << "Removing root value: " << min << "\n";
        
        heap[0] = heap.back();
        heap.pop_back();
        
        std::cout << "Moved last element to root:\n";
        printHeap();
        
        if (!isEmpty()) {
            heapifyDown(0);
        }
        return min;
    }

    void buildHeap(const std::vector<int>& arr) {
        std::cout << "\n--- Building Heap from Array ---\n";
        heap = arr;
        printHeap();
        
        for (int i = (heap.size() / 2) - 1; i >= 0; --i) {
            std::cout << "Heapifying from index " << i << " (value: " << heap[i] << ")\n";
            heapifyDown(i);
        }
    }
};

int main() {
    MinHeap heap;
    
    std::cout << "=== MINHEAP VISUAL DEMONSTRATION ===\n\n";
    
    // Insertion demo
    heap.insert(10);
    heap.insert(5);
    heap.insert(15);
    heap.insert(3);
    heap.insert(7);
    
    // Extraction demo
    heap.extractMin();
    
    // Build heap demo
    heap.buildHeap({12, 11, 13, 5, 6, 7});
    
    return 0;
}