#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <sstream>
#include <queue>
#include <algorithm>
#include <set> // Include the set header
#include <stdexcept> // For exceptions

// Define a structure for a node in the Fibonacci heap
struct FibonacciNode {
    int key;             // The key (priority) of the node
    FibonacciNode* parent;          // Pointer to the parent node
    FibonacciNode* child;           // Pointer to an arbitrary child node
    FibonacciNode* left;            // Pointer to the left sibling
    FibonacciNode* right;           // Pointer to the right sibling
    int degree;            // The number of children of this node
    bool mark;             // A boolean indicating if this node has lost a child
    void* data;            // A generic pointer to store any kind of data (optional)

    // Constructor for the FibonacciNode
    FibonacciNode(int k) : key(k), parent(nullptr), child(nullptr), left(this), right(this), degree(0), mark(false), data(nullptr) {} // Initialize left/right to self
};

// Class for the Fibonacci Heap
class FibonacciHeap {
private:
    FibonacciNode* minNode;         // Pointer to the node with the minimum key in the heap
    int nodeCount;           // The total number of nodes in the heap

    // Helper function to link two nodes (used in consolidate)
    // Makes y a child of x
    void linkNodes(FibonacciNode* y, FibonacciNode* x) {
        // Remove y from the root list
        y->left->right = y->right;
        y->right->left = y->left;

        // Make y a child of x
        y->parent = x;

        if (x->child == nullptr) {
            x->child = y;
            y->right = y; // y becomes the only child, points to itself
            y->left = y;
        } else {
            // Insert y into x's child list (doubly linked circular list)
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y; // Update the node originally to the right of x->child
            x->child->right = y;      // x->child's right pointer now points to y
        }
        x->degree++;
        y->mark = false; // When a node becomes a child, its mark is cleared
    }

    // Helper function to consolidate the root list after extractMin
    void consolidate() {
        if (minNode == nullptr) return;

        // Max degree is bounded by O(log n)
        // Need a size slightly larger due to potential degree increase during linking
        int maxDegree = static_cast<int>(floor(log2(nodeCount))) + 2; // +2 for safety margin
        std::vector<FibonacciNode*> degreeTable(maxDegree, nullptr);

        // Collect root nodes to iterate safely without modifying the list during iteration
        std::vector<FibonacciNode*> roots;
        FibonacciNode* current = minNode;
        if (current != nullptr) {
            do {
                roots.push_back(current);
                current = current->right;
            } while (current != minNode);
        }

        // Iterate through the collected root nodes
        for (FibonacciNode* node : roots) {
            // Ensure the node is actually part of the root list before processing
            // (It might have become a child during previous linking steps)
            if (node->parent != nullptr) continue;

            int d = node->degree;
            while (d < degreeTable.size() && degreeTable[d] != nullptr) {
                FibonacciNode* y = degreeTable[d];

                // Ensure node has the smaller key, swap if necessary
                if (node->key > y->key) {
                    std::swap(node, y);
                }

                // Make y a child of node
                linkNodes(y, node);

                // Clear the degree table entry and increment degree for the next check
                degreeTable[d] = nullptr;
                d++;
                 // Resize degreeTable if needed (should be rare with initial sizing)
                if (d >= degreeTable.size()) {
                    degreeTable.resize(d + 1, nullptr);
                }
            }
             // Ensure d is within bounds before assignment
            if (d < degreeTable.size()) {
                degreeTable[d] = node;
            } else {
                 // Handle resize case if d went out of bounds during while loop
                 degreeTable.resize(d + 1, nullptr);
                 degreeTable[d] = node;
            }
        }

        // Rebuild the root list from the degree table and find the new minimum
        minNode = nullptr;
        for (int i = 0; i < degreeTable.size(); ++i) {
            if (degreeTable[i] != nullptr) {
                FibonacciNode* rootNode = degreeTable[i];
                // Ensure node is detached from any potential parent (should be already)
                rootNode->parent = nullptr;
                // Ensure left/right point to self before adding to root list
                rootNode->left = rootNode;
                rootNode->right = rootNode;

                if (minNode == nullptr) {
                    minNode = rootNode;
                } else {
                    // Add rootNode to the root list (circular doubly linked)
                    rootNode->right = minNode->right;
                    rootNode->left = minNode;
                    minNode->right->left = rootNode;
                    minNode->right = rootNode;

                    // Update minNode if necessary
                    if (rootNode->key < minNode->key) {
                        minNode = rootNode;
                    }
                }
            }
        }
    }

