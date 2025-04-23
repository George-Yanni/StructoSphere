#include <iostream>

// Definition of a node in the singly linked list
struct Node {
    int data;
    Node* next;

    // Constructor to easily create a new node
    Node(int value) : data(value), next(nullptr) {}
};

// Definition of the singly linked list class
class SinglyLinkedList {
private:
    Node* head; // Pointer to the head of the list

public:
    // Constructor to initialize an empty list
    SinglyLinkedList() : head(nullptr) {}

    // Destructor to clean up the list
    ~SinglyLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // Function to insert a new node at the beginning of the list
    void insertAtHead(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    // Function to insert a new node at the end of the list
    void insertAtTail(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Function to delete a node by value
    void deleteNode(int value) {
        if (head == nullptr) return;

        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }

        if (current->next == nullptr) return;

        Node* temp = current->next;
        current->next = current->next->next;
        delete temp;
    }

    // Function to display the list
    void display() const {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " -> ";
            temp = temp->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    // Function to search for a value in the list
    bool search(int value) const {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == value) return true;
            temp = temp->next;
        }
        return false;
    }
};

int main() {
    SinglyLinkedList list;

    list.insertAtHead(10);
    list.insertAtHead(20);
    list.insertAtTail(30);
    list.insertAtTail(40);

    std::cout << "Linked List: ";
    list.display();

    std::cout << "Search 20: " << (list.search(20) ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 50: " << (list.search(50) ? "Found" : "Not Found") << std::endl;

    list.deleteNode(20);
    std::cout << "After deleting 20: ";
    list.display();

    list.deleteNode(30);
    std::cout << "After deleting 30: ";
    list.display();

    return 0;
}
