#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

// Toggle logging for debugging
#define ENABLE_LOGGING 1
#if ENABLE_LOGGING
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

class TwoThreeNode {
public:
    std::vector<int> keys;
    std::vector<TwoThreeNode*> children;
    bool isLeaf;

    TwoThreeNode(bool leaf = true) : isLeaf(leaf) {
        children.resize(3, nullptr);
    }

    ~TwoThreeNode() {
        for (TwoThreeNode* child : children) {
            delete child;
        }
    }

    int numKeys() const { return keys.size(); }
    int numChildren() const { return std::count_if(children.begin(), children.end(), [](auto c) { return c != nullptr; }); }
};

struct SplitResult {
    int promotedKey;
    TwoThreeNode* rightNode;

    SplitResult(int key, TwoThreeNode* node) : promotedKey(key), rightNode(node) {}
};

class TwoThreeTree {
private:
    TwoThreeNode* root = nullptr;

    int findKeyIndex(const TwoThreeNode* node, int key) const {
        int idx = 0;
        while (idx < node->numKeys() && key > node->keys[idx]) ++idx;
        return idx;
    }

    SplitResult splitNode(TwoThreeNode* node, int key, TwoThreeNode* leftChild, TwoThreeNode* rightChild) {
        LOG("Splitting node with keys: " << (node->keys.empty() ? -1 : node->keys[0]) << (node->numKeys() > 1 ? (", " + std::to_string(node->keys[1])) : ""));

        std::vector<int> tempKeys = node->keys;
        tempKeys.push_back(key);
        std::sort(tempKeys.begin(), tempKeys.end());

        std::vector<TwoThreeNode*> tempChildren;
        if (!node->isLeaf) {
            tempChildren.reserve(4);
            for (TwoThreeNode* c : node->children) {
                if (c) tempChildren.push_back(c);
            }
            int idx = findKeyIndex(node, key);
            tempChildren.insert(tempChildren.begin() + idx + 1, rightChild);
        }

        // Create new right node
        TwoThreeNode* rightNode = new TwoThreeNode(node->isLeaf);

        node->keys = {tempKeys[0]};
        rightNode->keys = {tempKeys[2]};

        if (!node->isLeaf) {
            node->children = {tempChildren[0], tempChildren[1], nullptr};
            rightNode->children = {tempChildren[2], tempChildren[3], nullptr};
        }

        return SplitResult(tempKeys[1], rightNode);
    }

    SplitResult* insertRecursive(TwoThreeNode* node, int key) {
        LOG("Inserting " << key << " into node with keys: " << (node->keys.empty() ? -1 : node->keys[0]) << (node->numKeys() > 1 ? (", " + std::to_string(node->keys[1])) : ""));

        if (node->isLeaf) {
            if (node->numKeys() < 2) {
                node->keys.push_back(key);
                std::sort(node->keys.begin(), node->keys.end());
                return nullptr;
            } else {
                SplitResult split = splitNode(node, key, nullptr, nullptr);
                return new SplitResult(split.promotedKey, split.rightNode);
            }
        }

        int idx = findKeyIndex(node, key);
        TwoThreeNode* child = node->children[idx];
        assert(child != nullptr && "Invalid child pointer");

        SplitResult* childSplit = insertRecursive(child, key);
        if (!childSplit) return nullptr;

        int promotedKey = childSplit->promotedKey;
        TwoThreeNode* newChild = childSplit->rightNode;
        delete childSplit;

        if (node->numKeys() < 2) {
            node->keys.push_back(promotedKey);
            std::sort(node->keys.begin(), node->keys.end());

            int newIdx = findKeyIndex(node, promotedKey);
            for (int i = node->numChildren(); i > newIdx + 1; --i) {
                node->children[i] = node->children[i - 1];
            }
            node->children[newIdx + 1] = newChild;
            return nullptr;
        } else {
            SplitResult split = splitNode(node, promotedKey, child, newChild);
            return new SplitResult(split.promotedKey, split.rightNode);
        }
    }

public:
    ~TwoThreeTree() { delete root; }

    void insert(int key) {
        LOG("\nStarting insertion of key: " << key);

        if (!root) {
            root = new TwoThreeNode(true);
            root->keys.push_back(key);
            return;
        }

        SplitResult* rootSplit = insertRecursive(root, key);
        if (rootSplit) {
            TwoThreeNode* newRoot = new TwoThreeNode(false);
            newRoot->keys.push_back(rootSplit->promotedKey);
            newRoot->children[0] = root;
            newRoot->children[1] = rootSplit->rightNode;
            root = newRoot;
            delete rootSplit;
        }
    }

    void printTree() const {
        printTreeRecursive(root, 0);
    }

private:
    void printTreeRecursive(const TwoThreeNode* node, int depth) const {
        if (!node) return;

        for (int i = node->numKeys(); i >= 0; --i) {
            if (i < node->numKeys()) {
                for (int d = 0; d < depth; ++d) std::cout << "    ";
                std::cout << node->keys[i] << std::endl;
            }
            printTreeRecursive(i < node->children.size() ? node->children[i] : nullptr, depth + 1);
        }
    }
};

// --- MAIN DRIVER ---
int main() {
    TwoThreeTree tree;

    std::vector<int> values = {10, 20, 5, 15, 25, 30, 2, 7, 12, 17};
    for (int val : values) {
        tree.insert(val);
    }

    std::cout << "\nFinal 2-3 Tree Structure:\n";
    tree.printTree();

    return 0;
}
