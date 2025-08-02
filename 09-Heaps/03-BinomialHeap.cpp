#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>     // For level-order traversal (printing, destructor, findNode)
#include <stdexcept> // For exceptions
#include <limits>    // For numeric_limits in deleteNode
#include <string>    // For printing prefixes

// Forward declaration
template <typename T>
class BinomialHeap;

// Structure for a node in a Binomial Tree
template <typename T>
struct BinomialNode {
    T key;
    int degree;
    BinomialNode<T>* parent;
    BinomialNode<T>* child;   // Pointer to the leftmost child
    BinomialNode<T>* sibling; // Pointer to the right sibling

    BinomialNode(T val) : key(val), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}

    // Prevent copying of nodes to avoid complex ownership issues
    BinomialNode(const BinomialNode&) = delete;
    BinomialNode& operator=(const BinomialNode&) = delete;
};

// Class for the Binomial Heap
template <typename T>
class BinomialHeap {
private:
    BinomialNode<T>* head; // Pointer to the head of the root list

    // Links two binomial trees of the same degree
    // Makes y the child of x (assuming x.key <= y.key)
    void link(BinomialNode<T>* y, BinomialNode<T>* x) {
        y->parent = x;
        y->sibling = x->child;
        x->child = y;
        x->degree++;
    }

    // Merges the root lists of two heaps, maintaining sorted order by degree
    BinomialNode<T>* mergeRootLists(BinomialNode<T>* h1, BinomialNode<T>* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        BinomialNode<T>* mergedHead = nullptr;
        BinomialNode<T>* currentTail = nullptr;
        BinomialNode<T>** nextNodePtr = &mergedHead; // Pointer to the pointer to fill next

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *nextNodePtr = h1;
                h1 = h1->sibling;
            } else {
                *nextNodePtr = h2;
                h2 = h2->sibling;
            }
            currentTail = *nextNodePtr;
            nextNodePtr = &(currentTail->sibling);
        }

        // Append remaining nodes from either list
        *nextNodePtr = h1 ? h1 : h2;

        return mergedHead;
    }


    // Performs the union of two binomial heaps (represented by their head nodes)
    BinomialNode<T>* unionHeaps(BinomialNode<T>* h1, BinomialNode<T>* h2) {
        BinomialNode<T>* H = mergeRootLists(h1, h2);
        if (!H) return nullptr;

        BinomialNode<T>* prev_x = nullptr;
        BinomialNode<T>* x = H;
        BinomialNode<T>* next_x = x->sibling;

        while (next_x) {
            // Case 1 & 2: Degrees differ, or three consecutive roots have the same degree.
            // Move pointers forward without merging x and next_x.
            if ((x->degree != next_x->degree) || (next_x->sibling && next_x->sibling->degree == x->degree)) {
                prev_x = x;
                x = next_x;
            }
            // Case 3: x and next_x have same degree, x.key <= next_x.key. Merge next_x into x.
            else if (x->key <= next_x->key) {
                x->sibling = next_x->sibling; // Remove next_x from root list
                link(next_x, x);              // Make next_x child of x
                // x remains the same, prev_x remains the same
            }
            // Case 4: x and next_x have same degree, x.key > next_x.key. Merge x into next_x.
            else {
                // Remove x from root list
                if (prev_x) {
                    prev_x->sibling = next_x;
                } else {
                    H = next_x; // next_x becomes the new head
                }
                link(x, next_x); // Make x child of next_x
                x = next_x;      // Continue check from the merged tree (now next_x)
            }
            // Advance next_x for the next iteration
            next_x = x->sibling;
        }
        return H;
    }

    // Reverses the sibling list of children of a node and nullifies their parent pointers.
    // Returns the head of the reversed list (which becomes a root list).
    BinomialNode<T>* reverseChildren(BinomialNode<T>* node) {
        if (!node || !node->child) return nullptr;

        BinomialNode<T>* current = node->child;
        BinomialNode<T>* prev = nullptr;
        BinomialNode<T>* next = nullptr;

        while (current) {
            next = current->sibling;
            current->sibling = prev;
            current->parent = nullptr; // Children become roots of a new heap
            prev = current;
            current = next;
        }
        node->child = nullptr; // Original node no longer has children in this list
        return prev; // Prev is the new head of the reversed list
    }

    // Helper function for printing a single Binomial Tree recursively (ASCII version)
    void printTree(BinomialNode<T>* root, const std::string& prefix, bool isLastChild) const {
        if (!root) return;

        // Print current node
        std::cout << prefix;
        // Use ASCII characters for connectors
        std::cout << (isLastChild ? "\\-- " : "+-- "); // Replaced └── and ├──
        std::cout << root->key << " (deg " << root->degree << ")" << std::endl;

        // Prepare prefix for children
        // Use ASCII character for vertical line
        std::string childPrefix = prefix + (isLastChild ? "    " : "|   "); // Replaced │

        // Collect children to identify the last one for correct prefix
        std::vector<BinomialNode<T>*> children;
        BinomialNode<T>* currentChild = root->child;
        while(currentChild) {
            children.push_back(currentChild);
            currentChild = currentChild->sibling;
        }

        // Print children recursively
        for (size_t i = 0; i < children.size(); ++i) {
            printTree(children[i], childPrefix, i == children.size() - 1);
        }
    }

    // Helper function to delete all nodes in a subtree rooted at 'node'
    void deleteSubtree(BinomialNode<T>* node) {
        if (!node) return;

        // Use a queue for BFS traversal to delete nodes
        std::queue<BinomialNode<T>*> q;
        q.push(node);

        while (!q.empty()) {
            BinomialNode<T>* current = q.front();
            q.pop();

            // Add children to the queue
            BinomialNode<T>* child = current->child;
            while (child) {
                q.push(child);
                child = child->sibling;
            }

            // Delete the current node after processing its children
            // std::cout << "Deleting node " << current->key << std::endl; // Optional debug print
            delete current;
        }
    }