    // Helper function to cut a node from its parent and add it to the root list
    void cut(FibonacciNode* node, FibonacciNode* parent) {
        // Remove node from the child list of parent
        if (node->right == node) { // node is the only child
            parent->child = nullptr;
        } else {
            node->left->right = node->right;
            node->right->left = node->left;
            if (parent->child == node) { // If node was the one pointed to by parent->child
                parent->child = node->right; // Point to another child
            }
        }
        parent->degree--;

        // Add node to the root list
        node->left = minNode;
        node->right = minNode->right;
        minNode->right->left = node;
        minNode->right = node;

        node->parent = nullptr;
        node->mark = false; // Node becomes a root, mark is cleared
    }

    // Helper function for cascading cut
    void cascadingCut(FibonacciNode* node) {
        FibonacciNode* parent = node->parent;
        if (parent != nullptr) {
            if (!node->mark) {
                // If node is not marked, mark it (it lost a child)
                node->mark = true;
            } else {
                // If node is marked, it means it already lost a child before.
                // Cut it from its parent and perform cascading cut on the parent.
                cut(node, parent);
                cascadingCut(parent);
            }
        }
    }

    // Helper function to find a node by key (robust traversal)
    FibonacciNode* findNode(int key) {
        if (minNode == nullptr) return nullptr;

        std::queue<FibonacciNode*> q;
        std::set<FibonacciNode*> visited;

        // Add all root nodes to the queue initially
        FibonacciNode* currentRoot = minNode;
        do {
            if (visited.find(currentRoot) == visited.end()) {
                 q.push(currentRoot);
                 visited.insert(currentRoot);
            } else {
                // Avoid infinite loop if root list is corrupted
                break;
            }
            currentRoot = currentRoot->right;
        } while (currentRoot != minNode);


        while (!q.empty()) {
            FibonacciNode* current = q.front();
            q.pop();

            if (current->key == key) {
                return current; // Found the node
            }

            // Add children to the queue
            FibonacciNode* child = current->child;
            if (child != nullptr) {
                FibonacciNode* startChild = child;
                do {
                    if (visited.find(child) == visited.end()) {
                        q.push(child);
                        visited.insert(child);
                    } else {
                        // Avoid infinite loop if child list is corrupted
                        break;
                    }
                    child = child->right;
                } while (child != startChild);
            }
        }

        return nullptr; // Node not found
    }

    // Helper function to print a single tree structure recursively
    void printSingleTree(FibonacciNode* node, int indent, std::ostream& out) const {
         if (node == nullptr) return;

         // Print the current node
         out << std::setw(indent) << "" << node->key;
         if (node->mark) {
             out << "(M)"; // Indicate marked nodes
         }
         if (node == minNode) {
             out << " (min)"; // Indicate the overall minimum node
         }
         out << std::endl;

         // Recursively print children
         FibonacciNode* child = node->child;
         if (child != nullptr) {
             FibonacciNode* start = child;
             do {
                 printSingleTree(child, indent + 4, out); // Increase indent for children
                 child = child->right;
             } while (child != start);
         }
    }


public:
    // Constructor for the Fibonacci Heap
    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    // Function to insert a new node with a given key
    void insert(int key) {
        FibonacciNode* newNode = new FibonacciNode(key);
        // newNode->left and newNode->right are already initialized to self

        if (minNode == nullptr) {
            minNode = newNode;
        } else {
            // Insert newNode into the root list (next to minNode)
            newNode->right = minNode->right;
            newNode->left = minNode;
            minNode->right->left = newNode;
            minNode->right = newNode;

            // Update minNode if the new node is smaller
            if (newNode->key < minNode->key) {
                minNode = newNode;
            }
        }
        nodeCount++;
    }

    // Function to get the minimum key
    int getMin() const {
        if (minNode == nullptr) {
            throw std::runtime_error("Heap is empty. Cannot get minimum.");
        }
        return minNode->key;
    }

