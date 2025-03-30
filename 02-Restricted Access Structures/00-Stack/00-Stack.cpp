#include <iostream>
#include <vector>
using namespace std;

// Stack Implementation using a dynamic array (vector)
class Stack {
private:
    vector<int> stack; // Internal container for stack elements

public:
    // Push an element onto the stack
    void push(int value) {
        stack.push_back(value);
        cout << "Pushed: " << value << endl;
    }

    // Pop an element from the stack
    int pop() {
        if (isEmpty()) {
            cerr << "Stack Underflow! Cannot pop from an empty stack." << endl;
            return -1; // Return an invalid value to indicate error
        }
        int topElement = stack.back();
        stack.pop_back();
        cout << "Popped: " << topElement << endl;
        return topElement;
    }

    // Peek the top element of the stack
    int peek() const {
        if (isEmpty()) {
            cerr << "Stack is empty! No top element to peek." << endl;
            return -1;
        }
        return stack.back();
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return stack.empty();
    }

    // Get the size of the stack
    size_t size() const {
        return stack.size();
    }

    // Display the stack contents
    void display() const {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return;
        }
        cout << "Stack elements (top to bottom): ";
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }
};

// Demonstration of stack operations
int main() {
    Stack s;
    
    cout << "Demonstrating Stack Operations:" << endl;
    s.push(10);
    s.push(20);
    s.push(30);
    s.display();
    
    cout << "Top element: " << s.peek() << endl;
    
    s.pop();
    s.display();
    
    cout << "Stack size: " << s.size() << endl;
    
    s.pop();
    s.pop();
    s.pop(); // Trying to pop from an empty stack
    
    return 0;
}
