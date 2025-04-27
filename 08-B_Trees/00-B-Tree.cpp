#include <iostream>
#include <vector>
#include <iomanip>   // For potential future formatting (not strictly needed now)
#include <algorithm> // For std::lower_bound, std::copy
#include <string>
#include <stdexcept> // For potential error handling (like invalid t)

// Forward declaration
class BTreeNode;

//-----------------------------------------------------
// BTree Class Declaration
//-----------------------------------------------------
class BTree
{
private:
    BTreeNode *root;
    int t; // Minimum degree

    // Helper function for recursive deletion
    void deleteTree(BTreeNode *node);
    // Helper function for recursive printing
    void printTreeShape(const BTreeNode *node, int level, const std::string& prefix) const;

public:
    // Constructor
    explicit BTree(int minimum_degree); // Use explicit to prevent implicit conversions

    // Destructor
    ~BTree();

    // Public Interface
    void traverse() const;
    BTreeNode *search(int k) const;
    void insert(int k);
    void remove(int k);
    void printTreeShape() const;

    // Getter for minimum degree (optional)
    int getMinimumDegree() const { return t; }
};

//-----------------------------------------------------
// BTreeNode Class Declaration
//-----------------------------------------------------
class BTreeNode
{
private:
    std::vector<int> keys;
    std::vector<BTreeNode *> children;
    int t;    // Minimum degree (same as BTree's t)
    bool leaf; // True if node is a leaf, false otherwise

    // Private helper methods for insertion and deletion
    void insertNonFull(int k);
    void splitChild(int i, BTreeNode *y); // i is index of y in children vector
    void remove(int k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPred(int idx) const; // Find predecessor
    int getSucc(int idx) const; // Find successor
    void fill(int idx);         // Ensure child[idx] has at least t keys
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx); // Merge child[idx] and child[idx+1]

public:
    // Constructor
    BTreeNode(int minimum_degree, bool is_leaf);

    // Public methods (mostly called internally or via BTree)
    void traverse() const;
    BTreeNode *search(int k); // Non-const version needed for internal modification potential? No, search itself doesn't modify. Make const.
    // Make BTree a friend class to allow access to private members
    friend class BTree;
};

//-----------------------------------------------------
// BTreeNode Implementation
//-----------------------------------------------------

BTreeNode::BTreeNode(int minimum_degree, bool is_leaf) : t(minimum_degree), leaf(is_leaf)
{
    // Reserve space to minimize reallocations, though vectors will grow if needed
    // Max keys = 2t - 1
    // Max children = 2t
    keys.reserve(2 * t - 1);
    children.reserve(2 * t);
}

// Traverse the subtree rooted with this node (in-order)
void BTreeNode::traverse() const
{
    int i;
    for (i = 0; i < keys.size(); i++)
    {
        // If this is not a leaf, then before printing key[i],
        // traverse the subtree rooted with child children[i].
        if (!leaf)
            children[i]->traverse();
        std::cout << keys[i] << " ";
    }

    // Print the subtree rooted with the last child
    if (!leaf)
        children[i]->traverse(); // i is now equal to keys.size()
}

// Search for key k in the subtree rooted with this node
BTreeNode *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    // std::lower_bound could be used here too, but linear scan is fine for small node sizes
    int i = 0;
    while (i < keys.size() && k > keys[i])
        i++;

    // If the found key is equal to k, return this node
    if (i < keys.size() && keys[i] == k)
        return this;

    // If the key is not found here and this is a leaf node
    if (leaf)
        return nullptr;

    // Go to the appropriate child
    return children[i]->search(k);
}

