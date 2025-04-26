// Aspect           	    AVL Tree	          Red-Black Tree
// Read-heavy systems	    ✅ Ideal            	❌ Less optimal
// Write-heavy systems	    ❌ Less optimal     	✅ Ideal


#include <iostream>
#include <iomanip>
#include <queue>
#include <cmath>

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;
    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
        return y;
    }

    Node* insertHelper(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->data)
            node->left = insertHelper(node->left, value);
        else if (value > node->data)
            node->right = insertHelper(node->right, value);
        else
            return node;

        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        int balance = getBalance(node);

        if (balance > 1 && value < node->left->data)
            return rightRotate(node);
        if (balance < -1 && value > node->right->data)
            return leftRotate(node);
        if (balance > 1 && value > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && value < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        std::cout << node->data << " ";
        inOrder(node->right);
    }

    void preOrder(Node* node) {
        if (!node) return;
        std::cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void postOrder(Node* node) {
        if (!node) return;
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->data << " ";
    }

    void levelOrder(Node* node) {
        if (!node) return;
        std::queue<Node*> q;
        q.push(node);
        while (!q.empty()) {
            Node* curr = q.front(); q.pop();
            std::cout << curr->data << " ";
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
    }

    Node* searchHelper(Node* node, int value) {
        if (!node || node->data == value)
            return node;
        if (value < node->data)
            return searchHelper(node->left, value);
        return searchHelper(node->right, value);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void printTreeHelper(Node* root, int space) {
        if (root == nullptr) return;
        space += 6;
        printTreeHelper(root->right, space);
        std::cout << std::endl;
        for (int i = 6; i < space; i++) std::cout << " ";
        std::cout << root->data << "\n";
        printTreeHelper(root->left, space);
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(int value) { root = insertHelper(root, value); }

    bool search(int value) {
        return searchHelper(root, value) != nullptr;
    }

    void inOrder() { inOrder(root); std::cout << std::endl; }

    void preOrder() { preOrder(root); std::cout << std::endl; }

    void postOrder() { postOrder(root); std::cout << std::endl; }

    void levelOrder() { levelOrder(root); std::cout << std::endl; }

    int height() { return getHeight(root); }

    void printTree() {
        std::cout << "\nTree (Top-Down):\n";
        printTreeHelper(root, 0);
    }
};

// Main driver
int main() {
    AVLTree tree;
    tree.insert(10);
    printf("Inserted 10\n");
    tree.printTree();
    tree.insert(20);
    printf("Inserted 20\n");
    tree.printTree();
    tree.insert(30);
    printf("Inserted 30\n");
    tree.printTree();
    tree.insert(40);
    printf("Inserted 40\n");
    tree.printTree();
    tree.insert(50);
    printf("Inserted 50\n");
    tree.printTree();
    tree.insert(25);
    printf("Inserted 25\n");

    tree.printTree();

    std::cout << "\nIn-order: ";
    tree.inOrder();

    std::cout << "Pre-order: ";
    tree.preOrder();

    std::cout << "Post-order: ";
    tree.postOrder();

    std::cout << "Level-order: ";
    tree.levelOrder();

    std::cout << "Height: " << tree.height() << "\n";

    std::cout << "\nSearch 25: " << (tree.search(25) ? "Found" : "Not found") << std::endl;
    std::cout << "Search 60: " << (tree.search(60) ? "Found" : "Not found") << std::endl;

    return 0;
}
