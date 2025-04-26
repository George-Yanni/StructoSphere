#include <iostream>
#include <iomanip>

// Note That:
// In a splay tree, after every access operation (which includes insertion, search, and deletion),
// the accessed element is moved to the root of the tree through a series of rotations. This process
// is known as "splaying." The primary purpose of splaying is to improve the efficiency of future 
// operations by bringing frequently accessed elements closer to the root.


using namespace std;

// Node structure
struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent;

    Node(int k) : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Splay Tree class
class SplayTree {
private:
    Node* root;

    // Rotations
    void leftRotate(Node* x);
    void rightRotate(Node* x);

    // Splaying
    void splay(Node* x);

    // Internal utilities
    Node* subtreeMin(Node* node) const;
    Node* subtreeMax(Node* node) const;
    Node* findNode(int key) const;
    void replace(Node* u, Node* v);
    void clearHelper(Node* node);

    // Print utility
    void printTreeHelper(Node* node, int space) const;

public:
    SplayTree();
    ~SplayTree();
    void insert(int key);
    void remove(int key);
    bool search(int key);
    void printTree() const;
    void clear();
};

// Constructor
SplayTree::SplayTree() : root(nullptr) {}

// Destructor
SplayTree::~SplayTree() {
    clear();
}

// Left rotation
void SplayTree::leftRotate(Node* x) {
    Node* y = x->right;
    if (!y) return;

    x->right = y->left;
    if (y->left) y->left->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Right rotation
void SplayTree::rightRotate(Node* x) {
    Node* y = x->left;
    if (!y) return;

    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

// Splay function
void SplayTree::splay(Node* x) {
    while (x->parent) {
        if (!x->parent->parent) {
            // Zig step
            if (x == x->parent->left) {
                rightRotate(x->parent);
            } else {
                leftRotate(x->parent);
            }
        } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
            // Zig-zig left
            rightRotate(x->parent->parent);
            rightRotate(x->parent);
        } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
            // Zig-zig right
            leftRotate(x->parent->parent);
            leftRotate(x->parent);
        } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
            // Zig-zag left-right
            leftRotate(x->parent);
            rightRotate(x->parent);
        } else {
            // Zig-zag right-left
            rightRotate(x->parent);
            leftRotate(x->parent);
        }
    }
}

// Insert key
void SplayTree::insert(int key) {
    Node* z = root;
    Node* p = nullptr;

    while (z) {
        p = z;
        if (key < z->key) {
            z = z->left;
        } else if (key > z->key) {
            z = z->right;
        } else {
            // Key already exists, splay it to root
            splay(z);
            return;
        }
    }

    z = new Node(key);
    z->parent = p;

    if (!p) {
        root = z;
    } else if (key < p->key) {
        p->left = z;
    } else {
        p->right = z;
    }

    splay(z);
}

// Search for a key
bool SplayTree::search(int key) {
    Node* node = findNode(key);
    if (node) {
        splay(node);
        return true;
    }
    return false;
}

// Find node with given key
Node* SplayTree::findNode(int key) const {
    Node* x = root;
    while (x) {
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            return x;
        }
    }
    return nullptr;
}

// Remove node with given key
void SplayTree::remove(int key) {
    Node* node = findNode(key);
    if (!node || node->key != key) return;

    splay(node); // Splay the node to remove to root

    if (!node->left) {
        // No left child, replace with right child
        replace(node, node->right);
        if (node->right) {
            splay(node->right);
        }
    } else if (!node->right) {
        // No right child, replace with left child
        replace(node, node->left);
        if (node->left) {
            splay(node->left);
        }
    } else {
        // Node has both children
        Node* min = subtreeMin(node->right);
        if (min->parent != node) {
            replace(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }
        replace(node, min);
        min->left = node->left;
        min->left->parent = min;
        splay(min);
    }

    delete node;
}

// Replace node u with node v
void SplayTree::replace(Node* u, Node* v) {
    if (!u->parent) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v) {
        v->parent = u->parent;
    }
}

// Find minimum node in subtree
Node* SplayTree::subtreeMin(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// Find maximum node in subtree
Node* SplayTree::subtreeMax(Node* node) const {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

// Print the tree in a readable shape
void SplayTree::printTreeHelper(Node* node, int space) const {
    const int COUNT = 10;
    if (!node) return;

    space += COUNT;
    printTreeHelper(node->right, space);

    cout << endl;
    for (int i = COUNT; i < space; i++) {
        cout << " ";
    }
    cout << node->key << endl;

    printTreeHelper(node->left, space);
}

void SplayTree::printTree() const {
    printTreeHelper(root, 0);
    cout << endl;
}

// Clear the entire tree
void SplayTree::clearHelper(Node* node) {
    if (node) {
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
}

void SplayTree::clear() {
    clearHelper(root);
    root = nullptr;
}

// Main demo
int main() {
    SplayTree tree;

    cout << "Inserting elements..." << endl;
    tree.insert(30);
    cout << "After inserting 30:" << endl;
    tree.printTree();

    tree.insert(40);
    cout << "After inserting 40:" << endl;
    tree.printTree();

    tree.insert(20);
    cout << "After inserting 20:" << endl;
    tree.printTree();

    tree.insert(10);
    cout << "After inserting 10:" << endl;
    tree.printTree();

    tree.insert(25);
    cout << "After inserting 25:" << endl;
    tree.printTree();

    tree.insert(50);
    cout << "After inserting 50:" << endl;
    tree.printTree();

    cout << "Searching for 25..." << endl;
    if (tree.search(25)) {
        cout << "Found 25. Tree after splaying:" << endl;
        tree.printTree();
    } else {
        cout << "25 not found." << endl;
    }

    cout << "Searching for 10..." << endl;
    if (tree.search(10)) {
        cout << "Found 10. Tree after splaying:" << endl;
        tree.printTree();
    } else {
        cout << "10 not found." << endl;
    }

    cout << "Removing 40..." << endl;
    tree.remove(40);
    tree.printTree();

    cout << "Removing 20..." << endl;
    tree.remove(20);
    tree.printTree();

    return 0;
}