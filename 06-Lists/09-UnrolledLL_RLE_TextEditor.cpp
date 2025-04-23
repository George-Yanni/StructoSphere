#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
/*
This implementation allows simulating a real-world text editor with multiple open documents, 
where each document’s text is efficiently managed using an unrolled linked list, 
and all changes and searches are isolated to specific buffers.
*/
const int BLOCK_SIZE = 16; // Number of characters per block

struct Node {
    int numElements;
    char elements[BLOCK_SIZE];
    Node* next;

    Node() : numElements(0), next(nullptr) {}
};

class UnrolledLinkedList {
private:
    Node* head;

public:
    UnrolledLinkedList() : head(nullptr) {}

    // Insert a character at the end
    void insert(char c) {
        if (!head) {
            head = new Node();
            head->elements[head->numElements++] = c;
            return;
        }

        Node* curr = head;
        while (curr->next) curr = curr->next;

        if (curr->numElements < BLOCK_SIZE) {
            curr->elements[curr->numElements++] = c;
        } else {
            Node* newNode = new Node();
            newNode->elements[0] = c;
            newNode->numElements = 1;
            curr->next = newNode;
        }
    }

    // Insert a string at the end
    void insertString(const string& str) {
        for (char c : str) {
            insert(c);
        }
    }

    // Remove a character
    void remove() {
        if (!head) return;

        Node* curr = head;
        while (curr->next) curr = curr->next;

        if (curr->numElements > 0) {
            curr->numElements--;
        } else {
            delete curr;
            head = nullptr;
        }
    }

    // Print the current text buffer
    void printBuffer() {
        Node* curr = head;
        while (curr) {
            for (int i = 0; i < curr->numElements; ++i) {
                cout << curr->elements[i];
            }
            curr = curr->next;
        }
    }

    // Search for a character in the buffer
    bool search(char c) {
        Node* curr = head;
        while (curr) {
            for (int i = 0; i < curr->numElements; ++i) {
                if (curr->elements[i] == c) {
                    return true;
                }
            }
            curr = curr->next;
        }
        return false;
    }
};

class TextEditor {
private:
    map<string, UnrolledLinkedList> buffers;

public:
    // Add a new buffer (new document)
    void createBuffer(const string& bufferName) {
        if (buffers.find(bufferName) != buffers.end()) {
            cout << "Buffer \"" << bufferName << "\" already exists!" << endl;
            return;
        }
        buffers[bufferName] = UnrolledLinkedList();
        cout << "Created new buffer: " << bufferName << endl;
    }

    // Insert text into a specific buffer
    void insertText(const string& bufferName, const string& text) {
        if (buffers.find(bufferName) == buffers.end()) {
            cout << "Buffer \"" << bufferName << "\" does not exist!" << endl;
            return;
        }

        cout << "Inserting text into buffer: " << bufferName << endl;
        buffers[bufferName].insertString(text);
        cout << "Updated Buffer (" << bufferName << "): ";
        buffers[bufferName].printBuffer();
        cout << endl;
    }

    // Remove the last character from a specific buffer
    void removeText(const string& bufferName) {
        if (buffers.find(bufferName) == buffers.end()) {
            cout << "Buffer \"" << bufferName << "\" does not exist!" << endl;
            return;
        }

        cout << "Removing last character from buffer: " << bufferName << endl;
        buffers[bufferName].remove();
        cout << "Updated Buffer (" << bufferName << "): ";
        buffers[bufferName].printBuffer();
        cout << endl;
    }

    // Search for a character in a specific buffer
    void searchInBuffer(const string& bufferName, char c) {
        if (buffers.find(bufferName) == buffers.end()) {
            cout << "Buffer \"" << bufferName << "\" does not exist!" << endl;
            return;
        }

        cout << "Searching for character '" << c << "' in buffer: " << bufferName << endl;
        bool found = buffers[bufferName].search(c);
        if (found) {
            cout << "Character '" << c << "' found in buffer: " << bufferName << endl;
        } else {
            cout << "Character '" << c << "' not found in buffer: " << bufferName << endl;
        }
    }

    // Print all buffers
    void printAllBuffers() {
        for (auto& buffer : buffers) {
            cout << "\nBuffer: " << buffer.first << " - ";
            buffer.second.printBuffer();
            cout << endl;
        }
    }
};

int main() {
    TextEditor editor;

    // Create multiple buffers (documents)
    editor.createBuffer("Document1");
    editor.createBuffer("Document2");

    // Insert text into different buffers
    editor.insertText("Document1", "Hello, ");
    editor.insertText("Document1", "world!");
    editor.insertText("Document2", "This is the second document.");
    editor.insertText("Document2", " Adding more text.");

    // Remove text from a buffer
    editor.removeText("Document1");

    // Search for characters in buffers
    editor.searchInBuffer("Document1", 'w');
    editor.searchInBuffer("Document2", 'z');

    // Print all buffers
    editor.printAllBuffers();

    return 0;
}
