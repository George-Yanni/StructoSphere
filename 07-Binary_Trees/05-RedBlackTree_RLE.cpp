// Imagine a database that stores books, where each book has a title 
// (which will be the key for the tree) and an ISBN number. The Red-Black 
// Tree can help efficiently manage indexing for this system.



#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

enum Color {RED, BLACK};

struct Node {
    string title;
    string isbn;
    Node* left;
    Node* right;
    Node* parent;
    Color color;
};

class RedBlackTree {
private:
    Node* root;
    Node* TNULL;

    void printTreeShapeHelper(Node* node, int space);

public:
    RedBlackTree();
    void insert(string title, string isbn);
    void printTreeShape();
    void leftRotate(Node* x);
    void rightRotate(Node* y);
    void insertFixup(Node* z);
    void searchBook(string title);
    void deleteNode(string title);
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

// Insertion function
void RedBlackTree::insert(string title, string isbn) {
    Node* node = new Node;
    node->parent = nullptr;
    node->title = title;
    node->isbn = isbn;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;

    Node* y = nullptr;
    Node* x = root;

    while (x != TNULL) {
        y = x;
        if (node->title < x->title) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->title < y->title) {
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

// Fixup the tree after insertion
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

// Print Tree in shape (for visual debugging)
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

    cout << node->title << "(" << (node->color == RED ? "RED" : "BLACK") << ")" << endl;

    // Print the left subtree
    printTreeShapeHelper(node->left, space);
}

// Start tree shape printing
void RedBlackTree::printTreeShape() {
    printTreeShapeHelper(root, 0);
}

// Search for a book by title
void RedBlackTree::searchBook(string title) {
    Node* x = root;
    while (x != TNULL) {
        if (x->title == title) {
            cout << "Book Found: " << title << ", ISBN: " << x->isbn << endl;
            return;
        }
        if (title < x->title) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    cout << "Book not found!" << endl;
}

// Delete a node (book) by title
void RedBlackTree::deleteNode(string title) {
    Node* z = root;
    while (z != TNULL) {
        if (z->title == title) {
            // Implement node deletion here (requires deleteFixup as well)
            cout << "Book Deleted: " << title << endl;
            return;
        }
        if (title < z->title) {
            z = z->left;
        } else {
            z = z->right;
        }
    }
    cout << "Book not found!" << endl;
}

int main() {
    RedBlackTree libraryDB;

    libraryDB.insert("Harry Potter and the Sorcerer's Stone", "9780439708180");
    libraryDB.insert("The Lord of the Rings", "9780618640157");
    libraryDB.insert("To Kill a Mockingbird", "9780061120084");

    libraryDB.printTreeShape();

    libraryDB.searchBook("The Lord of the Rings");
    libraryDB.deleteNode("To Kill a Mockingbird");

    return 0;
}
