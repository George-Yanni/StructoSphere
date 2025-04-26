#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
// The Rules of Coloring
// Red-Black Tree Properties
// Every node is either red or black.
// The root is always black.
// All leaves (NIL nodes) are black.
// If a node is red, then both its children must be black (no two red nodes can be adjacent).
// Every path from a given node to its descendant NIL nodes must have the same number of black nodes.

// Why??
// These properties ensure that the tree remains approximately balanced, which guarantees that basic 
// operations (insert, delete, search) are performed in O(log n) time.
enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node *left, *right, *parent;
    Node(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* NIL;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != NIL) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node* k) {
        while (k != root && k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                Node* u = k->parent->parent->right; // Uncle
                if (u->color == RED) {
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            } else {
                Node* u = k->parent->parent->left; // Uncle
                if (u->color == RED) {
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            }
            if (k == root) break;
        }
        root->color = BLACK;
    }

    void printTreeUtil(Node* node, int space) {
        const int COUNT = 5;
        if (node == NIL) return;
        
        space += COUNT;
        printTreeUtil(node->right, space);
        
        std::cout << std::endl;
        for (int i = COUNT; i < space; i++) std::cout << " ";
        std::cout << node->data << (node->color == RED ? "(R)" : "(B)") << "\n";
        
        printTreeUtil(node->left, space);
    }

public:
    RedBlackTree() {
        NIL = new Node(0);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL; // NIL should point to itself
        NIL->parent = nullptr;
        root = NIL;
    }

    void insert(int data) {
        Node* node = new Node(data);
        node->left = NIL;
        node->right = NIL;
        Node* y = nullptr;
        Node* x = root;

        while (x != NIL) {
            y = x;
            if (node->data < x->data) x = x->left;
            else x = x->right;
        }

        node->parent = y;
        if (y == nullptr) root = node;
        else if (node->data < y->data) y->left = node;
        else y->right = node;

        std::cout << "\nAfter inserting " << data << " (before fixing):\n";
        printTree();
        std::cout << "Fixing colors for node " << data << "...\n";
        fixInsert(node);
        std::cout << "Tree after color fixing:\n";
        printTree();
    }

    void printTree() {
        if (root == NIL) {
            std::cout << "Empty tree\n";
            return;
        }
        printTreeUtil(root, 0);
    }
};

int main() {
    RedBlackTree rbt;
    std::cout << "Red-Black Tree Color Illustration\n";
    std::cout << "================================\n";

    int values[] = {10, 20, 5, 15, 25};
    for (int val : values) {
        std::cout << "\nInserting " << val << ":\n";
        rbt.insert(val);
        std::cout << "================================\n";
    }

    return 0;
}