// Insert a new key in the subtree rooted with this node.
// Assumes this node is non-full when this method is called.
void BTreeNode::insertNonFull(int k)
{
    // Initialize index as index of rightmost element
    int i = keys.size() - 1;

    // If this is a leaf node
    if (leaf)
    {
        // Find the location of the new key to be inserted
        // Move all greater keys one place ahead
        // Using std::lower_bound is more idiomatic C++
        auto insert_pos = std::lower_bound(keys.begin(), keys.end(), k);
        keys.insert(insert_pos, k);
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
            i--;
        // i is now the index of the key just before the target child index
        i++; // i is now the index of the target child

        // See if the found child is full
        if (children[i]->keys.size() == 2 * t - 1)
        {
            // If the child is full, then split it
            splitChild(i, children[i]);

            // After split, the middle key of children[i] goes up and
            // children[i] is split into two. See which of the two
            // children is going to have the new key.
            if (keys[i] < k) // Compare with the new key moved up to the parent
                i++;
        }
        children[i]->insertNonFull(k);
    }
}

// Split the child y of this node. i is index of y in children vector.
// y must be full when this function is called.
void BTreeNode::splitChild(int i, BTreeNode *y)
{
    // Create a new node which is going to store (t-1) keys of y
    BTreeNode *z = new BTreeNode(y->t, y->leaf);

    // --- FIX: Store the middle key *before* resizing y ---
    int middleKey = y->keys[t - 1];

    // Copy the last (t-1) keys of y to z
    // keys from index t to 2t-2 (t-1 keys total)
    z->keys.assign(y->keys.begin() + t, y->keys.end());

    // Copy the last t children of y to z if y is not a leaf
    // children from index t to 2t-1 (t children total)
    if (!y->leaf)
    {
        z->children.assign(y->children.begin() + t, y->children.end());
    }

    // Reduce the number of keys in y to t-1
    y->keys.resize(t - 1);

    // Reduce the number of children in y if it's not a leaf
    if (!y->leaf)
    {
        y->children.resize(t);
    }

    // Since this node is going to have a new child,
    // create space for the new child in the children vector
    children.insert(children.begin() + i + 1, z);

    // Link the middle key of y to this node
    // --- FIX: Use the stored middle key ---
    keys.insert(keys.begin() + i, middleKey);
}

