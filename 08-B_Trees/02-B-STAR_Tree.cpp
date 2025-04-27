#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

// Define constants
static constexpr int MIN_DEGREE = 3;
static constexpr int MAX_KEYS = 2 * MIN_DEGREE - 1; // 5
static constexpr int MAX_CHILDREN = 2 * MIN_DEGREE; // 6
static constexpr int MIN_KEYS = static_cast<int>(std::ceil(2.0 * MAX_KEYS / 3.0)); // 4
static constexpr int MAX_RECURSION_DEPTH = 100;

// Node structure
struct BStarTreeNode
{
    bool isLeaf;
    int numKeys;
    std::vector<int> keys;
    std::vector<BStarTreeNode *> children;

    BStarTreeNode(bool leaf) : isLeaf(leaf), numKeys(0)
    {
        keys.resize(MAX_KEYS);
        children.resize(MAX_CHILDREN);
        std::fill(children.begin(), children.end(), nullptr);
    }
};

// B*-Tree class
class BStarTree
{
private:
    BStarTreeNode *root;
    int recursionDepth;

    // --- Helper Functions ---

    int findKeyIndex(BStarTreeNode *node, int k)
    {
        int idx = 0;
        while (idx < node->numKeys && node->keys[idx] < k)
            ++idx;
        return idx;
    }

    void splitChildRoot(BStarTreeNode *parent, int index, BStarTreeNode *child)
    {
        BStarTreeNode *newChild = new BStarTreeNode(child->isLeaf);
        newChild->numKeys = MIN_DEGREE - 1;

        for (int i = 0; i < MIN_DEGREE - 1; ++i)
            newChild->keys[i] = child->keys[i + MIN_DEGREE];

        if (!child->isLeaf)
        {
            for (int i = 0; i < MIN_DEGREE; ++i)
                newChild->children[i] = child->children[i + MIN_DEGREE];
            for (int i = MIN_DEGREE; i < MAX_CHILDREN; ++i)
                child->children[i] = nullptr;
        }

        child->numKeys = MIN_DEGREE - 1;

        for (int i = parent->numKeys; i >= index + 1; --i)
            parent->children[i + 1] = parent->children[i];

        parent->children[index + 1] = newChild;

        for (int i = parent->numKeys - 1; i >= index; --i)
            parent->keys[i + 1] = parent->keys[i];

        parent->keys[index] = child->keys[MIN_DEGREE - 1];
        parent->numKeys++;

        assert(parent->numKeys <= MAX_KEYS && "Parent overflow in splitChildRoot");
    }

    void borrowFromLeft(BStarTreeNode *parent, int index, BStarTreeNode *nodeToFill, BStarTreeNode *leftSibling)
    {
        std::cout << "Borrowing from left sibling. nodeToFill keys: [";
        for (int i = 0; i < nodeToFill->numKeys; ++i)
            std::cout << nodeToFill->keys[i] << (i < nodeToFill->numKeys - 1 ? ", " : "");
        std::cout << "], leftSibling keys: [";
        for (int i = 0; i < leftSibling->numKeys; ++i)
            std::cout << leftSibling->keys[i] << (i < leftSibling->numKeys - 1 ? ", " : "");
        std::cout << "]" << std::endl;

        // Ensure nodeToFill has space
        assert(nodeToFill->numKeys < MAX_KEYS && "nodeToFill cannot borrow when full");

        // Move keys in nodeToFill to make space
        for (int i = nodeToFill->numKeys - 1; i >= 0; --i)
            nodeToFill->keys[i + 1] = nodeToFill->keys[i];

        // Move children if not a leaf
        if (!nodeToFill->isLeaf)
        {
            for (int i = nodeToFill->numKeys; i >= 0; --i)
                nodeToFill->children[i + 1] = nodeToFill->children[i];
            nodeToFill->children[0] = leftSibling->children[leftSibling->numKeys];
            leftSibling->children[leftSibling->numKeys] = nullptr;
        }

        // Move parent key to nodeToFill
        nodeToFill->keys[0] = parent->keys[index - 1];
        nodeToFill->numKeys++;

        // Move leftSibling's last key to parent
        parent->keys[index - 1] = leftSibling->keys[leftSibling->numKeys - 1];
        leftSibling->numKeys--;

        std::cout << "After borrowing. nodeToFill keys: [";
        for (int i = 0; i < nodeToFill->numKeys; ++i)
            std::cout << nodeToFill->keys[i] << (i < nodeToFill->numKeys - 1 ? ", " : "");
        std::cout << "], parent key[" << index - 1 << "]: " << parent->keys[index - 1] << std::endl;
    }

