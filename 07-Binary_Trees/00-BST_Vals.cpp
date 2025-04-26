#include <iostream>
#include <iomanip>

// Node structure for the Binary Search Tree
struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;
    
    // Private helper functions
    Node* insertHelper(Node* node, int value);
    Node* deleteHelper(Node* node, int value);
    Node* findMin(Node* node);
    void inOrderHelper(Node* node);
    void preOrderHelper(Node* node);
    void postOrderHelper(Node* node);
    Node* searchHelper(Node* node, int value);
    void destroyHelper(Node* node);
    int heightHelper(Node* node);
    bool isBalancedHelper(Node* node);
    void printTreeHelper(Node* node, int level, const std::string& prefix);
    
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroyHelper(root); }
    
    // Public interface
    void insert(int value);
    void remove(int value);
    bool search(int value);
    void inOrderTraversal();
    void preOrderTraversal();
    void postOrderTraversal();
    int getHeight();
    bool isBalanced();
    bool isEmpty() { return root == nullptr; }
    void printTree();
};

// Insert a new value in the BST
Node* BinarySearchTree::insertHelper(Node* node, int value) {
    if (node == nullptr) {
        return new Node(value);
    }
    
    if (value < node->data) {
        node->left = insertHelper(node->left, value);
    } else if (value > node->data) {
        node->right = insertHelper(node->right, value);
    }
    
    return node;
}

void BinarySearchTree::insert(int value) {
    root = insertHelper(root, value);
}

// Find minimum value node in a subtree
Node* BinarySearchTree::findMin(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete a value from the BST
Node* BinarySearchTree::deleteHelper(Node* node, int value) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (value < node->data) {
        node->left = deleteHelper(node->left, value);
    } else if (value > node->data) {
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

void BinarySearchTree::remove(int value) {
    root = deleteHelper(root, value);
}

// Search for a value in the BST
Node* BinarySearchTree::searchHelper(Node* node, int value) {
    if (node == nullptr || node->data == value) {
        return node;
    }
    
    if (value < node->data) {
        return searchHelper(node->left, value);
    }
    return searchHelper(node->right, value);
}

bool BinarySearchTree::search(int value) {
    return searchHelper(root, value) != nullptr;
}

// Tree traversals
void BinarySearchTree::inOrderHelper(Node* node) {
    if (node != nullptr) {
        inOrderHelper(node->left);
        std::cout << node->data << " ";
        inOrderHelper(node->right);
    }
}

void BinarySearchTree::inOrderTraversal() {
    inOrderHelper(root);
    std::cout << std::endl;
}

void BinarySearchTree::preOrderHelper(Node* node) {
    if (node != nullptr) {
        std::cout << node->data << " ";
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
        std::cout << node->data << " ";
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
    std::cout << node->data << std::endl;
    
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

// Main function to test the BST implementation
int main() {
    BinarySearchTree bst;
    
    // Insert some values
    bst.insert(50);
    printf("Inserted 50\n");
    bst.insert(30);
    printf("Inserted 30\n");
    bst.insert(70);
    printf("Inserted 70\n");
    bst.insert(20);
    printf("Inserted 20\n");
    bst.insert(40);
    printf("Inserted 40\n");
    bst.insert(60);
    printf("Inserted 60\n");
    bst.insert(80);
    printf("Inserted 80\n");
    
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
    std::cout << "Search 40: " << (bst.search(40) ? "Found" : "Not found") << std::endl;
    std::cout << "Search 90: " << (bst.search(90) ? "Found" : "Not found") << std::endl;
    
    // Test height and balance
    std::cout << "Tree height: " << bst.getHeight() << std::endl;
    std::cout << "Is balanced: " << (bst.isBalanced() ? "Yes" : "No") << std::endl;
    
    // Test deletion
    bst.remove(30);
    std::cout << "After deleting 30, tree structure:" << std::endl;
    bst.printTree();
    std::cout << "After deleting 30, in-order traversal: ";
    bst.inOrderTraversal();
    
    return 0;
}