public:
    // Constructor
    BinomialHeap() : head(nullptr) {}

    // Destructor - Cleans up all allocated nodes
    ~BinomialHeap() {
        BinomialNode<T>* currentRoot = head;
        while (currentRoot) {
            BinomialNode<T>* nextRoot = currentRoot->sibling; // Get next root before deleting current tree
            deleteSubtree(currentRoot); // Delete the entire tree rooted at currentRoot
            currentRoot = nextRoot;
        }
        head = nullptr; // Ensure head is null after cleanup
    }

    // Prevent copying and assignment of the heap itself
    BinomialHeap(const BinomialHeap&) = delete;
    BinomialHeap& operator=(const BinomialHeap&) = delete;


    // Inserts a new element into the heap
    void insert(T val) {
        BinomialNode<T>* newNode = new BinomialNode<T>(val);
        // Create a temporary heap structure conceptually (just the node as head)
        BinomialNode<T>* tempHead = newNode;
        head = unionHeaps(head, tempHead);
        // No temporary BinomialHeap object to delete
    }

    // Finds the minimum element in the heap (returns key)
    T findMin() const {
        if (!head) {
            throw std::runtime_error("Heap is empty. Cannot find minimum.");
        }
        BinomialNode<T>* minNode = head;
        BinomialNode<T>* current = head->sibling;
        while (current) {
            if (current->key < minNode->key) {
                minNode = current;
            }
            current = current->sibling;
        }
        return minNode->key;
    }

    // Extracts the minimum element from the heap (returns key)
    T extractMin() {
        if (!head) {
            throw std::runtime_error("Heap is empty. Cannot extract minimum.");
        }

        // Find the root with the minimum key and its predecessor
        BinomialNode<T>* minNode = head;
        BinomialNode<T>* prevMinNode = nullptr;
        BinomialNode<T>* current = head->sibling;
        BinomialNode<T>* prevCurrent = head;

        while (current) {
            if (current->key < minNode->key) {
                minNode = current;
                prevMinNode = prevCurrent;
            }
            prevCurrent = current;
            current = current->sibling;
        }

        // Remove the minimum node from the root list
        if (prevMinNode) {
            prevMinNode->sibling = minNode->sibling;
        } else {
            head = minNode->sibling; // Min node was the head
        }

        // Reverse the children of the minimum node to form a new heap's root list
        BinomialNode<T>* childrenHead = reverseChildren(minNode);

        // Store the key and delete the extracted node
        T minKey = minNode->key;
        delete minNode; // Delete the node itself
        minNode = nullptr; // Avoid dangling pointer

        // Union the remaining heap with the new heap formed by children
        head = unionHeaps(head, childrenHead);

        return minKey;
    }

    // Decreases the key of a given node.
    // Assumes 'node' is a valid pointer to a node within this heap.
    void decreaseKey(BinomialNode<T>* node, T new_key) {
        if (!node) {
             throw std::invalid_argument("Node pointer is null.");
        }
        if (new_key > node->key) {
            throw std::invalid_argument("New key is greater than current key.");
        }
        if (new_key == node->key) {
            return; // No change needed
        }

        node->key = new_key;
        BinomialNode<T>* y = node;
        BinomialNode<T>* z = y->parent;

        // Bubble up the node if min-heap property is violated
        while (z && y->key < z->key) {
            // Swap keys between child y and parent z
            // Note: If nodes had significant satellite data, swapping data
            // or restructuring pointers might be preferred over just swapping keys.
            std::swap(y->key, z->key);

            // Move up the tree
            y = z;
            z = y->parent;
        }
    }

    // Deletes a node from the heap.
    // Assumes 'node' is a valid pointer to a node within this heap.
    // Requires T to support std::numeric_limits<T>::lowest() or similar concept.
    void deleteNode(BinomialNode<T>* node) {
        if (!node) {
            throw std::invalid_argument("Node pointer is null.");
        }
        // Decrease key to a value guaranteed to be the minimum.
        // This assumes T has a meaningful 'lowest' value.
        // Consider alternatives if T doesn't fit this model well.
        T minPossibleKey = std::numeric_limits<T>::lowest();
        if (node->key == minPossibleKey) {
             // If the key is already lowest, we might have issues if multiple
             // nodes have this key. ExtractMin will just get one of them.
             // This edge case is tricky. For simplicity, we proceed,
             // but in a robust system, this might need careful handling.
        }
        decreaseKey(node, minPossibleKey);
        extractMin(); // The node is now the minimum and will be extracted.
    }

    // Merges another binomial heap into this one.
    // The other heap becomes empty after the merge.
    void merge(BinomialHeap<T>& otherHeap) {
        if (this == &otherHeap) return; // Prevent merging with self

        head = unionHeaps(head, otherHeap.head);
        otherHeap.head = nullptr; // Prevent otherHeap's destructor from deleting nodes now owned by this heap.
    }

    // Checks if the heap is empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Prints the heap structure (roots and their trees)
    void printHeap() const {
        std::cout << "--------------------" << std::endl;
        std::cout << "Binomial Heap Status:" << std::endl;
        if (!head) {
            std::cout << "  Heap is empty." << std::endl;
             std::cout << "--------------------" << std::endl;
            return;
        }

        BinomialNode<T>* current = head;
        int treeCount = 0;
        while (current) {
            treeCount++;
            std::cout << "\nTree " << treeCount << " (Root Degree " << current->degree << "):" << std::endl;
            printTree(current, "  ", true); // Start printing this tree
            current = current->sibling;
        }
         std::cout << "--------------------" << std::endl;
    }

    // Helper function to find a node with a specific key (for testing decreaseKey/delete)
    // WARNING: This is O(n) and inefficient for large heaps. Use only for testing
    // or if you don't have direct pointers to nodes.
    BinomialNode<T>* findNode(T key) {
        if (!head) return nullptr;

        std::queue<BinomialNode<T>*> q;
        BinomialNode<T>* currentRoot = head;
        while(currentRoot) {
            q.push(currentRoot);
            currentRoot = currentRoot->sibling;
        }

        while(!q.empty()) {
            BinomialNode<T>* currentNode = q.front();
            q.pop();

            if (currentNode->key == key) {
                return currentNode; // Found the node
            }

            // Add children to the queue for searching
            BinomialNode<T>* currentChild = currentNode->child;
            while(currentChild) {
                q.push(currentChild);
                currentChild = currentChild->sibling;
            }
        }
        return nullptr; // Node not found
    }
};

