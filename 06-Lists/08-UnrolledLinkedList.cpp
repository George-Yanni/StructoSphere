#include <iostream>
#include <vector>
using namespace std;

const int BLOCK_SIZE = 4; // Max number of elements per node

struct Node {
    int numElements;
    int elements[BLOCK_SIZE];
    Node* next;

    Node() : numElements(0), next(nullptr) {}
};

class UnrolledLinkedList {
private:
    Node* head;

public:
    UnrolledLinkedList() : head(nullptr) {}

    void insert(int value) {
        cout << "Inserting: " << value << endl;
        if (!head) {
            head = new Node();
            head->elements[0] = value;
            head->numElements = 1;
            return;
        }

        Node* curr = head;
        while (curr->next) curr = curr->next;

        if (curr->numElements < BLOCK_SIZE) {
            curr->elements[curr->numElements++] = value;
        } else {
            Node* newNode = new Node();
            newNode->elements[0] = value;
            newNode->numElements = 1;
            curr->next = newNode;
        }
    }

    void printList() {
        cout << "Unrolled Linked List: \n";
        Node* curr = head;
        int nodeIndex = 0;
        while (curr) {
            cout << " Node " << nodeIndex++ << " (" << curr->numElements << " elements): ";
            for (int i = 0; i < curr->numElements; ++i)
                cout << curr->elements[i] << " ";
            cout << endl;
            curr = curr->next;
        }
    }

    void remove(int value) {
        cout << "Removing: " << value << endl;
        Node* curr = head;
        while (curr) {
            for (int i = 0; i < curr->numElements; ++i) {
                if (curr->elements[i] == value) {
                    for (int j = i; j < curr->numElements - 1; ++j) {
                        curr->elements[j] = curr->elements[j + 1];
                    }
                    curr->numElements--;
                    return;
                }
            }
            curr = curr->next;
        }
        cout << "Value not found.\n";
    }
};

int main() {
    UnrolledLinkedList list;
    list.insert(10);
    list.insert(20);
    list.insert(30);
    list.insert(40);
    list.insert(50);
    list.insert(60);
    list.printList();

    list.remove(30);
    list.printList();

    list.remove(50);
    list.printList();

    return 0;
}
