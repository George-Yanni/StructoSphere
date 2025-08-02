#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept> // Required for std::runtime_error
#include <iomanip>   // For better formatting

// Forward declaration is not strictly needed here anymore as Node is defined before its use
// within the class, but it doesn't hurt.
struct Node;

// Define the Node struct for the Pairing Heap.
// Removed the 'prev' pointer for a standard implementation.
struct Node {
    int data;
    Node* child; // Pointer to the leftmost child
    Node* next;  // Pointer to the next sibling (to the right)

    Node(int value) : data(value), child(nullptr), next(nullptr) {}
};

// Class for the Pairing Heap.
class PairingHeap {
private:
    Node* root;

    // Helper function to recursively destroy the heap nodes.
    void destroyRecursive(Node* node) {
        if (!node) {
            return;
        }
        // Recursively destroy siblings first, then children
        // This avoids deep recursion on the child pointers in skewed trees
        Node* currentSibling = node->next;
        while(currentSibling) {
            Node* nextSibling = currentSibling->next;
            destroyRecursive(currentSibling);
            currentSibling = nextSibling;
        }
        // Now destroy the children
        destroyRecursive(node->child);
        // Finally, delete the node itself
        delete node;
    }


    // Core merge operation: Merges h2 into h1, assuming h1->data <= h2->data.
    // Makes h2 the leftmost child of h1.
    Node* link(Node* h1, Node* h2) {
        // h2 becomes the new leftmost child of h1
        h2->next = h1->child; // h2's right sibling is h1's old first child
        h1->child = h2;       // h1's first child is now h2
        // Note: No need to manage h2's original children or siblings here,
        // they remain attached to h2.
        return h1;
    }

    // Merges two heaps/subtrees, returning the resulting root.
    // Handles null cases and ensures the smaller root becomes the parent.
    Node* merge(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        // Ensure h1 has the smaller root key
        if (h1->data <= h2->data) {
            return link(h1, h2);
        } else {
            return link(h2, h1);
        }
    }

    // Helper function to perform the two-pass merge required by deleteMin.
    Node* mergeSiblings(Node* firstSibling) {
        if (!firstSibling || !firstSibling->next) {
            return firstSibling; // Zero or one sibling requires no merging
        }

        // Two-pass merge:
        std::vector<Node*> siblings;
        Node* current = firstSibling;
        // Isolate siblings before merging
        while (current) {
            Node* next = current->next;
            current->next = nullptr; // Disconnect sibling pointer for merging
            siblings.push_back(current);
            current = next;
        }

        // Pass 1: Merge pairs from left to right
        std::vector<Node*> mergedPairs;
        mergedPairs.reserve(siblings.size() / 2 + 1); // Optimization: reserve space
        for (size_t i = 0; i + 1 < siblings.size(); i += 2) {
            mergedPairs.push_back(merge(siblings[i], siblings[i + 1]));
        }
        // Add the last odd sibling if it exists
        if (siblings.size() % 2 != 0) {
            mergedPairs.push_back(siblings.back());
        }

        // Handle case where there was only one pair or one initial sibling
        if (mergedPairs.empty()) {
             return nullptr; // Should not happen if initial check passed, but safe
        }
        if (mergedPairs.size() == 1) {
            return mergedPairs[0];
        }


        // Pass 2: Merge the resulting pairs from right to left
        Node* finalRoot = mergedPairs.back();
        for (int i = mergedPairs.size() - 2; i >= 0; --i) {
            finalRoot = merge(mergedPairs[i], finalRoot);
        }

        return finalRoot;
    }


    // Helper function to print the tree recursively.
    // *** Added const qualifier here ***
    void printTree(Node* node, int depth) const {
        if (node) {
            // Print current node
            std::cout << std::setw(depth * 4) << "" << node->data << std::endl;

            // Recursively print children (and their siblings)
            Node* child = node->child;
            while (child) {
                printTree(child, depth + 1);
                child = child->next; // Move to the next sibling
            }
        }
    }


public:
    // Constructor.
    PairingHeap() : root(nullptr) {}

