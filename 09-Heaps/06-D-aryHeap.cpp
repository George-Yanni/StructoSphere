#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <queue>

// A class template for a D-ary Min Heap
template <typename T>
class DaryHeap {
private:
    std::vector<T> heap; // The array to store heap elements
    int degree;          // The degree D of the heap
    int capacity;        // Maximum capacity of the heap

    // Helper function to get the parent index of a node
    int parent(int i) const {
        if (i == 0) return -1; // Root has no parent
        return (i - 1) / degree;
    }

    // Helper function to get the index of the k-th child of a node
    int child(int i, int k) const {
        return degree * i + k;
    }

    // Helper function to maintain the heap property by moving an element down
    void heapifyDown(int i) {
        int min_child_index;
        while (true) {
            min_child_index = -1;
            T min_child_value = std::numeric_limits<T>::max(); // Initialize with max possible value

            // Find the smallest child among the D children
            for (int k = 1; k <= degree; ++k) {
                int current_child_index = child(i, k);
                if (current_child_index < heap.size()) { // Check if child exists
                    if (min_child_index == -1 || heap[current_child_index] < min_child_value) {
                        min_child_value = heap[current_child_index];
                        min_child_index = current_child_index;
                    }
                } else {
                    // No more children in this range
                    break;
                }
            }

            // If no children or the current node is smaller than its smallest child, stop
            if (min_child_index == -1 || heap[i] <= heap[min_child_index]) {
                break;
            }

            // Swap the current node with the smallest child
            std::swap(heap[i], heap[min_child_index]);

            // Move down to the child's position and continue heapifying
            i = min_child_index;
        }
    }