    void borrowFromRight(BStarTreeNode *parent, int index, BStarTreeNode *nodeToFill, BStarTreeNode *rightSibling)
    {
        std::cout << "Borrowing from right sibling. nodeToFill keys: [";
        for (int i = 0; i < nodeToFill->numKeys; ++i)
            std::cout << nodeToFill->keys[i] << (i < nodeToFill->numKeys - 1 ? ", " : "");
        std::cout << "], rightSibling keys: [";
        for (int i = 0; i < rightSibling->numKeys; ++i)
            std::cout << rightSibling->keys[i] << (i < rightSibling->numKeys - 1 ? ", " : "");
        std::cout << "]" << std::endl;

        // Ensure nodeToFill has space
        assert(nodeToFill->numKeys < MAX_KEYS && "nodeToFill cannot borrow when full");

        // Move parent key to nodeToFill
        nodeToFill->keys[nodeToFill->numKeys] = parent->keys[index];
        nodeToFill->numKeys++;

        // Move child if not a leaf
        if (!nodeToFill->isLeaf)
        {
            nodeToFill->children[nodeToFill->numKeys] = rightSibling->children[0];
            rightSibling->children[0] = nullptr;
        }

        // Move rightSibling's first key to parent
        parent->keys[index] = rightSibling->keys[0];

        // Shift keys in rightSibling
        for (int i = 1; i < rightSibling->numKeys; ++i)
            rightSibling->keys[i - 1] = rightSibling->keys[i];

        // Shift children in rightSibling
        if (!rightSibling->isLeaf)
        {
            for (int i = 1; i <= rightSibling->numKeys; ++i)
                rightSibling->children[i - 1] = rightSibling->children[i];
            rightSibling->children[rightSibling->numKeys] = nullptr;
        }

        rightSibling->numKeys--;

        std::cout << "After borrowing. nodeToFill keys: [";
        for (int i = 0; i < nodeToFill->numKeys; ++i)
            std::cout << nodeToFill->keys[i] << (i < nodeToFill->numKeys - 1 ? ", " : "");
        std::cout << "], parent key[" << index << "]: " << parent->keys[index] << std::endl;
    }

