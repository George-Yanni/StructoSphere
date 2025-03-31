#include <iostream>
#include <iomanip>  // For setw() formatting
using namespace std;

class CircularQueue {
private:
    int* arr;
    int capacity;
    int front;
    int rear;
    int size;

    // Helper function to visualize the queue
    void visualize(const string& operation) const {
        cout << "\n--- " << operation << " ---\n";
        cout << "Front: " << front << " | Rear: " << rear << " | Size: " << size << "\n";
        
        // Print indices
        cout << "Indices: ";
        for (int i = 0; i < capacity; i++) {
            cout << setw(3) << i << " ";
        }
        cout << "\n";
        
        // Print values
        cout << "Values:  ";
        for (int i = 0; i < capacity; i++) {
            if (i >= front && i <= rear && !isEmpty()) {
                cout << setw(3) << arr[i] << " ";
            } else if (rear < front && (i <= rear || i >= front) && !isEmpty()) {
                cout << setw(3) << arr[i] << " ";
            } else {
                cout << setw(3) << "_" << " ";
            }
        }
        cout << "\n\n";
    }

public:
    CircularQueue(int cap) : capacity(cap), front(0), rear(-1), size(0) {
        arr = new int[capacity];
    }

    ~CircularQueue() {
        delete[] arr;
    }

    bool isFull() const { return size == capacity; }
    bool isEmpty() const { return size == 0; }

    void enqueue(int value) {
        if (isFull()) {
            cout << "Queue is full! Cannot enqueue " << value << ".\n";
            visualize("Enqueue Failed (Full)");
            return;
        }
        rear = (rear + 1) % capacity;
        arr[rear] = value;
        size++;
        visualize("Enqueue " + to_string(value));
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty! Cannot dequeue.\n";
            visualize("Dequeue Failed (Empty)");
            return -1;
        }
        int value = arr[front];
        front = (front + 1) % capacity;
        size--;
        visualize("Dequeue " + to_string(value));
        return value;
    }

    int peek() const {
        if (isEmpty()) {
            cout << "Queue is empty! No front element.\n";
            return -1;
        }
        return arr[front];
    }

    void display() const {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }
        cout << "Queue contents (in order): ";
        int count = 0;
        int index = front;
        while (count < size) {
            cout << arr[index] << " ";
            index = (index + 1) % capacity;
            count++;
        }
        cout << "\n";
    }
};

int main() {
    CircularQueue cq(5);

    cout << "===== Testing Circular Queue =====\n";

    // Test enqueue
    cq.enqueue(10);
    cq.enqueue(20);
    cq.enqueue(30);
    cq.enqueue(40);
    cq.enqueue(50);

    // Test overflow
    cq.enqueue(60);

    // Test dequeue
    cq.dequeue();
    cq.dequeue();

    // Test circular behavior
    cq.enqueue(60);
    cq.enqueue(70);

    // Display final state
    cout << "\n===== Final State =====\n";
    cq.display();

    return 0;
}