    // Helper function to maintain the heap property by moving an element up
    void heapifyUp(int i) {
        while (i > 0 && heap[i] < heap[parent(i)]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    // Constructor
    DaryHeap(int d, int cap) : degree(d), capacity(cap) {
        if (d <= 1) {
            throw std::invalid_argument("Degree D must be greater than 1.");
        }
        if (cap <= 0) {
            throw std::invalid_argument("Capacity must be positive.");
        }
        heap.reserve(capacity); // Reserve memory
    }

    // Get the current size of the heap
    int size() const {
        return heap.size();
    }

    // Check if the heap is empty
    bool isEmpty() const {
        return heap.empty();
    }

    // Check if the heap is full
    bool isFull() const {
        return heap.size() == capacity;
    }

    // Insert an element into the heap
    void insert(const T& value) {
        if (isFull()) {
            std::cerr << "Error: Heap is full. Cannot insert." << std::endl;
            return;
        }
        heap.push_back(value); // Add to the end
        heapifyUp(heap.size() - 1); // Heapify up the new element
    }

    // Extract the minimum element from the heap (for a min-heap)
    T extractMin() {
        if (isEmpty()) {
            throw std::out_of_range("Heap is empty. Cannot extract minimum.");
        }
        T min_value = heap[0]; // The minimum is at the root
        heap[0] = heap.back(); // Move the last element to the root
        heap.pop_back();       // Remove the last element
        if (!isEmpty()) {
            heapifyDown(0);    // Heapify down the new root
        }
        return min_value;
    }

    // Decrease the key of an element at a given index (for a min-heap)
    void decreaseKey(int i, const T& newValue) {
        if (i < 0 || i >= heap.size()) {
            throw std::out_of_range("Invalid index.");
        }
        if (newValue > heap[i]) {
            std::cerr << "Warning: New value (" << newValue << ") is greater than current value (" << heap[i] << ") at index " << i << ". Performing heapifyDown instead of heapifyUp." << std::endl;
            heap[i] = newValue; // Update the value
            heapifyDown(i);     // Heapify down to restore heap property
        } else {
            heap[i] = newValue; // Update the value
            heapifyUp(i);       // Heapify up to restore heap property
        }
    }

    // Get the minimum element without removing it
    const T& peekMin() const {
        if (isEmpty()) {
            throw std::out_of_range("Heap is empty. No minimum element.");
        }
        return heap[0];
    }

    // Build a heap from a vector of elements
    void buildHeap(const std::vector<T>& elements) {
        heap = elements; // Copy elements to the heap vector
        if (heap.size() > capacity) {
            std::cerr << "Warning: Input elements exceed heap capacity. Building with available elements." << std::endl;
            heap.resize(capacity); // Truncate if exceeds capacity
        }

        // Start from the last non-leaf node and heapify down
        int start_index = (heap.size() > 1) ? (heap.size() - 2) / degree : -1;

        for (int i = start_index; i >= 0; --i) {
            heapifyDown(i);
        }
    }

    // Function to print the heap using level-order traversal with parent-child mapping
    void printTree() const {
        if (isEmpty()) {
            std::cout << "Heap is empty." << std::endl;
            return;
        }

        std::cout << "D-ary Heap (D=" << degree << "):" << std::endl;

        // Calculate maximum width for values and indices
        size_t max_value_width = 0;
        size_t max_index_width = 0;
        for (size_t i = 0; i < heap.size(); ++i) {
            std::stringstream ss;
            ss << heap[i];
            max_value_width = std::max(max_value_width, ss.str().length());
            ss.str("");
            ss << i;
            max_index_width = std::max(max_index_width, ss.str().length());
        }
        max_value_width = std::max(size_t(4), max_value_width + 1);
        max_index_width = std::max(size_t(4), max_index_width + 1);

        // Use a queue for level-order traversal
        std::queue<std::pair<int, int>> q; // Pair of index and level
        q.push({0, 0}); // Start with root at level 0
        int current_level = 0;
        std::vector<std::string> level_nodes;

        while (!q.empty()) {
            auto [index, level] = q.front();
            q.pop();

            // Start a new level if necessary
            if (level > current_level) {
                // Print accumulated nodes for the previous level
                std::cout << "Level " << current_level << ":\n";
                for (const auto& node : level_nodes) {
                    std::cout << node << "\n";
                }
                std::cout << "\n";
                level_nodes.clear();
                current_level = level;
            }

            // Format the current node
            std::stringstream ss;
            ss << "Index " << std::setw(max_index_width) << std::left << index
               << ": Value " << std::setw(max_value_width) << std::left << heap[index]
               << ", Children: [";
            std::vector<int> children;
            for (int k = 1; k <= degree; ++k) {
                int child_index = child(index, k);
                if (child_index < heap.size()) {
                    children.push_back(child_index);
                    q.push({child_index, level + 1});
                }
            }
            for (size_t i = 0; i < children.size(); ++i) {
                ss << children[i];
                if (i < children.size() - 1) ss << ", ";
            }
            ss << "]";
            level_nodes.push_back(ss.str());
        }

        // Print the last level
        if (!level_nodes.empty()) {
            std::cout << "Level " << current_level << ":\n";
            for (const auto& node : level_nodes) {
                std::cout << node << "\n";
            }
            std::cout << "\n";
        }

        std::cout << "----------------------------------------" << std::endl;
    }

    // Public method to access an element by index (use with caution, doesn't guarantee heap property)
    const T& getElementAtIndex(int i) const {
        if (i < 0 || i >= heap.size()) {
            throw std::out_of_range("Invalid index.");
        }
        return heap[i];
    }
};

// Example Usage
int main() {
    try {
        // Create a 3-ary heap with capacity 20
        DaryHeap<int> heap(3, 20);

        // Insert elements
        heap.insert(10);
        std::cout << "Inserted 10:\n";
        heap.printTree();
        
        heap.insert(4);
        std::cout << "Inserted 4:\n";
        heap.printTree();
        
        heap.insert(15);
        std::cout << "Inserted 15:\n";
        heap.printTree();
        
        heap.insert(2);
        std::cout << "Inserted 2:\n";
        heap.printTree();
        
        heap.insert(8);
        std::cout << "Inserted 8:\n";
        heap.printTree();
        
        heap.insert(12);
        std::cout << "Inserted 12:\n";
        heap.printTree();
        
        heap.insert(18);
        std::cout << "Inserted 18:\n";
        heap.printTree();
        
        heap.insert(1);
        std::cout << "Inserted 1:\n";
        heap.printTree();
        
        heap.insert(6);
        std::cout << "Inserted 6:\n";
        heap.printTree();
        
        heap.insert(11);
        std::cout << "Inserted 11:\n";
        heap.printTree();

        std::cout << "Heap after insertions:" << std::endl;
        heap.printTree();

        // Extract minimum
        int min_val = heap.extractMin();
        std::cout << "Extracted minimum: " << min_val << std::endl;
        std::cout << "Heap after extracting minimum:" << std::endl;
        heap.printTree();

        // Decrease key
        if (heap.size() > 3) {
            std::cout << "Decreasing value at index 3 (which is " << heap.getElementAtIndex(3) << ") to 3." << std::endl;
            heap.decreaseKey(3, 3);
            std::cout << "Heap after decreasing key:" << std::endl;
            heap.printTree();
        } else {
            std::cout << "Heap size is too small to decrease key at index 3." << std::endl;
        }

        // Build heap from a vector
        std::vector<int> data = {5, 3, 17, 10, 8, 19, 1, 4, 9, 7};
        DaryHeap<int> built_heap(4, 15); // Create a 4-ary heap
        built_heap.buildHeap(data);
        std::cout << "Heap built from vector (4-ary heap):" << std::endl;
        built_heap.printTree();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}