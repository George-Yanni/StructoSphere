// 2-3-4 Tree implementation in C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

// Node structure for 2-3-4 Tree
struct Node234 {
    std::vector<int> keys; // 1 to 3 keys
    std::vector<Node234*> children; // 0 to 4 children
    bool isLeaf;

    Node234(bool leaf = true) : isLeaf(leaf) {}

    int numKeys() const { return keys.size(); }
    int numChildren() const { return children.size(); }
};

class Tree234 {
private:
    Node234* root;

    struct SplitResult {
        int promotedKey;
        Node234* rightNode;
    };

    int findKeyIndex(Node234* node, int key) const {
        int idx = 0;
        while (idx < node->numKeys() && key > node->keys[idx]) {
            ++idx;
        }
        return idx;
    }

    SplitResult splitChild(Node234* parent, int idx) {
        Node234* node = parent->children[idx];
        assert(node->numKeys() == 3);

        int midKey = node->keys[1];

        Node234* leftChild = new Node234(node->isLeaf);
        Node234* rightChild = new Node234(node->isLeaf);

        leftChild->keys.push_back(node->keys[0]);
        rightChild->keys.push_back(node->keys[2]);

        if (!node->isLeaf) {
            leftChild->children.push_back(node->children[0]);
            leftChild->children.push_back(node->children[1]);
            rightChild->children.push_back(node->children[2]);
            rightChild->children.push_back(node->children[3]);
        }

        parent->keys.insert(parent->keys.begin() + idx, midKey);
        parent->children[idx] = leftChild;
        parent->children.insert(parent->children.begin() + idx + 1, rightChild);

        delete node;

        return { midKey, rightChild };
    }

    void insertNonFull(Node234* node, int key) {
        if (node->isLeaf) {
            node->keys.insert(std::upper_bound(node->keys.begin(), node->keys.end(), key), key);
        } else {
            int idx = findKeyIndex(node, key);
            if (node->children[idx]->numKeys() == 3) {
                splitChild(node, idx);
                if (key > node->keys[idx]) {
                    ++idx;
                }
            }
            insertNonFull(node->children[idx], key);
        }
    }

    void printTreeRecursive(Node234* node, int depth = 0) const {
        if (!node) return;
        for (int d = 0; d < depth; ++d) std::cout << "    ";
        std::cout << "[";
        for (size_t i = 0; i < node->keys.size(); ++i) {
            std::cout << node->keys[i];
            if (i + 1 < node->keys.size()) std::cout << ", ";
        }
        std::cout << "]" << (node->isLeaf ? " (Leaf)" : "") << std::endl;

        for (Node234* child : node->children) {
            printTreeRecursive(child, depth + 1);
        }
    }

    void destroy(Node234* node) {
        if (!node) return;
        for (Node234* child : node->children) {
            destroy(child);
        }
        delete node;
    }

public:
    Tree234() : root(nullptr) {}
    ~Tree234() { destroy(root); }

    void insert(int key) {
        if (!root) {
            root = new Node234(true);
            root->keys.push_back(key);
            return;
        }

        if (root->numKeys() == 3) {
            Node234* newRoot = new Node234(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key);
    }

    void printTree() const {
        std::cout << "2-3-4 Tree structure:" << std::endl;
        printTreeRecursive(root);
        std::cout << "-------------------------" << std::endl;
    }
};

// Main function for demonstration
int main() {
    Tree234 tree;
    std::vector<int> keys = {10, 20, 5, 6, 12, 30, 7, 17, 3, 15, 25, 35, 9, 18, 22, 28, 32};

    for (int key : keys) {
        std::cout << "Inserting " << key << std::endl;
        tree.insert(key);
        tree.printTree();
    }

    return 0;
}