    // Function to extract the minimum node
    int extractMin() {
        if (minNode == nullptr) {
             throw std::runtime_error("Heap is empty. Cannot extract minimum.");
        }
        FibonacciNode* extractedMin = minNode;
        int minValue = extractedMin->key;

        // 1. Add children of the minimum node to the root list
        if (extractedMin->child != nullptr) {
            FibonacciNode* child = extractedMin->child;
            FibonacciNode* startChild = child;
            do {
                FibonacciNode* nextChild = child->right; // Store next before modifying list

                // Remove child from its sibling list (which is the child list of extractedMin)
                child->left->right = child->right;
                child->right->left = child->left;

                // Add child to the root list
                child->left = minNode;
                child->right = minNode->right;
                minNode->right->left = child;
                minNode->right = child;

                child->parent = nullptr; // Child becomes a root

                child = nextChild;
            } while (child != startChild); // Use startChild to detect end of circular list
        }

        // 2. Remove extractedMin from the root list
        extractedMin->left->right = extractedMin->right;
        extractedMin->right->left = extractedMin->left;

        // 3. Update minNode and consolidate
        if (extractedMin == extractedMin->right) { // Heap becomes empty
            minNode = nullptr;
        } else {
            minNode = extractedMin->right; // Temporarily point minNode to another root
            consolidate();                 // Consolidate trees and find the new true minimum
        }

        nodeCount--;
        delete extractedMin; // Free the memory of the extracted node
        return minValue;
    }

    // Function to decrease the key of a node
    void decreaseKey(int oldKey, int newKey) {
        if (newKey > oldKey) { // Note: Allow newKey == oldKey, though it does nothing
             throw std::invalid_argument("New key must be smaller than or equal to the old key.");
        }

        FibonacciNode* targetNode = findNode(oldKey);

        if (targetNode == nullptr) {
            throw std::runtime_error("Node with the specified old key not found.");
        }

        // If new key is not actually smaller, do nothing
        if (newKey == oldKey) return;

        targetNode->key = newKey;
        FibonacciNode* parent = targetNode->parent;

        // If the heap property is violated (node < parent)
        if (parent != nullptr && targetNode->key < parent->key) {
            cut(targetNode, parent);    // Cut the node from its parent
            cascadingCut(parent);   // Perform cascading cuts upwards
        }

        // Update the overall minimum pointer if necessary
        if (targetNode->key < minNode->key) {
            minNode = targetNode;
        }
    }

    // Function to delete a node with a given key
    void deleteNode(int key) {
         if (minNode == nullptr) {
             throw std::runtime_error("Heap is empty. Cannot delete node.");
         }

         // To delete, we decrease the key to the minimum possible value
         // and then extract the minimum.
         try {
             // Use a value guaranteed to be smaller than any other key
             decreaseKey(key, std::numeric_limits<int>::min());

             // Now the node to be deleted is the minimum node
             extractMin();
         } catch (const std::runtime_error& e) {
             // Catch if decreaseKey couldn't find the node
             std::cerr << "Error during delete: " << e.what() << std::endl;
             // Re-throw or handle as appropriate
             throw;
         } catch (const std::invalid_argument& e) {
              // Should not happen if INT_MIN is used, but handle just in case
             std::cerr << "Error during delete (invalid key decrease): " << e.what() << std::endl;
             throw;
         }
    }

    // Function to print the Fibonacci heap (iterates through root list)
    void printHeap(std::ostream& out = std::cout) const {
        if (minNode == nullptr) {
            out << "Heap is empty." << std::endl;
            return;
        }
        out << "Fibonacci Heap (Root List):" << std::endl;
        FibonacciNode* current = minNode;
        std::set<FibonacciNode*> visitedRoots; // Avoid infinite loop

        do {
            if (visitedRoots.count(current)) {
                 out << "  (Detected cycle in root list, stopping print)" << std::endl;
                 break;
            }
            visitedRoots.insert(current);
            out << "--- Tree Rooted at " << current->key << " (Degree: " << current->degree << ") ---" << std::endl;
            printSingleTree(current, 2, out); // Print tree starting from this root
            current = current->right;
        } while (current != minNode);
         out << "----------------------------------" << std::endl;
    }

     // Function to get the number of nodes in the heap
    int size() const {
        return nodeCount;
    }