// Wrapper function to remove the key k from the subtree rooted with this node.
void BTreeNode::remove(int k)
{
    // Find the first key greater than or equal to k
    auto it = std::lower_bound(keys.begin(), keys.end(), k);
    int idx = std::distance(keys.begin(), it);

    // Check if the key k is present in this node
    if (idx < keys.size() && keys[idx] == k)
    {
        // Key found in this node
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {
        // Key k is not in this node
        // If this is a leaf node, then the key is not in the tree
        if (leaf)
        {
            std::cout << "Key " << k << " does not exist in the tree.\n";
            return;
        }

        // The key k might be in the subtree rooted at children[idx]
        // Determine if the key is in the last child's subtree
        bool is_last_child = (idx == keys.size());

        // If the child where the key is supposed to exist has fewer than t keys,
        // we need to fill that child first
        if (children[idx]->keys.size() < t)
            fill(idx);

        // After potentially filling, the child might have changed.
        // If 'is_last_child' was true and we merged the last child (idx)
        // with its previous sibling (idx-1) during fill, then the key k
        // is now in the subtree rooted at children[idx-1].
        // Otherwise, the key is in the subtree rooted at children[idx].
        if (is_last_child && idx > keys.size()) // Check if merge happened with previous sibling
            children[idx - 1]->remove(k);
        else
            children[idx]->remove(k);
    }
}

// Remove the key at index idx from this node, which is a leaf node.
void BTreeNode::removeFromLeaf(int idx)
{
    keys.erase(keys.begin() + idx);
}

// Remove the key at index idx from this node, which is a non-leaf node.
void BTreeNode::removeFromNonLeaf(int idx)
{
    int k = keys[idx];

    // Case 1: Child preceding k (children[idx]) has at least t keys.
    // Find the predecessor 'pred' of k in the subtree rooted at children[idx].
    // Replace k with 'pred'. Recursively delete 'pred' in children[idx].
    if (children[idx]->keys.size() >= t)
    {
        int pred = getPred(idx);
        keys[idx] = pred;
        children[idx]->remove(pred);
    }
    // Case 2: Child succeeding k (children[idx+1]) has at least t keys.
    // Find the successor 'succ' of k in the subtree rooted at children[idx+1].
    // Replace k with 'succ'. Recursively delete 'succ' in children[idx+1].
    else if (children[idx + 1]->keys.size() >= t)
    {
        int succ = getSucc(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ);
    }
    // Case 3: Both children[idx] and children[idx+1] have less than t keys.
    // Merge k and all of children[idx+1] into children[idx].
    // Now children[idx] contains 2t-1 keys.
    // Delete children[idx+1] and recursively delete k from children[idx].
    else
    {
        merge(idx);
        // k is now in the merged child children[idx]
        children[idx]->remove(k); // Recurse on the merged child
    }
}

// Get the predecessor of the key at keys[idx] in this non-leaf node.
// The predecessor is the rightmost key in the subtree rooted at children[idx].
int BTreeNode::getPred(int idx) const
{
    BTreeNode *cur = children[idx];
    while (!cur->leaf)
        cur = cur->children.back(); // Go to the rightmost child
    return cur->keys.back(); // Return the last key of the leaf
}

// Get the successor of the key at keys[idx] in this non-leaf node.
// The successor is the leftmost key in the subtree rooted at children[idx+1].
int BTreeNode::getSucc(int idx) const
{
    BTreeNode *cur = children[idx + 1];
    while (!cur->leaf)
        cur = cur->children.front(); // Go to the leftmost child
    return cur->keys.front(); // Return the first key of the leaf
}

// Fill child children[idx] which has less than t keys.
void BTreeNode::fill(int idx)
{
    // Try borrowing from the previous sibling (children[idx-1])
    if (idx != 0 && children[idx - 1]->keys.size() >= t)
        borrowFromPrev(idx);
    // Try borrowing from the next sibling (children[idx+1])
    else if (idx != keys.size() && children[idx + 1]->keys.size() >= t)
        borrowFromNext(idx);
    // If borrowing is not possible, merge the child with a sibling.
    // If children[idx] is not the last child, merge it with the next sibling.
    // Otherwise, merge it with the previous sibling.
    else
    {
        if (idx != keys.size()) // Merge with next sibling
            merge(idx);
        else // Merge with previous sibling (idx becomes idx-1 after merge)
            merge(idx - 1);
    }
}

// Borrow a key from children[idx-1] and place it in children[idx].
void BTreeNode::borrowFromPrev(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx - 1];

    // The key keys[idx-1] from the current node moves down into child as the first key.
    // Insert at the beginning requires shifting existing keys.
    child->keys.insert(child->keys.begin(), keys[idx - 1]);

    // If child is not a leaf, move the last child of the sibling to be the first child of child.
    if (!child->leaf)
    {
        child->children.insert(child->children.begin(), sibling->children.back());
        sibling->children.pop_back(); // Remove the child from sibling
    }

    // The last key of the sibling moves up to the current node, replacing keys[idx-1].
    keys[idx - 1] = sibling->keys.back();
    sibling->keys.pop_back(); // Remove the key from sibling
}

// Borrow a key from children[idx+1] and place it in children[idx].
void BTreeNode::borrowFromNext(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];

    // The key keys[idx] from the current node moves down into child as the last key.
    child->keys.push_back(keys[idx]);

    // If child is not a leaf, move the first child of the sibling to be the last child of child.
    if (!child->leaf)
    {
        child->children.push_back(sibling->children.front());
        sibling->children.erase(sibling->children.begin()); // Remove the child from sibling
    }

    // The first key of the sibling moves up to the current node, replacing keys[idx].
    keys[idx] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin()); // Remove the key from sibling
}