    void mergeAndSplit3WayRight(BStarTreeNode *parent, int index, BStarTreeNode *grandParent = nullptr, int grandParentIndex = -1)
    {
        std::cout << "Performing 2-to-3 split (right) at parent index " << index << std::endl;

        BStarTreeNode *node = parent->children[index];
        BStarTreeNode *rightSibling = parent->children[index + 1];
        int parentKeyIndex = index;

        // Combine keys and children
        int totalKeys = node->numKeys + rightSibling->numKeys + 1;
        std::vector<int> combinedKeys(totalKeys);
        std::vector<BStarTreeNode *> combinedChildren(totalKeys + 1, nullptr);

        for (int i = 0; i < node->numKeys; ++i)
            combinedKeys[i] = node->keys[i];
        if (!node->isLeaf)
            for (int i = 0; i <= node->numKeys; ++i)
                combinedChildren[i] = node->children[i];

        combinedKeys[node->numKeys] = parent->keys[parentKeyIndex];

        for (int i = 0; i < rightSibling->numKeys; ++i)
            combinedKeys[node->numKeys + 1 + i] = rightSibling->keys[i];
        if (!rightSibling->isLeaf)
            for (int i = 0; i <= rightSibling->numKeys; ++i)
                combinedChildren[node->numKeys + 1 + i] = rightSibling->children[i];

        // Calculate split points
        int keysNode1 = static_cast<int>(std::floor((double)totalKeys / 3.0));
        int keysNode2 = static_cast<int>(std::floor((double)(totalKeys + 1) / 3.0));
        int keysNode3 = totalKeys - keysNode1 - keysNode2 - 2;

        int promotedKey1Index = keysNode1;
        int promotedKey2Index = keysNode1 + 1 + keysNode2;

        int promotedKey1 = combinedKeys[promotedKey1Index];
        int promotedKey2 = combinedKeys[promotedKey2Index];

        // Create new middle node
        BStarTreeNode *middleNode = new BStarTreeNode(node->isLeaf);

        // Populate first node (reuse node)
        node->numKeys = keysNode1;
        for (int i = 0; i < keysNode1; ++i)
            node->keys[i] = combinedKeys[i];
        if (!node->isLeaf)
        {
            for (int i = 0; i <= keysNode1; ++i)
                node->children[i] = combinedChildren[i];
            for (int i = keysNode1 + 1; i < MAX_CHILDREN; ++i)
                node->children[i] = nullptr;
        }

        // Populate middle node
        middleNode->numKeys = keysNode2;
        for (int i = 0; i < keysNode2; ++i)
            middleNode->keys[i] = combinedKeys[promotedKey1Index + 1 + i];
        if (!middleNode->isLeaf)
        {
            for (int i = 0; i <= keysNode2; ++i)
                middleNode->children[i] = combinedChildren[promotedKey1Index + 1 + i];
            for (int i = keysNode2 + 1; i < MAX_CHILDREN; ++i)
                middleNode->children[i] = nullptr;
        }

        // Populate third node (reuse rightSibling)
        rightSibling->numKeys = keysNode3;
        for (int i = 0; i < keysNode3; ++i)
            rightSibling->keys[i] = combinedKeys[promotedKey2Index + 1 + i];
        if (!rightSibling->isLeaf)
        {
            for (int i = 0; i <= keysNode3; ++i)
                rightSibling->children[i] = combinedChildren[promotedKey2Index + 1 + i];
            for (int i = keysNode3 + 1; i < MAX_CHILDREN; ++i)
                rightSibling->children[i] = nullptr;
        }

        // Update parent
        for (int i = parent->numKeys; i > parentKeyIndex + 1; --i)
        {
            parent->keys[i] = parent->keys[i - 1];
            parent->children[i + 1] = parent->children[i];
        }
        parent->keys[parentKeyIndex] = promotedKey1;
        parent->keys[parentKeyIndex + 1] = promotedKey2;
        parent->children[parentKeyIndex + 1] = middleNode;
        parent->children[parentKeyIndex + 2] = rightSibling;
        parent->numKeys++;

        // Handle parent overflow
        if (parent->numKeys > MAX_KEYS)
        {
            if (parent == root)
            {
                BStarTreeNode *newRoot = new BStarTreeNode(false);
                newRoot->children[0] = root;
                splitChildRoot(newRoot, 0, root);
                root = newRoot;
            }
            else if (grandParent)
            {
                bool handled = false;
                if (grandParentIndex > 0)
                {
                    BStarTreeNode *leftSibling = grandParent->children[grandParentIndex - 1];
                    if (leftSibling->numKeys < MAX_KEYS)
                    {
                        borrowFromLeft(grandParent, grandParentIndex, parent, leftSibling);
                        handled = true;
                    }
                }
                if (!handled && grandParentIndex < grandParent->numKeys)
                {
                    BStarTreeNode *rightSibling = grandParent->children[grandParentIndex + 1];
                    if (rightSibling->numKeys < MAX_KEYS)
                    {
                        borrowFromRight(grandParent, grandParentIndex, parent, rightSibling);
                        handled = true;
                    }
                }
                if (!handled)
                {
                    if (grandParentIndex > 0)
                        mergeAndSplit3WayLeft(grandParent, grandParentIndex);
                    else
                        mergeAndSplit3WayRight(grandParent, grandParentIndex);
                }
            }
        }
    }

    void mergeAndSplit3WayLeft(BStarTreeNode *parent, int index, BStarTreeNode *grandParent = nullptr, int grandParentIndex = -1)
    {
        std::cout << "Performing 2-to-3 split (left) at parent index " << index << std::endl;
        mergeAndSplit3WayRight(parent, index - 1, grandParent, grandParentIndex - 1);
    }