    // Function to check if the heap is empty
    bool empty() const {
        return minNode == nullptr; // More direct check
    }

    // Destructor to free memory (using robust traversal)
    ~FibonacciHeap() {
        if (minNode == nullptr) return;

        std::queue<FibonacciNode*> q;
        std::set<FibonacciNode*> visited;
        std::vector<FibonacciNode*> nodesToDelete; // Collect nodes first to avoid use-after-free

        // Add all root nodes to the queue initially
        FibonacciNode* currentRoot = minNode;
         do {
            if (visited.find(currentRoot) == visited.end()) {
                 q.push(currentRoot);
                 visited.insert(currentRoot);
            } else {
                // Break if cycle detected during initial root scan
                break;
            }
            currentRoot = currentRoot->right;
        } while (currentRoot != minNode);


        while (!q.empty()) {
            FibonacciNode* current = q.front();
            q.pop();
            nodesToDelete.push_back(current); // Add node to the delete list

            // Add children to the queue
            FibonacciNode* child = current->child;
            if (child != nullptr) {
                FibonacciNode* startChild = child;
                do {
                    if (visited.find(child) == visited.end()) {
                        q.push(child);
                        visited.insert(child);
                    } else {
                         // Break if cycle detected in child list traversal
                         break;
                    }
                    child = child->right;
                } while (child != startChild);
            }
        }

        // Now delete all collected nodes
        for (FibonacciNode* node : nodesToDelete) {
            delete node;
        }

        // Ensure state is clean after deletion
        minNode = nullptr;
        nodeCount = 0;
    }
};

// Example usage of the Fibonacci Heap
int main() {
    FibonacciHeap fh;

    // Insert some nodes and print after each insertion
    std::cout << "--- Inserting Nodes ---" << std::endl;
    int values[] = {5, 10, 2, 8, 1, 15, 3, 7};
    for (int val : values) {
        fh.insert(val);
        std::cout << "Inserted " << val << ", Current Heap:" << std::endl;
        fh.printHeap();
        std::cout << "Min: " << fh.getMin() << ", Size: " << fh.size() << std::endl << std::endl;
    }


    std::cout << "\n--- Extracting Minimum ---" << std::endl;
    try {
        int minVal = fh.extractMin();
        std::cout << "Extracted Min: " << minVal << std::endl;
        std::cout << "Heap after extracting min:" << std::endl;
        fh.printHeap();
        std::cout << "Min: " << fh.getMin() << ", Size: " << fh.size() << std::endl << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n--- Decreasing Key ---" << std::endl;
    try {
        fh.decreaseKey(8, 0); // Decrease 8 to 0
        std::cout << "Heap after decreasing key of 8 to 0:" << std::endl;
        fh.printHeap();
        std::cout << "Min: " << fh.getMin() << ", Size: " << fh.size() << std::endl << std::endl;
    } catch (const std::exception& e) {
         std::cerr << "Error decreasing key: " << e.what() << std::endl;
    }

     std::cout << "\n--- Deleting Node ---" << std::endl;
    try {
        fh.deleteNode(10); // Delete node with key 10
        std::cout << "Heap after deleting node with key 10:" << std::endl;
        fh.printHeap();
         std::cout << "Min: " << fh.getMin() << ", Size: " << fh.size() << std::endl << std::endl;
    } catch (const std::exception& e) {
         std::cerr << "Error deleting node: " << e.what() << std::endl;
    }


    // Check the size and if it is empty
    std::cout << "\n--- Final Checks ---" << std::endl;
    std::cout << "Size of the heap: " << fh.size() << std::endl;
    std::cout << "Is the heap empty? " << (fh.empty() ? "Yes" : "No") << std::endl;

    // Extract all remaining elements
    std::cout << "\n--- Extracting All Elements ---" << std::endl;
    std::cout << "Extracting: ";
    while (!fh.empty()) {
        try {
            std::cout << fh.extractMin() << " ";
        } catch (const std::runtime_error& e) {
             std::cerr << "Error extracting: " << e.what() << std::endl;
             break; // Stop if error occurs
        }
    }
    std::cout << std::endl;
    std::cout << "Heap after extracting all elements:" << std::endl;
    fh.printHeap();
    std::cout << "Size: " << fh.size() << std::endl;


    return 0;
}