// Merge children[idx+1] into children[idx].
// Assumes children[idx] and children[idx+1] each have t-1 keys.
void BTreeNode::merge(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];

    // Pull the key keys[idx] from the current node down into child.
    // This key will become the median key in the merged child.
    child->keys.push_back(keys[idx]);

    // Copy all keys from sibling to the end of child.
    child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

    // If the children are not leaves, copy the children pointers from sibling to child.
    if (!child->leaf)
    {
        child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
    }

    // Remove the key keys[idx] from the current node.
    keys.erase(keys.begin() + idx);

    // Remove the pointer to the sibling from the current node's children.
    children.erase(children.begin() + idx + 1);

    // Free the memory occupied by the sibling node.
    delete sibling;
}

//-----------------------------------------------------
// BTree Implementation
//-----------------------------------------------------

BTree::BTree(int minimum_degree) : root(nullptr), t(minimum_degree)
{
    // B-Tree minimum degree must be at least 2.
    if (t < 2)
    {
        // Consider throwing an exception for a more robust error handling
        // throw std::invalid_argument("B-Tree minimum degree must be at least 2.");
        std::cerr << "Warning: B-Tree minimum degree must be at least 2. Setting t = 2." << std::endl;
        t = 2;
    }
}

BTree::~BTree()
{
    deleteTree(root);
}

// Recursive helper function to delete the tree
void BTree::deleteTree(BTreeNode *node)
{
    if (node == nullptr)
        return;

    // Recurse for non-leaf nodes
    if (!node->leaf)
    {
        for (BTreeNode *child : node->children)
        {
            deleteTree(child);
        }
    }
    // Delete the current node after its children are deleted
    delete node;
}

// Traverse the tree (in-order)
void BTree::traverse() const
{
    if (root != nullptr)
    {
        root->traverse();
        std::cout << std::endl;
    }
    else
    {
        std::cout << "The tree is empty." << std::endl;
    }
}

// Search for a key k in the tree
BTreeNode *BTree::search(int k) const
{
    return (root == nullptr) ? nullptr : root->search(k);
}

// Insert a new key k into the B-Tree
void BTree::insert(int k)
{
    // If tree is empty
    if (root == nullptr)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys.push_back(k); // Insert key
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->keys.size() == 2 * t - 1)
        {
            // Allocate memory for new root
            BTreeNode *new_root = new BTreeNode(t, false); // New root is internal node

            // Make old root a child of the new root
            new_root->children.push_back(root);

            // Split the old root and move 1 key to the new root
            new_root->splitChild(0, root); // Split child at index 0 (the old root)

            // New root has two children now. Decide which of the
            // two children is going to have the new key.
            int i = 0;
            if (new_root->keys[0] < k)
                i++; // Key goes into the right child
            new_root->children[i]->insertNonFull(k);

            // Change root
            root = new_root;
        }
        else // If root is not full, call insertNonFull for root
        {
            root->insertNonFull(k);
        }
    }
}

// Remove key k from the B-Tree
void BTree::remove(int k)
{
    if (root == nullptr)
    {
        std::cout << "The tree is empty\n";
        return;
    }

    // Call the remove function for the root node
    root->remove(k);

    // If the root node has 0 keys after removal
    if (root->keys.empty())
    {
        BTreeNode *old_root = root;
        // If the root was a leaf, the tree is now empty
        if (root->leaf)
        {
            root = nullptr;
        }
        // If the root was not a leaf, make its first (and only) child the new root
        else
        {
            root = root->children[0];
        }
        // Free the old root
        delete old_root;
    }
}

// Print the tree structure (for debugging/visualization)
void BTree::printTreeShape() const
{
    if (root == nullptr)
    {
        std::cout << "The tree is empty\n";
        return;
    }
    std::cout << "B-Tree Structure (t=" << t << "):" << std::endl;
    std::cout << "------------------------" << std::endl;
    printTreeShape(root, 0, "");
    std::cout << "------------------------" << std::endl;
}

