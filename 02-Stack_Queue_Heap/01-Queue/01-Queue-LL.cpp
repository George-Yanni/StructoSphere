#include <iostream>
using namespace std;

// Node structure for linked list implementation of queue
struct Node {
    int data;
    Node* next;

    Node(int val) {
        data = val;
        next = nullptr;
    }
};

// Queue implementation using a linked list
class Queue {
private:
    Node* front;
    Node* rear;
    int count;

public:
    // Constructor
    Queue() {
        front = rear = nullptr;
        count = 0;
    }

    // Destructor
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue operation (Insert element at rear)
    void enqueue(int item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
        cout << "Enqueued: " << item << endl;
    }

    // Dequeue operation (Remove element from front)
    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue." << endl;
            return -1;
        }
        Node* temp = front;
        int item = front->data;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
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
        return front->data;
    }

    // Check if the queue is empty
    bool isEmpty() {
        return (front == nullptr);
    }

    // Get queue size
    int size() {
        return count;
    }
};

// Demonstration of Queue operations
int main() {
    Queue q; // Create a queue

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.enqueue(50);
    
    cout << "Front element: " << q.peek() << endl;
    
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue(); // Should indicate queue is empty
    
    return 0;
}
