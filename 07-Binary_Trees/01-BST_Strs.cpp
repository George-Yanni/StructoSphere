#include <iostream>
#include <iomanip>
#include <string> // Added for std::string

// Node structure for the Binary Search Tree
struct Node {
    std::string data; // Changed from int to std::string for names
    Node* left;
    Node* right;
    
    Node(const std::string& value) : data(value), left(nullptr), right(nullptr) {} // Updated constructor
};

class BinarySearchTree {
private:
    Node* root;
    
    // Private helper functions
    Node* insertHelper(Node* node, const std::string& value); // Updated parameter type
    Node* deleteHelper(Node* node, const std::string& value); // Updated parameter type
    Node* findMin(Node* node);
    void inOrderHelper(Node* node);
    void preOrderHelper(Node* node);
    void postOrderHelper(Node* node);
    Node* searchHelper(Node* node, const std::string& value); // Updated parameter type
    void destroyHelper(Node* node);
    int heightHelper(Node* node);
    bool isBalancedHelper(Node* node);
    void printTreeHelper(Node* node, int level, const std::string& prefix);
    
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroyHelper(root); }
    
    // Public interface
    void insert(const std::string& value); // Updated parameter type
    void remove(const std::string& value); // Updated parameter type
    bool search(const std::string& value); // Updated parameter type
    void inOrderTraversal();
    void preOrderTraversal();
    void postOrderTraversal();
    int getHeight();
    bool isBalanced();
    bool isEmpty() { return root == nullptr; }
    void printTree();
};

// Insert a new name in the BST
Node* BinarySearchTree::insertHelper(Node* node, const std::string& value) {
    if (node == nullptr) {
        return new Node(value);
    }
    
    if (value < node->data) { // String comparison (lexicographical)
        node->left = insertHelper(node->left, value);
    } else if (value > node->data) { // String comparison (lexicographical)
        node->right = insertHelper(node->right, value);
    }
    
    return node;
}

void BinarySearchTree::insert(const std::string& value) {
    root = insertHelper(root, value);
}

// Find minimum name node in a subtree
Node* BinarySearchTree::findMin(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete a name from the BST
Node* BinarySearchTree::deleteHelper(Node* node, const std::string& value) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (value < node->data) { // String comparison
        node->left = deleteHelper(node->left, value);
    } else if (value > node->data) { // String comparison
        node->right = deleteHelper(node->right, value);
    } else {
        // Node with only one child or no child
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        
        // Node with two children
        Node* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteHelper(node->right, temp->data);
    }
    return node;
}

void BinarySearchTree::remove(const std::string& value) {
    root = deleteHelper(root, value);
}

// Search for a name in the BST
Node* BinarySearchTree::searchHelper(Node* node, const std::string& value) {
    if (node == nullptr || node->data == value) {
        return node;
    }
    
    if (value < node->data) { // String comparison
        return searchHelper(node->left, value);
    }
    return searchHelper(node->right, value);
}

bool BinarySearchTree::search(const std::string& value) {
    return searchHelper(root, value) != nullptr;
}

// Tree traversals
void BinarySearchTree::inOrderHelper(Node* node) {
    if (node != nullptr) {
        inOrderHelper(node->left);
        std::cout << node->data << " "; // Outputs string
        inOrderHelper(node->right);
    }
}

void BinarySearchTree::inOrderTraversal() {
    inOrderHelper(root);
    std::cout << std::endl;
}

void BinarySearchTree::preOrderHelper(Node* node) {
    if (node != nullptr) {
        std::cout << node->data << " "; // Outputs string
        preOrderHelper(node->left);
        preOrderHelper(node->right);
    }
}

void BinarySearchTree::preOrderTraversal() {
    preOrderHelper(root);
    std::cout << std::endl;
}

void BinarySearchTree::postOrderHelper(Node* node) {
    if (node != nullptr) {
        postOrderHelper(node->left);
        postOrderHelper(node->right);
        std::cout << node->data << " "; // Outputs string
    }
}

void BinarySearchTree::postOrderTraversal() {
    postOrderHelper(root);
    std::cout << std::endl;
}

// Calculate height of the tree
int BinarySearchTree::heightHelper(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);
    return std::max(leftHeight, rightHeight) + 1;
}

int BinarySearchTree::getHeight() {
    return heightHelper(root);
}

// Check if tree is balanced
bool BinarySearchTree::isBalancedHelper(Node* node) {
    if (node == nullptr) {
        return true;
    }
    
    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);
    
    if (abs(leftHeight - rightHeight) <= 1 &&
        isBalancedHelper(node->left) &&
        isBalancedHelper(node->right)) {
        return true;
    }
    return false;
}

bool BinarySearchTree::isBalanced() {
    return isBalancedHelper(root);
}

// Print tree in a tree-like structure
void BinarySearchTree::printTreeHelper(Node* node, int level, const std::string& prefix) {
    if (node == nullptr) {
        return;
    }
    
    // Print right subtree first (so it appears on top)
    printTreeHelper(node->right, level + 1, "    ");
    
    // Print current node
    std::cout << std::setw(level * 4) << prefix;
    std::cout << node->data << std::endl; // Outputs string
    
    // Print left subtree
    printTreeHelper(node->left, level + 1, "    ");
}

void BinarySearchTree::printTree() {
    if (root == nullptr) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }
    printTreeHelper(root, 0, "");
}

// Clean up the tree
void BinarySearchTree::destroyHelper(Node* node) {
    if (node != nullptr) {
        destroyHelper(node->left);
        destroyHelper(node->right);
        delete node;
    }
}

// Main function to test the BST implementation with names
int main() {
    BinarySearchTree bst;
    
    // Insert some names
    bst.insert("Emma");
    bst.insert("Alice");
    bst.insert("John");
    bst.insert("Bob");
    bst.insert("Diana");
    bst.insert("Frank");
    bst.insert("Hannah");
    
    // Test tree printing
    std::cout << "Tree structure:" << std::endl;
    bst.printTree();
    std::cout << std::endl;
    
    // Test traversals
    std::cout << "In-order traversal: ";
    bst.inOrderTraversal();
    
    std::cout << "Pre-order traversal: ";
    bst.preOrderTraversal();
    
    std::cout << "Post-order traversal: ";
    bst.postOrderTraversal();
    
    // Test search
    std::cout << "Search Diana: " << (bst.search("Diana") ? "Found" : "Not found") << std::endl;
    std::cout << "Search Zoe: " << (bst.search("Zoe") ? "Found" : "Not found") << std::endl;
    
    // Test height and balance
    std::cout << "Tree height: " << bst.getHeight() << std::endl;
    std::cout << "Is balanced: " << (bst.isBalanced() ? "Yes" : "No") << std::endl;
    
    // Test deletion
    bst.remove("Alice");
    std::cout << "After deleting Alice, tree structure:" << std::endl;
    bst.printTree();
    std::cout << "After deleting Alice, in-order traversal: ";
    bst.inOrderTraversal();
    
    return 0;
}