#include <iostream>
#include <iomanip>
#include <memory>
#include <string>

enum Color { RED, BLACK };

struct Node {
    int key;
    bool color;
    int size; // size of subtree rooted at this node
    Node* left;
    Node* right;
    Node* parent;

    Node(int key) : key(key), color(RED), size(1), left(nullptr), right(nullptr), parent(nullptr) {}
};

class OrderStatisticTree {
private:
    Node* root;
    Node* NIL;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
        y->size = x->size;
        x->size = 1 + x->left->size + x->right->size;
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != NIL) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
        x->size = y->size;
        y->size = 1 + y->left->size + y->right->size;
    }

    void insertFixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
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
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
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
            }
        }
        root->color = BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == NIL) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    Node* treeMinimum(Node* x) const {
        while (x->left != NIL) x = x->left;
        return x;
    }

    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void updateSize(Node* x) {
        while (x != NIL) {
            x->size = 1 + x->left->size + x->right->size;
            x = x->parent;
        }
    }

    void deleteNode(Node* z) {
        Node* y = z;
        Node* x;
        bool yOriginalColor = y->color;
        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = treeMinimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        updateSize(x->parent);
        if (yOriginalColor == BLACK) deleteFixup(x);
    }

    void printTree(Node* node, int indent = 0) const {
        if (node != NIL) {
            if (node->right != NIL)
                printTree(node->right, indent + 6);
            if (indent)
                std::cout << std::setw(indent) << ' ';
            std::cout << node->key << (node->color == RED ? "R" : "B") << "[" << node->size << "]\n";
            if (node->left != NIL)
                printTree(node->left, indent + 6);
        }
    }

public:
    OrderStatisticTree() {
        NIL = new Node(0);
        NIL->color = BLACK;
        NIL->size = 0;
        root = NIL;
    }

    void insert(int key) {
        Node* z = new Node(key);
        Node* y = NIL;
        Node* x = root;
        while (x != NIL) {
            x->size++;
            y = x;
            if (z->key < x->key) x = x->left;
            else x = x->right;
        }
        z->parent = y;
        if (y == NIL)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
        z->left = z->right = NIL;
        z->color = RED;
        insertFixup(z);
    }

    void remove(int key) {
        Node* z = root;
        while (z != NIL && z->key != key) {
            z = (key < z->key) ? z->left : z->right;
        }
        if (z == NIL) return;
        deleteNode(z);
    }

    int select(int k) const {
        Node* x = root;
        while (x != NIL) {
            int r = x->left->size + 1;
            if (k == r) return x->key;
            else if (k < r) x = x->left;
            else {
                k -= r;
                x = x->right;
            }
        }
        throw std::out_of_range("Rank out of bounds");
    }

    int rank(int key) const {
        Node* x = root;
        int r = 0;
        while (x != NIL) {
            if (key < x->key) x = x->left;
            else {
                r += x->left->size + 1;
                if (key == x->key) return r;
                x = x->right;
            }
        }
        return r;
    }

    void print() const {
        printTree(root);
    }

    ~OrderStatisticTree() {
        clear(root);
        delete NIL;
    }

private:
    void clear(Node* node) {
        if (node != NIL) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
};

// Example usage
int main() {
    OrderStatisticTree ost;
    printf("Note that in the following illustration the number between [ ] is the total number of nodes (including the node itself) and all its subsequent nodes. Also the B/R is Black or Red as this implementation uses Red Black Tree. \n\n");

    ost.insert(20);
    printf("Inserted 20\n");
    ost.print();
    ost.insert(15);
    printf("Inserted 15\n");
    ost.print();    
    ost.insert(25);
    printf("Inserted 25\n");
    ost.print();
    ost.insert(10);
    printf("Inserted 10\n");
    ost.print();
    ost.insert(5);
    printf("Inserted 5\n");
    ost.print();
    ost.insert(30);
    printf("Inserted 30\n");
    ost.insert(17);
    printf("Inserted 17\n");
    ost.print();

    std::cout << "Tree structure:\n";
    ost.print();

    std::cout << "\nSelect 4th smallest: " << ost.select(4) << "\n";
    std::cout << "Rank of 17: " << ost.rank(17) << "\n";

    ost.remove(15);
    std::cout << "\nAfter deleting 15:\n";
    ost.print();

    std::cout << "\nSelect 4th smallest: " << ost.select(4) << "\n";
    std::cout << "Rank of 20: " << ost.rank(20) << "\n";

    return 0;
}