    void insertRecursive(BStarTreeNode *node, int key, BStarTreeNode *parent = nullptr, int parentIndex = -1, int depth = 0)
    {
        if (depth > MAX_RECURSION_DEPTH)
        {
            std::cerr << "Error: Maximum recursion depth exceeded" << std::endl;
            exit(1);
        }

        std::cout << "Inserting key " << key << " at depth " << depth << ", node keys: [";
        for (int i = 0; i < node->numKeys; ++i)
            std::cout << node->keys[i] << (i < node->numKeys - 1 ? ", " : "");
        std::cout << "]" << std::endl;

        int i = node->numKeys - 1;

        if (node->isLeaf)
        {
            // Only insert if there's space
            if (node->numKeys < MAX_KEYS)
            {
                while (i >= 0 && key < node->keys[i])
                {
                    node->keys[i + 1] = node->keys[i];
                    i--;
                }
                node->keys[i + 1] = key;
                node->numKeys++;
            }
            else
            {
                // Leaf is full, defer to parent to handle split
                std::cout << "Leaf node full, deferring to parent" << std::endl;
                return;
            }
        }
        else
        {
            while (i >= 0 && key < node->keys[i])
                i--;
            int childIndex = i + 1;
            BStarTreeNode *child = node->children[childIndex];
            assert(child != nullptr && "Invalid child pointer");

            if (child->numKeys == MAX_KEYS)
            {
                std::cout << "Child at index " << childIndex << " is full, handling overflow. Child keys: [";
                for (int j = 0; j < child->numKeys; ++j)
                    std::cout << child->keys[j] << (j < child->numKeys - 1 ? ", " : "");
                std::cout << "]" << std::endl;

                bool handled = false;

                if (childIndex > 0)
                {
                    BStarTreeNode *leftSibling = node->children[childIndex - 1];
                    std::cout << "Left sibling keys: [";
                    for (int j = 0; j < leftSibling->numKeys; ++j)
                        std::cout << leftSibling->keys[j] << (j < leftSibling->numKeys - 1 ? ", " : "");
                    std::cout << "]" << std::endl;

                    // Check if child has space and left sibling can lend
                    if (child->numKeys < MAX_KEYS && leftSibling->numKeys < MAX_KEYS - 1)
                    {
                        std::cout << "Borrowing from left sibling" << std::endl;
                        borrowFromLeft(node, childIndex, child, leftSibling);
                        handled = true;
                    }
                }

                if (!handled && childIndex < node->numKeys)
                {
                    BStarTreeNode *rightSibling = node->children[childIndex + 1];
                    std::cout << "Right sibling keys: [";
                    for (int j = 0; j < rightSibling->numKeys; ++j)
                        std::cout << rightSibling->keys[j] << (j < rightSibling->numKeys - 1 ? ", " : "");
                    std::cout << "]" << std::endl;

                    // Check if child has space and right sibling can lend
                    if (child->numKeys < MAX_KEYS && rightSibling->numKeys < MAX_KEYS - 1)
                    {
                        std::cout << "Borrowing from right sibling" << std::endl;
                        borrowFromRight(node, childIndex, child, rightSibling);
                        handled = true;
                    }
                }

                if (!handled)
                {
                    std::cout << "Performing 2-to-3 split" << std::endl;
                    if (childIndex > 0)
                        mergeAndSplit3WayLeft(node, childIndex, parent, parentIndex);
                    else
                        mergeAndSplit3WayRight(node, childIndex, parent, parentIndex);

                    // Recalculate child index
                    i = node->numKeys - 1;
                    while (i >= 0 && key < node->keys[i])
                        i--;
                    childIndex = i + 1;
                    child = node->children[childIndex];
                    assert(child != nullptr && "Invalid child after split");
                }
            }

            // Recurse only if child has space
            if (child->numKeys < MAX_KEYS)
            {
                insertRecursive(child, key, node, childIndex, depth + 1);
            }
            else
            {
                // Child is still full, trigger another overflow handling
                std::cout << "Child still full after handling, reprocessing" << std::endl;
                insertRecursive(node, key, parent, parentIndex, depth);
            }
        }
    }

