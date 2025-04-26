// AVL Tree-based Gaming Leaderboard Management
#include <iostream>
#include <queue>
#include <algorithm>
#include <string>

using namespace std;

// Node structure for the AVL Tree
struct Node {
    int score;
    string playerName;
    Node* left;
    Node* right;
    int height;
    
    Node(int s, const string& name) : score(s), playerName(name), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree class
class AVLTree {
private:
    Node* root;
    
    int getHeight(Node* node);
    int getBalance(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* insertHelper(Node* node, int score, const string& playerName);
    Node* deleteHelper(Node* node, int score, const string& playerName);
    Node* findMin(Node* node);
    Node* searchHelper(Node* node, int score, const string& playerName);
    void printTreeHelper(Node* node, int space);
    void destroyHelper(Node* node);

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(); }
    
    void insert(int score, const string& playerName);
    void remove(int score, const string& playerName);
    bool search(int score, const string& playerName);
    void printTree();
    void destroy();
};

// Get height of a node
int AVLTree::getHeight(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

// Get balance factor of a node
int AVLTree::getBalance(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Right rotation
Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

// Left rotation
Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

// Insert a value into the AVL Tree
Node* AVLTree::insertHelper(Node* node, int score, const string& playerName) {
    if (node == nullptr) {
        return new Node(score, playerName);
    }
    
    if (score < node->score) {
        node->left = insertHelper(node->left, score, playerName);
    } else if (score > node->score) {
        node->right = insertHelper(node->right, score, playerName);
    } else {
        return node; // Duplicate values not allowed
    }
    
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    
    int balance = getBalance(node);
    
    // Left Left Case
    if (balance > 1 && score < node->left->score) {
        return rightRotate(node);
    }
    
    // Right Right Case
    if (balance < -1 && score > node->right->score) {
        return leftRotate(node);
    }
    
    // Left Right Case
    if (balance > 1 && score > node->left->score) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Right Left Case
    if (balance < -1 && score < node->right->score) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

void AVLTree::insert(int score, const string& playerName) {
    root = insertHelper(root, score, playerName);
}

// Find minimum value node in a subtree
Node* AVLTree::findMin(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete a value from the AVL Tree
Node* AVLTree::deleteHelper(Node* node, int score, const string& playerName) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (score < node->score) {
        node->left = deleteHelper(node->left, score, playerName);
    } else if (score > node->score) {
        node->right = deleteHelper(node->right, score, playerName);
    } else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        
        Node* minNode = findMin(node->right);
        node->score = minNode->score;
        node->playerName = minNode->playerName;
        node->right = deleteHelper(node->right, minNode->score, minNode->playerName);
    }
    
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    
    int balance = getBalance(node);
    
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rightRotate(node);
    }
    
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    if (balance < -1 && getBalance(node->right) <= 0) {
        return leftRotate(node);
    }
    
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

void AVLTree::remove(int score, const string& playerName) {
    root = deleteHelper(root, score, playerName);
}

// Search for a value in the AVL Tree
Node* AVLTree::searchHelper(Node* node, int score, const string& playerName) {
    if (node == nullptr || (node->score == score && node->playerName == playerName)) {
        return node;
    }
    
    if (score < node->score) {
        return searchHelper(node->left, score, playerName);
    }
    return searchHelper(node->right, score, playerName);
}

bool AVLTree::search(int score, const string& playerName) {
    return searchHelper(root, score, playerName) != nullptr;
}

// Function to print the tree in a structured way with scores
void AVLTree::printTreeHelper(Node* node, int space) {
    if (node == nullptr) {
        return;
    }
    
    // Increase distance between levels
    space += 10;
    
    // Print right subtree first
    printTreeHelper(node->right, space);
    
    // Print the current node after padding with spaces
    cout << endl;
    for (int i = 10; i < space; i++) {
        cout << " ";
    }
    cout << node->playerName << " (" << node->score << ")" << endl;
    
    // Print left subtree
    printTreeHelper(node->left, space);
}

void AVLTree::printTree() {
    printTreeHelper(root, 0);
}

// Destroy the tree (deallocate memory)
void AVLTree::destroyHelper(Node* node) {
    if (node != nullptr) {
        destroyHelper(node->left);
        destroyHelper(node->right);
        delete node;
    }
}

void AVLTree::destroy() {
    destroyHelper(root);
    root = nullptr;
}

// Main function to demonstrate the AVL Tree for Gaming Leaderboard Management
int main() {
    AVLTree leaderboard;

    // Insert some players with scores
    leaderboard.insert(1500, "Alice");
    leaderboard.insert(1800, "Bob");
    leaderboard.insert(1200, "Charlie");
    leaderboard.insert(2000, "Dave");
    leaderboard.insert(1700, "Eve");
    
    // Print the tree structure with scores
    cout << "Tree Shape of the Leaderboard (Player Names with Scores):" << endl;
    leaderboard.printTree();
    cout << endl;

    // Search for a player
    cout << "Search for Bob: " << (leaderboard.search(1800, "Bob") ? "Found" : "Not found") << endl;
    
    // Remove a player
    cout << "Removing Alice..." << endl;
    leaderboard.remove(1500, "Alice");
    
    // Print the updated tree structure with scores
    leaderboard.printTree();
    
    return 0;
}
