#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class MaxHeap
{
private:
    vector<int> heap;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2 * i + 1); }
    int right(int i) { return (2 * i + 2); }

    void heapifyDown(int i)
    {
        int l = left(i);
        int r = right(i);
        int largest = i;

        if (l < heap.size() && heap[l] > heap[largest])
            largest = l;
        if (r < heap.size() && heap[r] > heap[largest])
            largest = r;
        if (largest != i)
        {
            swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int i)
    {
        while (i != 0 && heap[parent(i)] < heap[i])
        {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    MaxHeap() {}

    int getMax()
    {
        if (heap.empty())
        {
            throw runtime_error("Heap is empty");
        }
        return heap[0];
    }

    void insert(int key)
    {
        heap.push_back(key);
        heapifyUp(heap.size() - 1);
    }

    void decreaseKey(int i, int new_val)
    {
        if (i >= heap.size())
            throw out_of_range("Index out of range");

        heap[i] = new_val;
        heapifyDown(i); // In max heap, decreasing might require pushing down
    }

    int extractMax()
    {
        if (heap.empty())
            throw runtime_error("Heap is empty");

        if (heap.size() == 1)
        {
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

    void deleteKey(int i)
    {
        if (i >= heap.size())
            throw out_of_range("Index out of range");

        heap[i] = INT_MAX;
        heapifyUp(i);
        extractMax();
    }

    void printTree(int i = 0, int level = 0)
    {
        if (i >= heap.size())
            return;

        printTree(right(i), level + 1);

        cout << setw(4 * level) << "";
        cout << heap[i] << endl;

        printTree(left(i), level + 1);
    }

    void printArray()
    {
        for (int val : heap)
            cout << val << " ";
        cout << endl;
    }

    int size()
    {
        return heap.size();
    }
};

int main()
{
    MaxHeap h;

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

    cout << "\nExtracted Max: " << h.extractMax() << endl;

    cout << "\nHeap after extractMax:\n";
    h.printTree();

    h.decreaseKey(2, 1);
    cout << "\nHeap after decreaseKey at index 2 to 1:\n";
    h.printTree();

    h.deleteKey(1);
    cout << "\nHeap after deleteKey at index 1:\n";
    h.printTree();

    return 0;
}
