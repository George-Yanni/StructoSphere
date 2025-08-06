#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

class BTreeNode {
public:
    std::vector<int> keys;
    std::vector<BTreeNode*> children;
    bool isLeaf;

    BTreeNode(bool leaf) : isLeaf(leaf) {}

    ~BTreeNode() {
        for (BTreeNode* child : children)
            delete child;
    }
};

class BTree {
private:
    BTreeNode* root;
    int t; // Minimum degree

    void print(BTreeNode* node, int indent = 0) {
        if (!node) return;

        std::cout << std::setw(indent) << " ";
        for (int key : node->keys)
            std::cout << key << " ";
        std::cout << std::endl;

        for (BTreeNode* child : node->children)
            print(child, indent + 4);
    }

    bool search(BTreeNode* node, int key) {
        if (!node) return false;

        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) i++;

        if (i < node->keys.size() && node->keys[i] == key)
            return true;

        if (node->isLeaf)
            return false;
        return search(node->children[i], key);
    }

    void insertNonFull(BTreeNode* node, int key) {
        int i = node->keys.size() - 1;

        if (node->isLeaf) {
            node->keys.push_back(0);
            while (i >= 0 && node->keys[i] > key) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        } else {
            while (i >= 0 && node->keys[i] > key)
                i--;

            if (node->children[i + 1]->keys.size() == 2 * t - 1) {
                splitChild(node, i + 1);
                if (node->keys[i + 1] < key)
                    i++;
            }
            insertNonFull(node->children[i + 1], key);
        }
    }

    void splitChild(BTreeNode* parent, int i) {
        BTreeNode* fullChild = parent->children[i];
        BTreeNode* newNode = new BTreeNode(fullChild->isLeaf);

        // Move keys
        newNode->keys.assign(fullChild->keys.begin() + t, fullChild->keys.end());
        fullChild->keys.resize(t - 1);

        // Move children
        if (!fullChild->isLeaf) {
            newNode->children.assign(fullChild->children.begin() + t, fullChild->children.end());
            fullChild->children.resize(t);
        }

        parent->children.insert(parent->children.begin() + i + 1, newNode);
        parent->keys.insert(parent->keys.begin() + i, fullChild->keys[t - 1]);
    }

    void remove(BTreeNode* node, int key) {
        int idx = 0;
        while (idx < node->keys.size() && key > node->keys[idx])
            idx++;

        // Case 1: Key is in this node
        if (idx < node->keys.size() && node->keys[idx] == key) {
            if (node->isLeaf) {
                node->keys.erase(node->keys.begin() + idx);
            } else {
                removeInternalKey(node, key, idx);
            }
        }
        else {
            if (node->isLeaf)
                return; // Key not found
            bool flag = (idx == node->keys.size());
            if (node->children[idx]->keys.size() < t)
                fill(node, idx);
            if (flag && idx > node->keys.size())
                remove(node->children[idx - 1], key);
            else
                remove(node->children[idx], key);
        }
    }

    void removeInternalKey(BTreeNode* node, int key, int idx) {
        BTreeNode* predChild = node->children[idx];
        BTreeNode* succChild = node->children[idx + 1];

        if (predChild->keys.size() >= t) {
            int predKey = getPredecessor(predChild);
            node->keys[idx] = predKey;
            remove(predChild, predKey);
        } else if (succChild->keys.size() >= t) {
            int succKey = getSuccessor(succChild);
            node->keys[idx] = succKey;
            remove(succChild, succKey);
        } else {
            merge(node, idx);
            remove(predChild, key);
        }
    }

    int getPredecessor(BTreeNode* node) {
        while (!node->isLeaf)
            node = node->children.back();
        return node->keys.back();
    }

    int getSuccessor(BTreeNode* node) {
        while (!node->isLeaf)
            node = node->children.front();
        return node->keys.front();
    }

    void fill(BTreeNode* node, int idx) {
        if (idx != 0 && node->children[idx - 1]->keys.size() >= t)
            borrowFromPrev(node, idx);
        else if (idx != node->keys.size() && node->children[idx + 1]->keys.size() >= t)
            borrowFromNext(node, idx);
        else {
            if (idx != node->keys.size())
                merge(node, idx);
            else
                merge(node, idx - 1);
        }
    }

    void borrowFromPrev(BTreeNode* node, int idx) {
        BTreeNode* child = node->children[idx];
        BTreeNode* sibling = node->children[idx - 1];

        child->keys.insert(child->keys.begin(), node->keys[idx - 1]);
        if (!child->isLeaf)
            child->children.insert(child->children.begin(), sibling->children.back());

        node->keys[idx - 1] = sibling->keys.back();
        sibling->keys.pop_back();
        if (!sibling->isLeaf)
            sibling->children.pop_back();
    }

    void borrowFromNext(BTreeNode* node, int idx) {
        BTreeNode* child = node->children[idx];
        BTreeNode* sibling = node->children[idx + 1];

        child->keys.push_back(node->keys[idx]);
        if (!child->isLeaf)
            child->children.push_back(sibling->children.front());

        node->keys[idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
        if (!sibling->isLeaf)
            sibling->children.erase(sibling->children.begin());
    }

    void merge(BTreeNode* node, int idx) {
        BTreeNode* child = node->children[idx];
        BTreeNode* sibling = node->children[idx + 1];

        child->keys.push_back(node->keys[idx]);
        child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
        if (!child->isLeaf)
            child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());

        node->keys.erase(node->keys.begin() + idx);
        node->children.erase(node->children.begin() + idx + 1);

        delete sibling;
    }

public:
    BTree(int degree) : t(degree), root(new BTreeNode(true)) {}

    ~BTree() {
        delete root;
    }

    void insert(int key) {
        if (root->keys.size() == 2 * t - 1) {
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key);
    }

    bool search(int key) {
        return search(root, key);
    }

    void remove(int key) {
        remove(root, key);
        if (!root->keys.empty()) return;

        BTreeNode* oldRoot = root;
        if (!root->isLeaf)
            root = root->children[0];
        else
            root = new BTreeNode(true); // Empty tree

        delete oldRoot;
    }

    void print() {
        print(root, 0);
    }
};

int main() {
    BTree tree(3); // B-tree of minimum degree t = 3

    // Insert keys
    for (int key : {10, 20, 5, 6, 12, 30, 7, 17})
        tree.insert(key);

    std::cout << "B-Tree after insertions:\n";
    tree.print();

    // Search
    std::cout << "\nSearch for 6: " << (tree.search(6) ? "Found" : "Not found") << "\n";
    std::cout << "Search for 15: " << (tree.search(15) ? "Found" : "Not found") << "\n";

    // Remove some keys
    for (int key : {6, 13, 7}) {
        std::cout << "\nRemoving " << key << ":\n";
        tree.remove(key);
        tree.print();
    }

    return 0;
}