    // Destructor to free memory.
    ~PairingHeap() {
        destroyRecursive(root);
        root = nullptr; // Good practice after deletion
    }

    // Rule of 3/5/0: If defining a destructor, consider copy/move operations.
    // For simplicity here, we disable copy/move.
    PairingHeap(const PairingHeap&) = delete;
    PairingHeap& operator=(const PairingHeap&) = delete;
    PairingHeap(PairingHeap&&) = delete;
    PairingHeap& operator=(PairingHeap&&) = delete;


    // Function to check if the heap is empty.
    bool isEmpty() const {
        return root == nullptr;
    }

    // Function to insert a new element into the Pairing Heap.
    void insert(int value) {
        Node* newNode = new Node(value);
        root = merge(root, newNode);
    }

    // Function to find the minimum element in the Pairing Heap.
    int findMin() const { // Added const qualifier
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");
        }
        return root->data;
    }

    // Function to delete the minimum element from the Pairing Heap.
    int deleteMin() {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");
        }

        int minVal = root->data;
        Node* oldRoot = root;
        Node* firstChild = root->child;

        // Important: Nullify root's child pointer *before* deleting oldRoot
        // to prevent mergeSiblings from accessing deleted memory via firstChild
        // if firstChild was the only child.
        root->child = nullptr; // Not strictly necessary if mergeSiblings handles null firstChild, but safer.

        delete oldRoot; // Delete the old root node
        root = nullptr; // Set root to null before merging children

        // Merge the children using the two-pass strategy
        root = mergeSiblings(firstChild);

        return minVal;
    }

    // Function to print the Pairing Heap in a tree-like structure.
    void printHeap() const { // Added const qualifier
        if (isEmpty()) {
            std::cout << "Heap is empty." << std::endl;
            return;
        }
        std::cout << "Heap Structure (Root is top, children indented):\n";
        printTree(root, 0); // Now calls the const version of printTree
        std::cout << "--------------------" << std::endl;
    }
};

int main() {
    PairingHeap ph;

    // Insert some elements.
    ph.insert(5);
    std::cout <<" Inserted 5:\n";
    ph.printHeap();
    ph.insert(2);
    std::cout <<" Inserted 2:\n";
    ph.printHeap();
    ph.insert(8);
    std::cout <<" Inserted 8:\n";
    ph.printHeap();
    ph.insert(1);
    std::cout <<" Inserted 1:\n";
    ph.printHeap();
    ph.insert(9);
    std::cout <<" Inserted 9:\n";
    ph.printHeap();
    ph.insert(3);
    std::cout<<" Inserted 3: \n";
    ph.printHeap();
    ph.insert(7);
    std::cout <<" Inserted 7:\n";
    ph.printHeap();
    ph.insert(4);
    std::cout <<" Inserted 4:\n";
    ph.printHeap();
    ph.insert(6);
    std::cout <<" Inserted 6:\n";
    ph.printHeap();

    std::cout << "Pairing Heap after insertions:\n";
    ph.printHeap();

    // Find the minimum element.
    try {
        std::cout << "Minimum element: " << ph.findMin() << std::endl;

        // Delete the minimum element.
        std::cout << "\nDeleting minimum: " << ph.deleteMin() << std::endl;
        std::cout << "Pairing Heap after deleting minimum:\n";
        ph.printHeap();

        // Delete the minimum element again.
        std::cout << "\nDeleting minimum: " << ph.deleteMin() << std::endl;
        std::cout << "Pairing Heap after deleting minimum again:\n";
        ph.printHeap();

        // Find the minimum element.
        std::cout << "\nMinimum element: " << ph.findMin() << std::endl;

         // Delete remaining elements
        std::cout << "\nDeleting remaining elements:\n";
        while (!ph.isEmpty()) {
             std::cout << "Deleting minimum: " << ph.deleteMin() << std::endl;
             ph.printHeap();
        }

        // Try finding min on empty heap
        // std::cout << ph.findMin() << std::endl; // Uncomment to test exception

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    return 0;
}
