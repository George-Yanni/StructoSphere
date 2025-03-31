#include <iostream>
using namespace std;

// Structure for a Node in the deque
struct Node {
    int data;          // Data value stored in the node
    Node* next;        // Pointer to the next node
    Node* prev;        // Pointer to the previous node
    
    // Constructor to initialize a node
    Node(int value) : data(value), next(nullptr), prev(nullptr) {}
};

// Deque class definition
class Deque {
private:
    Node* front;       // Pointer to the front of the deque
    Node* rear;        // Pointer to the rear of the deque
    int size;          // Current size of the deque

public:
    // Constructor: Initializes an empty deque
    Deque() : front(nullptr), rear(nullptr), size(0) {}

    // Destructor: Cleans up memory by deleting all nodes
    ~Deque() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    // Check if deque is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Get current size of deque
    int getSize() const {
        return size;
    }

    // Add element to the front of the deque
    void push_front(int value) {
        Node* newNode = new Node(value);  // Create new node
        if (isEmpty()) {                  // If deque is empty
            front = rear = newNode;       // Front and rear point to the same node
        } else {                          // If deque has elements
            newNode->next = front;        // New node's next points to current front
            front->prev = newNode;        // Current front's prev points to new node
            front = newNode;              // Update front to new node
        }
        size++;                           // Increment size
        cout << "Pushed " << value << " to front\n";
    }

    // Add element to the back of the deque
    void push_back(int value) {
        Node* newNode = new Node(value);  // Create new node
        if (isEmpty()) {                  // If deque is empty
            front = rear = newNode;       // Front and rear point to the same node
        } else {                          // If deque has elements
            newNode->prev = rear;         // New node's prev points to current rear
            rear->next = newNode;         // Current rear's next points to new node
            rear = newNode;               // Update rear to new node
        }
        size++;                           // Increment size
        cout << "Pushed " << value << " to back\n";
    }

    // Remove element from the front of the deque
    void pop_front() {
        if (isEmpty()) {                  // Check if deque is empty
            cout << "Deque is empty, cannot pop front\n";
            return;
        }
        Node* temp = front;               // Store front node to delete later
        cout << "Popped " << front->data << " from front\n";
        if (front == rear) {              // If only one element
            front = rear = nullptr;       // Deque becomes empty
        } else {                          // If more than one element
            front = front->next;          // Move front to next node
            front->prev = nullptr;        // New front's prev is null
        }
        delete temp;                      // Free memory
        size--;                           // Decrement size
    }

    // Remove element from the back of the deque
    void pop_back() {
        if (isEmpty()) {                  // Check if deque is empty
            cout << "Deque is empty, cannot pop back\n";
            return;
        }
        Node* temp = rear;                // Store rear node to delete later
        cout << "Popped " << rear->data << " from back\n";
        if (front == rear) {              // If only one element
            front = rear = nullptr;       // Deque becomes empty
        } else {                          // If more than one element
            rear = rear->prev;            // Move rear to previous node
            rear->next = nullptr;         // New rear's next is null
        }
        delete temp;                      // Free memory
        size--;                           // Decrement size
    }

    // Display the deque from front to rear
    void display() const {
        if (isEmpty()) {                  // Check if deque is empty
            cout << "Deque is empty\n";
            return;
        }
        cout << "Deque contents (front to rear): ";
        Node* current = front;            // Start from front
        while (current != nullptr) {      // Traverse until end
            cout << current->data << " ";
            current = current->next;      // Move to next node
        }
        cout << "\nSize: " << size << "\n";
    }
};

// Main function to demonstrate deque operations
int main() {
    Deque dq;  // Create an empty deque
    
    cout << "Demonstrating Deque Operations:\n";
    cout << "--------------------------------\n";

    // Test 1: Add elements to front and back
    dq.push_front(10);    // Add 10 to front
    dq.push_back(20);     // Add 20 to back
    dq.push_front(5);     // Add 5 to front
    dq.push_back(30);     // Add 30 to back
    dq.display();         // Show current deque: 5 10 20 30

    // Test 2: Remove from front
    dq.pop_front();       // Remove 5 from front
    dq.display();         // Show current deque: 10 20 30

    // Test 3: Remove from back
    dq.pop_back();        // Remove 30 from back
    dq.display();         // Show current deque: 10 20

    // Test 4: Add more elements
    dq.push_front(15);    // Add 15 to front
    dq.push_back(25);     // Add 25 to back
    dq.display();         // Show current deque: 15 10 20 25

    // Test 5: Remove all elements
    dq.pop_front();       // Remove 15
    dq.pop_back();        // Remove 25
    dq.pop_front();       // Remove 10
    dq.pop_back();        // Remove 20
    dq.display();         // Show empty deque

    // Test 6: Try popping from empty deque
    dq.pop_front();       // Should show error message
    dq.pop_back();        // Should show error message

    return 0;
}