// Example Usage
int main() {
    BinomialHeap<int> bh;

    bh.insert(10);
    bh.insert(20);
    bh.insert(30);
    bh.insert(5);
    bh.insert(15);
    bh.insert(25);
    bh.insert(3);
    bh.insert(7);
    bh.insert(12);
    bh.insert(18);

    bh.printHeap();

    std::cout << "Minimum element: " << bh.findMin() << std::endl;

    std::cout << "\nExtracting minimum: " << bh.extractMin() << std::endl;
    bh.printHeap();

    std::cout << "\nExtracting minimum: " << bh.extractMin() << std::endl;
    bh.printHeap();

    // Example of decreaseKey (requires finding the node first)
    BinomialNode<int>* nodeToDecrease = bh.findNode(20); // Find a node with key 20
    if (nodeToDecrease) {
        std::cout << "\nDecreasing key of node 20 to 1" << std::endl;
        try {
            bh.decreaseKey(nodeToDecrease, 1);
            bh.printHeap();
            std::cout << "Minimum element after decreaseKey: " << bh.findMin() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error decreasing key: " << e.what() << std::endl;
        }
    } else {
        std::cout << "\nNode with key 20 not found for decreaseKey." << std::endl;
    }

    // Example of delete (requires finding the node first)
    BinomialNode<int>* nodeToDelete = bh.findNode(15); // Find a node with key 15
     if (nodeToDelete) {
        std::cout << "\nDeleting node with key 15" << std::endl;
         try {
            bh.deleteNode(nodeToDelete);
            bh.printHeap();
         } catch (const std::exception& e) {
            std::cerr << "Error deleting node: " << e.what() << std::endl;
         }
    } else {
        std::cout << "\nNode with key 15 not found for deleteNode." << std::endl;
    }


    BinomialHeap<int> bh2;
    bh2.insert(1);
    bh2.insert(4);
    bh2.insert(8);

    std::cout << "\nSecond Heap:" << std::endl;
    bh2.printHeap();

    std::cout << "\nMerging heaps..." << std::endl;
    bh.merge(bh2);
    bh.printHeap();

    std::cout << "\nSecond heap after merge (should be empty):" << std::endl;
    bh2.printHeap(); // Should be empty

    std::cout << "\nMain heap operations finished. Destructor will now clean up." << std::endl;

    return 0; // bh and bh2 destructors called here
}