// Recursive helper for printing tree structure
void BTree::printTreeShape(const BTreeNode *node, int level, const std::string& prefix) const
{
    if (node == nullptr) return;

    // Print node's keys
    std::cout << prefix << "[";
    for (size_t i = 0; i < node->keys.size(); ++i)
    {
        std::cout << node->keys[i] << (i == node->keys.size() - 1 ? "" : ", ");
    }
    std::cout << "]";
    if (node->leaf) {
        std::cout << " (Leaf)";
    }
    std::cout << std::endl;

    // Recursively print children with appropriate indentation and range info
    if (!node->leaf)
    {
        std::string childPrefix = prefix + "  |--"; // Indentation for children
        for (size_t i = 0; i < node->children.size(); ++i)
        {
            // Print range information (optional but helpful)
            std::cout << prefix << "  |   Child " << i;
             if (!node->keys.empty()) {
                std::cout << " (";
                if (i == 0) {
                    std::cout << "keys < " << node->keys[0];
                } else if (i == node->keys.size()) { // Child after the last key
                    std::cout << "keys > " << node->keys[i - 1];
                } else { // Child between keys[i-1] and keys[i]
                    std::cout << node->keys[i - 1] << " < keys < " << node->keys[i];
                }
                std::cout << ")";
            }
            std::cout << ":" << std::endl;

            // Recursive call
            printTreeShape(node->children[i], level + 1, prefix + "  |  ");
        }
    }
}


//-----------------------------------------------------
// Driver Code (main function)
//-----------------------------------------------------
int main()
{
    // Create a B-Tree with minimum degree 2 (t=2 means max 3 keys, max 4 children)
    BTree t(2);

    // Testing the insertion operation
    std::vector<int> keys_to_insert = {10, 20, 5, 6, 12, 30, 7, 17};
    std::cout << "--- Insertion Phase ---" << std::endl;
    for (int k : keys_to_insert)
    {
        std::cout << "\nInserting " << k << "..." << std::endl;
        t.insert(k);
        t.printTreeShape();
    }

    std::cout << "\n--- Final Tree after Insertions ---" << std::endl;
    std::cout << "Tree traversal (inorder): ";
    t.traverse();
    t.printTreeShape();

    // Testing the deletion operation
    std::cout << "\n--- Deletion Phase ---" << std::endl;

    int key_to_delete = 6;
    std::cout << "\nDeleting " << key_to_delete << "..." << std::endl;
    t.remove(key_to_delete);
    t.printTreeShape();
    std::cout << "Traversal after deleting " << key_to_delete << ": "; t.traverse();

    key_to_delete = 13; // Key not in tree
    std::cout << "\nAttempting to delete " << key_to_delete << " (not present)..." << std::endl;
    t.remove(key_to_delete);
    t.printTreeShape();
    std::cout << "Traversal after attempting delete " << key_to_delete << ": "; t.traverse();


    key_to_delete = 7;
    std::cout << "\nDeleting " << key_to_delete << "..." << std::endl;
    t.remove(key_to_delete);
    t.printTreeShape();
    std::cout << "Traversal after deleting " << key_to_delete << ": "; t.traverse();

    key_to_delete = 12; // Causes merge/borrow potentially
    std::cout << "\nDeleting " << key_to_delete << "..." << std::endl;
    t.remove(key_to_delete);
    t.printTreeShape();
    std::cout << "Traversal after deleting " << key_to_delete << ": "; t.traverse();

    int key_to_insert = 13;
    std::cout << "\nInserting " << key_to_insert << "..." << std::endl;
    t.insert(key_to_insert);
    t.printTreeShape();
    std::cout << "Traversal after inserting " << key_to_insert << ": "; t.traverse();


    // Additional test: Delete remaining keys to test empty tree handling
    std::cout << "\n--- Deleting Remaining Keys ---" << std::endl;
    std::vector<int> remaining_keys = {5, 10, 13, 17, 20, 30}; // Order might matter for testing different scenarios
    for (int k : remaining_keys)
    {
        std::cout << "\nDeleting " << k << "..." << std::endl;
        t.remove(k);
        t.printTreeShape();
        std::cout << "Traversal after deleting " << k << ": "; t.traverse();
    }

    std::cout << "\n--- Final State (Should be empty) ---" << std::endl;
    t.printTreeShape();
    t.traverse();


    return 0;
}
