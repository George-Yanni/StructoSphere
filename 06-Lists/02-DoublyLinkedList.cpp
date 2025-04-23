#include <iostream>

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        Node(int val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    // Insert at the beginning
    void insertFront(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    // Insert at the end
    void insertBack(int val) {
        Node* newNode = new Node(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // Insert at a specific position (0-based index)
    void insertAt(int index, int val) {
        if (index < 0) return;

        if (index == 0) {
            insertFront(val);
            return;
        }

        Node* current = head;
        for (int i = 0; current && i < index - 1; ++i)
            current = current->next;

        if (!current) return; // Index out of bounds

        Node* newNode = new Node(val);
        newNode->next = current->next;
        newNode->prev = current;

        if (current->next)
            current->next->prev = newNode;
        else
            tail = newNode; // Update tail if at end

        current->next = newNode;
    }

    // Delete from front
    void deleteFront() {
        if (!head) return;
        Node* toDelete = head;
        head = head->next;

        if (head)
            head->prev = nullptr;
        else
            tail = nullptr; // List became empty

        delete toDelete;
    }

    // Delete from back
    void deleteBack() {
        if (!tail) return;
        Node* toDelete = tail;
        tail = tail->prev;

        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;

        delete toDelete;
    }

    // Delete from specific position
    void deleteAt(int index) {
        if (index < 0 || !head) return;

        if (index == 0) {
            deleteFront();
            return;
        }

        Node* current = head;
        for (int i = 0; current && i < index; ++i)
            current = current->next;

        if (!current) return;

        if (current->prev)
            current->prev->next = current->next;
        if (current->next)
            current->next->prev = current->prev;

        if (current == tail)
            tail = current->prev;

        delete current;
    }

    // Search for a value
    int search(int val) {
        Node* current = head;
        int index = 0;
        while (current) {
            if (current->data == val)
                return index;
            current = current->next;
            ++index;
        }
        return -1; // Not found
    }

    // Print list forward
    void printForward() const {
        Node* current = head;
        std::cout << "Forward: ";
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << "\n";
    }

    // Print list backward
    void printBackward() const {
        Node* current = tail;
        std::cout << "Backward: ";
        while (current) {
            std::cout << current->data << " ";
            current = current->prev;
        }
        std::cout << "\n";
    }
};

// Demonstration
int main() {
    DoublyLinkedList dll;

    dll.insertBack(10);
    dll.insertBack(20);
    dll.insertFront(5);
    dll.insertAt(1, 15); // List: 5 15 10 20

    dll.printForward();
    dll.printBackward();

    std::cout << "Index of 10: " << dll.search(10) << "\n";

    dll.deleteAt(1);     // Delete 15
    dll.deleteFront();   // Delete 5
    dll.deleteBack();    // Delete 20

    dll.printForward();  // Should print: 10

    return 0;
}