    void traverse(BStarTreeNode *node)
    {
        if (!node)
            return;
        int i;
        for (i = 0; i < node->numKeys; i++)
        {
            if (!node->isLeaf)
                traverse(node->children[i]);
            std::cout << node->keys[i] << " ";
        }
        if (!node->isLeaf)
            traverse(node->children[i]);
    }

    BStarTreeNode *search(BStarTreeNode *node, int key)
    {
        if (!node)
            return nullptr;
        int i = findKeyIndex(node, key);

        if (i < node->numKeys && node->keys[i] == key)
            return node;

        if (node->isLeaf)
            return nullptr;

        return search(node->children[i], key);
    }

    void printTree(BStarTreeNode *node, int depth)
    {
        if (!node)
            return;

        for (int d = 0; d < depth; ++d)
            std::cout << "    ";

        std::cout << "[";
        for (int i = 0; i < node->numKeys; ++i)
        {
            std::cout << node->keys[i] << (i == node->numKeys - 1 ? "" : ", ");
        }
        std::cout << "]" << (node->isLeaf ? " (Leaf)" : "") << " (n=" << node->numKeys << ")" << std::endl;

        if (!node->isLeaf)
        {
            for (int i = 0; i <= node->numKeys; ++i)
            {
                if (node->children[i])
                    printTree(node->children[i], depth + 1);
            }
        }
    }

    void destroyTree(BStarTreeNode *node)
    {
        if (!node)
            return;

        if (!node->isLeaf)
        {
            for (int i = 0; i <= node->numKeys; ++i)
            {
                if (node->children[i])
                    destroyTree(node->children[i]);
            }
        }
        delete node;
    }

public:
    BStarTree() : root(nullptr), recursionDepth(0) {}

    ~BStarTree()
    {
        destroyTree(root);
        root = nullptr;
    }

    void insert(int key)
    {
        if (!root)
        {
            root = new BStarTreeNode(true);
            root->keys[0] = key;
            root->numKeys = 1;
            return;
        }

        if (root->numKeys == MAX_KEYS)
        {
            std::cout << "Root is full, splitting" << std::endl;
            BStarTreeNode *newRoot = new BStarTreeNode(false);
            newRoot->children[0] = root;
            splitChildRoot(newRoot, 0, root);
            root = newRoot;
        }

        insertRecursive(root, key);
    }

    void traverse()
    {
        if (root)
            traverse(root);
        else
            std::cout << "Tree is empty.";
        std::cout << std::endl;
    }

    bool search(int key)
    {
        return search(root, key) != nullptr;
    }

    void printTree()
    {
        std::cout << "Tree structure:" << std::endl;
        if (!root)
        {
            std::cout << "Tree is empty." << std::endl;
        }
        else
        {
            printTree(root, 0);
        }
        std::cout << "-------------------------" << std::endl;
    }
};

// Main function for testing
int main()
{
    BStarTree tree;
    const int keysToInsert[] = {10, 20, 5, 6, 12, 30, 7, 17, 3, 15, 25, 35, 9, 18, 22, 28, 32, 40, 45, 50,
                                1, 2, 4, 8, 11, 13, 14, 16, 19, 21, 23, 24, 26, 27, 29, 31, 33, 34, 36, 37,
                                38, 39, 41, 42, 43, 44, 46, 47, 48, 49, 51, 52, 53, 54, 55};

    for (int key : keysToInsert)
    {
        std::cout << "Inserting " << key << std::endl;
        tree.insert(key);
        tree.printTree();
    }

    std::cout << "\nTraversal of the B* Tree (In-order): ";
    tree.traverse();

    std::cout << "\nFinal Tree structure:\n";
    tree.printTree();

    int key = 6;
    std::cout << "Search for key " << key << ": " << (tree.search(key) ? "Found" : "Not Found") << std::endl;

    key = 99;
    std::cout << "Search for key " << key << ": " << (tree.search(key) ? "Found" : "Not Found") << std::endl;

    key = 45;
    std::cout << "Search for key " << key << ": " << (tree.search(key) ? "Found" : "Not Found") << std::endl;

    key = 1;
    std::cout << "Search for key " << key << ": " << (tree.search(key) ? "Found" : "Not Found") << std::endl;

    return 0;
}