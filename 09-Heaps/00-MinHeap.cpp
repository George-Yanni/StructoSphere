#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class MinHeap {
private:
    vector<int> heap;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2 * i + 1); }
    int right(int i) { return (2 * i + 2); }

    void heapifyDown(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        if (l < heap.size() && heap[l] < heap[smallest])
            smallest = l;
        if (r < heap.size() && heap[r] < heap[smallest])
            smallest = r;
        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)] > heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    MinHeap() {}

    int getMin() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }
        return heap[0];
    }

    void insert(int key) {
        heap.push_back(key);
        heapifyUp(heap.size() - 1);
    }

    void decreaseKey(int i, int new_val) {
        if (i >= heap.size())
            throw out_of_range("Index out of range");

        heap[i] = new_val;
        heapifyUp(i);
    }

    int extractMin() {
        if (heap.empty())
            throw runtime_error("Heap is empty");

        if (heap.size() == 1) {
            int root = heap[0];
            heap.pop_back();
            return root;
        }

        int root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);

        return root;
    }

    void deleteKey(int i) {
        if (i >= heap.size())
            throw out_of_range("Index out of range");

        decreaseKey(i, INT_MIN);
        extractMin();
    }

    void printTree(int i = 0, int level = 0) {
        if (i >= heap.size())
            return;

        // Print right child first (higher on screen)
        printTree(right(i), level + 1);

        // Print current node
        cout << setw(4 * level) << "";
        cout << heap[i] << endl;

        // Print left child
        printTree(left(i), level + 1);
    }

    void printArray() {
        for (int val : heap)
            cout << val << " ";
        cout << endl;
    }

    int size() {
        return heap.size();
    }
};

int main() {
    MinHeap h;

    h.insert(3);
    cout << "Inserted 3\n";
    h.printTree();
    h.insert(2);
    cout << "Inserted 2\n";
    h.printTree();
    h.insert(15);
    cout << "Inserted 15\n";
    h.printTree();
    h.insert(5);
    cout << "Inserted 5\n";
    h.printTree();
    h.insert(4);
    cout << "Inserted 4\n";
    h.printTree();
    h.insert(45);
    cout << "Inserted 45\n";
    h.printTree();

    cout << "Heap as array: ";
    h.printArray();

    cout << "\nHeap as tree:\n";
    h.printTree();

    cout << "\nExtracted Min: " << h.extractMin() << endl;

    cout << "\nHeap after extractMin:\n";
    h.printTree();

    h.decreaseKey(2, 1);
    cout << "\nHeap after decreaseKey at index 2 to 1:\n";
    h.printTree();

    h.deleteKey(1);
    cout << "\nHeap after deleteKey at index 1:\n";
    h.printTree();

    return 0;
}
