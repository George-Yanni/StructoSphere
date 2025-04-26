#include <iostream>
#include <iomanip>

using namespace std;

// Red-Black Tree Node structure
enum Color {RED, BLACK};

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    Color color;
};

// Red-Black Tree class
class RedBlackTree {
private:
    Node* root;
    Node* TNULL;

    // Helper function to print the tree in shape
    void printTreeShapeHelper(Node* node, int space);
    
public:
    RedBlackTree();
    void insert(int key);
    void printTreeShape();
    void leftRotate(Node* x);
    void rightRotate(Node* y);
    void insertFixup(Node* z);
    void transplant(Node* u, Node* v);
    void deleteNode(int key);
    void deleteFixup(Node* x);
    Node* searchTree(int key);
    Node* minimum(Node* node);
    void inorderHelper(Node* node);
    void preorderHelper(Node* node);
};

// Constructor
RedBlackTree::RedBlackTree() {
    TNULL = new Node;
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
}

// Left Rotate
void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Right Rotate
void RedBlackTree::rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != TNULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == nullptr) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// Insert node
void RedBlackTree::insert(int key) {
    Node* node = new Node;
    node->parent = nullptr;
    node->data = key;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;

    Node* y = nullptr;
    Node* x = root;

    while (x != TNULL) {
        y = x;
        if (node->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->data < y->data) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == nullptr) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == nullptr) {
        return;
    }

    insertFixup(node);
}

// Fix up the tree after insertion
void RedBlackTree::insertFixup(Node* z) {
    Node* y = nullptr;
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->right) {
            y = z->parent->parent->left;
            if (y->color == RED) {
                y->color = BLACK;
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        } else {
            y = z->parent->parent->right;
            if (y->color == RED) {
                y->color = BLACK;
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        if (z == root) {
            break;
        }
    }
    root->color = BLACK;
}

// Function to print tree in shape
void RedBlackTree::printTreeShapeHelper(Node* node, int space) {
    if (node == TNULL) {
        return;
    }

    space += 10;

    // Print the right subtree first
    printTreeShapeHelper(node->right, space);

    // Print the current node
    cout << endl;
    for (int i = 10; i < space; i++) {
        cout << " ";
    }

    cout << node->data << "(" << (node->color == RED ? "RED" : "BLACK") << ")" << endl;

    // Print the left subtree
    printTreeShapeHelper(node->left, space);
}

// Function to start tree shape printing
void RedBlackTree::printTreeShape() {
    printTreeShapeHelper(root, 0);
}

int main() {
    RedBlackTree tree;

    tree.insert(50);
    printf("Inserted 50\n");
    tree.printTreeShape();
    tree.insert(40);
    printf("Inserted 40\n");
    tree.printTreeShape();
    tree.insert(30);
    printf("Inserted 30\n");
    tree.printTreeShape();
    tree.insert(25);
    printf("Inserted 25\n");
    tree.printTreeShape();
    tree.insert(20);
    printf("Inserted 20\n");
    tree.printTreeShape();
    tree.insert(10);
    printf("Inserted 10\n");
    tree.printTreeShape();

    cout << "Tree printed in shape:" << endl;
    tree.printTreeShape();

    return 0;
}
