#include <iostream>
#include <random>      // For modern C++ random number generation
#include <memory>      // for std::unique_ptr, std::make_unique
#include <algorithm>   // for std::max (though not used here)
#include <string>      // for std::string in printTree
#include <functional>  // for std::function (optional, for passing RNG)

// --- Random Number Generation Setup ---
// Use a modern C++ random number generator
std::mt19937 rng(std::random_device{}()); // Mersenne Twister engine seeded with random_device
std::uniform_int_distribution<int> dist(1, 1000); // Distribution for priorities (adjust range as needed)

struct Node {
    int key;
    int priority; // Nodes with higher priority are closer to the root
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    int size; // Size of the subtree rooted at this node

    // Constructor initializes key, assigns a random priority, and sets size to 1
    Node(int k) : key(k), priority(dist(rng)), size(1) {}

    // Update the size of this node based on its children's sizes
    void updateSize() {
        size = 1 + (left ? left->size : 0) + (right ? right->size : 0);
    }
};

// Helper function to get the size of a subtree (handles null pointers)
int getSize(const std::unique_ptr<Node>& node) {
    return node ? node->size : 0;
}

// --- Rotations ---
// Perform a right rotation at node y
// Returns the new root of the rotated subtree (x)
std::unique_ptr<Node> rightRotate(std::unique_ptr<Node> y) {
    // y->left becomes the new root (x)
    auto x = std::move(y->left);
    // x's right child becomes y's left child
    y->left = std::move(x->right);
    // y becomes x's right child
    x->right = std::move(y);

    // Update sizes (order matters: update children first)
    if (x->right) x->right->updateSize(); // Update y's size
    x->updateSize(); // Update x's size

    return x; // x is the new root
}

// Perform a left rotation at node x
// Returns the new root of the rotated subtree (y)
std::unique_ptr<Node> leftRotate(std::unique_ptr<Node> x) {
    // x->right becomes the new root (y)
    auto y = std::move(x->right);
    // y's left child becomes x's right child
    x->right = std::move(y->left);
    // x becomes y's left child
    y->left = std::move(x);

    // Update sizes (order matters: update children first)
    if (y->left) y->left->updateSize(); // Update x's size
    y->updateSize(); // Update y's size

    return y; // y is the new root
}

// --- Treap Operations ---

// Inserts a key into the treap rooted at 'root'
// Maintains BST property for keys and Max-Heap property for priorities.
// Returns the new root of the modified subtree.
std::unique_ptr<Node> insert(std::unique_ptr<Node> root, int key) {
    // Base case: If the tree is empty, create a new node
    if (!root) {
        return std::make_unique<Node>(key);
    }

    // Recursive insertion based on BST property
    if (key < root->key) {
        root->left = insert(std::move(root->left), key);
        // Check heap property: if left child has higher priority, rotate right
        if (root->left && root->left->priority > root->priority) {
            root = rightRotate(std::move(root));
        }
    } else { // key >= root->key (handle duplicates by inserting right, or modify if needed)
        root->right = insert(std::move(root->right), key);
        // Check heap property: if right child has higher priority, rotate left
        if (root->right && root->right->priority > root->priority) {
            root = leftRotate(std::move(root));
        }
    }

    // Update the size of the current node after modifications
    root->updateSize();
    return root; // Return the (potentially new) root of this subtree
}

// Searches for a key in the treap
// Returns a raw pointer to the node if found, otherwise nullptr
Node* search(const std::unique_ptr<Node>& root, int key) {
    if (!root || root->key == key) {
        return root.get(); // Return raw pointer, ownership remains with unique_ptr
    }
    if (key < root->key) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

// Deletes a key from the treap rooted at 'root'
// Maintains BST and Heap properties.
// Returns the new root of the modified subtree.
std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> root, int key) {
    if (!root) {
        return nullptr; // Key not found
    }

    if (key < root->key) {
        root->left = deleteNode(std::move(root->left), key);
    } else if (key > root->key) {
        root->right = deleteNode(std::move(root->right), key);
    } else {
        // Node to delete found
        // Case 1 & 2: Node has 0 or 1 child
        if (!root->left) {
            return std::move(root->right); // Replace root with right child
        }
        if (!root->right) {
            return std::move(root->left); // Replace root with left child
        }

        // Case 3: Node has two children
        // Rotate the child with higher priority up
        if (root->left->priority > root->right->priority) {
            root = rightRotate(std::move(root));
            // Recursively delete the key from the new right subtree
            root->right = deleteNode(std::move(root->right), key);
        } else {
            root = leftRotate(std::move(root));
            // Recursively delete the key from the new left subtree
            root->left = deleteNode(std::move(root->left), key);
        }
    }

    // If root still exists after potential deletion/rotation, update its size
    if (root) {
        root->updateSize();
    }
    return root; // Return the (potentially new) root of this subtree
}

// --- Utility Functions ---

// Prints the tree structure (in-order traversal, rotated 90 degrees counter-clockwise)
void printTree(const std::unique_ptr<Node>& root, int indent = 0) {
    if (!root) {
        return;
    }
    // Print right subtree first (appears higher on screen)
    printTree(root->right, indent + 4);

    // Print current node
    std::cout << std::string(indent, ' ') << root->key
              << "(" << root->priority << ") [Size: " << root->size << "]\n";

    // Print left subtree (appears lower on screen)
    printTree(root->left, indent + 4);
}

int main() {
    // Note: No need to call srand() when using <random>
    std::unique_ptr<Node> treapRoot = nullptr; // Start with an empty tree

    std::cout << "--- Treap Operations ---\n";

    treapRoot = insert(std::move(treapRoot), 30);
    std::cout << "\nInserted 30:\n";
    printTree(treapRoot);

    treapRoot = insert(std::move(treapRoot), 20);
    std::cout << "\nInserted 20:\n";
    printTree(treapRoot);

    treapRoot = insert(std::move(treapRoot), 40);
    std::cout << "\nInserted 40:\n";
    printTree(treapRoot);

    treapRoot = insert(std::move(treapRoot), 10);
    std::cout << "\nInserted 10:\n";
    printTree(treapRoot);

    treapRoot = insert(std::move(treapRoot), 5);
    std::cout << "\nInserted 5:\n";
    printTree(treapRoot);

    std::cout << "\n--- Final Treap Structure ---\n";
    printTree(treapRoot);

    int keyToSearch = 20;
    std::cout << "\n--- Searching for " << keyToSearch << " ---\n";
    Node* found = search(treapRoot, keyToSearch);
    if (found) {
        std::cout << "Found key " << found->key << " with priority " << found->priority << "\n";
    } else {
        std::cout << "Key " << keyToSearch << " not found.\n";
    }

    int keyToDelete = 10;
    std::cout << "\n--- Deleting " << keyToDelete << " ---\n";
    treapRoot = deleteNode(std::move(treapRoot), keyToDelete);
    std::cout << "Tree after deleting " << keyToDelete << ":\n";
    printTree(treapRoot);

    keyToDelete = 99; // Key not in tree
    std::cout << "\n--- Attempting to delete " << keyToDelete << " (not present) ---\n";
    treapRoot = deleteNode(std::move(treapRoot), keyToDelete);
    printTree(treapRoot);


    std::cout << "\n--- End of Operations ---\n";
    // No need to call clearTree(), unique_ptr handles cleanup automatically
    // when treapRoot goes out of scope here.

    return 0;
}
