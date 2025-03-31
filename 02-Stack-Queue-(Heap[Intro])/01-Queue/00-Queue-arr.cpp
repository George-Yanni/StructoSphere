#include <iostream>
using namespace std;

// Queue implementation using an array
class Queue {
private:
    int* arr;
    int front;
    int rear;
    int capacity;
    int count;

public:
    // Constructor
    Queue(int size) {
        arr = new int[size];
        capacity = size;
        front = 0;
        rear = -1;
        count = 0;
    }

    // Destructor
    ~Queue() {
        delete[] arr;
    }

    // Enqueue operation (Insert element at rear)
    void enqueue(int item) {
        if (isFull()) {
            cout << "Queue is full. Cannot enqueue." << endl;
            return;
        }
        rear = (rear + 1) % capacity;
        arr[rear] = item;
        count++;
        cout << "Enqueued: " << item << endl;
    }

    // Dequeue operation (Remove element from front)
    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue." << endl;
            return -1;
        }
        int item = arr[front];
        front = (front + 1) % capacity;
        count--;
        cout << "Dequeued: " << item << endl;
        return item;
    }

    // Peek operation (Get front element)
    int peek() {
        if (isEmpty()) {
            cout << "Queue is empty. No front element." << endl;
            return -1;
        }
        return arr[front];
    }

    // Check if the queue is empty
    bool isEmpty() {
        return (count == 0);
    }

    // Check if the queue is full
    bool isFull() {
        return (count == capacity);
    }

    // Get queue size
    int size() {
        return count;
    }
};

// Demonstration of Queue operations
int main() {
    Queue q(5); // Create a queue of size 5

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.enqueue(50);
    q.enqueue(60); // Should indicate queue is full
    
    cout << "Front element: " << q.peek() << endl;
    
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue(); // Should indicate queue is empty
    
    return 0;
}
