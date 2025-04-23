#include <iostream>
using namespace std;

/*
===============================================================================
                 Self-Organizing Linked List using Move-To-Front
===============================================================================
This is a singly linked list where frequently accessed elements are moved 
toward the front. It improves average access time for skewed access patterns.

Heuristic: Move-to-Front (MTF)
 - Every time an element is accessed, it's moved to the front of the list.

Key Operations:
 - insert(value): Add node to the end of the list
 - search(value): Search for a value, move it to the front if found
 - print(): Display the current list

Real-world Use Case:
 - Caching or auto-complete systems where frequently accessed items 
   should be faster to reach.
===============================================================================
*/

class SelfOrganizingList {
private:
    struct Node {
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };

    Node* head;

public:
    SelfOrganizingList() : head(nullptr) {}

    // Insert at the end
    void insert(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
    }

    // Search and move the found node to the front (Move-to-Front strategy)
    bool search(int value) {
        if (!head)
            return false;

        if (head->data == value) {
            return true; // Already at front
        }

        Node* prev = head;
        Node* curr = head->next;

        while (curr) {
            if (curr->data == value) {
                // Move curr to front
                prev->next = curr->next;
                curr->next = head;
                head = curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false; // Not found
    }

    // Display the current list
    void print() const {
        Node* temp = head;
        cout << "List: ";
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    // Cleanup
    ~SelfOrganizingList() {
        Node* temp = head;
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
};

// Example usage
int main() {
    SelfOrganizingList list;
    list.insert(10);
    list.insert(20);
    list.insert(30);
    list.insert(40);
    list.insert(50);

    list.print(); // List: 10 20 30 40 50

    cout << "Searching 30...\n";
    list.search(30);
    list.print(); // List: 30 10 20 40 50

    cout << "Searching 50...\n";
    list.search(50);
    list.print(); // List: 50 30 10 20 40

    cout << "Searching 60 (not in list)...\n";
    list.search(60);
    list.print(); // No change

    return 0;
}
