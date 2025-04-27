#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <optional>
#include <cmath> // For ceil

// Forward declaration
class BPTreeNode;

//-----------------------------------------------------
// B+ Tree Node Class Declaration
//-----------------------------------------------------
class BPTreeNode {
private:
    std::vector<int> keys;
    std::vector<BPTreeNode*> children; // Only used by internal nodes
    int t;                             // Minimum degree (defines node size)
    bool leaf;                         // True if node is a leaf
    BPTreeNode* next;                  // Pointer to the next leaf node (only for leaves)

    // Note: No findParent here; parent tracking is done by BPTree methods

public:
    // Constructor
    BPTreeNode(int minimum_degree, bool is_leaf);
    ~BPTreeNode(); // Destructor

    // Public methods
    int getKeyCount() const { return keys.size(); }
    bool isLeaf() const { return leaf; }
    BPTreeNode* getChild(int idx) const;
    int getKey(int idx) const;
    BPTreeNode* getNext() const { return next; } // Only valid for leaves
    int getMinKeys() const { return t - 1; } // Minimum keys for non-root node
    int getMaxKeys() const { return 2 * t - 1; } // Maximum keys

    // Friend class for access to private members
    friend class BPTree;
};

//-----------------------------------------------------
// B+ Tree Class Declaration
//-----------------------------------------------------
class BPTree {
private:
    BPTreeNode* root;
    int t; // Minimum degree

    // --- Recursive Helpers ---
    void deleteTreeRecursive(BPTreeNode* node);
    void printTreeShapeRecursive(const BPTreeNode* node, int level, const std::string& prefix) const;
    BPTreeNode* findLeftmostLeaf() const;

    // --- Insertion Helpers ---
    void insertInternal(BPTreeNode* parent, BPTreeNode* child, int key, BPTreeNode* new_child);
    // No separate split functions needed; logic integrated into insert/insertInternal

    // --- Deletion Helpers ---
    void removeFromNode(int key, BPTreeNode* node, BPTreeNode* parent); // Main recursive removal helper
    void handleUnderflow(BPTreeNode* node, BPTreeNode* parent, int child_idx_in_parent); // Dispatches to leaf/internal handlers
    void handleLeafUnderflow(BPTreeNode* node, BPTreeNode* parent, int child_idx_in_parent);
    void handleInternalUnderflow(BPTreeNode* node, BPTreeNode* parent, int child_idx_in_parent);

    // Borrow/Merge Helpers (Leaf)
    void borrowFromLeftLeaf(BPTreeNode* node, BPTreeNode* left_sibling, BPTreeNode* parent, int parent_key_idx);
    void borrowFromRightLeaf(BPTreeNode* node, BPTreeNode* right_sibling, BPTreeNode* parent, int parent_key_idx);
    void mergeLeafNodes(BPTreeNode* left_node, BPTreeNode* right_node, BPTreeNode* parent, int parent_key_idx);

    // Borrow/Merge Helpers (Internal)
    void borrowFromLeftInternal(BPTreeNode* node, BPTreeNode* left_sibling, BPTreeNode* parent, int parent_key_idx);
    void borrowFromRightInternal(BPTreeNode* node, BPTreeNode* right_sibling, BPTreeNode* parent, int parent_key_idx);
    void mergeInternalNodes(BPTreeNode* left_node, BPTreeNode* right_node, BPTreeNode* parent, int parent_key_idx);

    // Utility to find child index in parent (used during deletion)
    int findChildIndex(BPTreeNode* parent, BPTreeNode* child);


public:
    // Constructor
    explicit BPTree(int minimum_degree);

    // Destructor
    ~BPTree();

    // Public Interface
    void insert(int k);
    void remove(int k);
    bool search(int k) const; // Returns true if found, false otherwise
    void printLeaves() const; // Traverses the linked list of leaves
    void printTreeShape() const; // Prints the hierarchical structure

    // Getter for minimum degree (optional)
    int getMinimumDegree() const { return t; }
};


//-----------------------------------------------------
// BPTreeNode Implementation
//-----------------------------------------------------

BPTreeNode::BPTreeNode(int minimum_degree, bool is_leaf)
    : t(minimum_degree), leaf(is_leaf), next(nullptr) {
    // Max keys = 2t - 1
    // Max children = 2t
    keys.reserve(getMaxKeys());
    if (!is_leaf) {
        children.reserve(getMaxKeys() + 1); // Reserve space for 2t children
    }
    // Note: In a real B+ Tree, leaves would store data pointers/records.
    // Here, keys in leaves represent the data.
}

BPTreeNode::~BPTreeNode() {
    // The BPTree destructor handles recursive deletion.
    // This destructor is mainly for cleanup if needed, but children
    // are managed by the tree structure (splits/merges).
}

BPTreeNode* BPTreeNode::getChild(int idx) const {
    if (leaf || idx < 0 || idx >= children.size()) {
         // Consider throwing an error or returning nullptr safely
         // For simplicity, assume valid index based on tree logic
         return nullptr; // Or throw std::out_of_range("Child index out of bounds");
    }
    return children[idx];
}

int BPTreeNode::getKey(int idx) const {
     if (idx < 0 || idx >= keys.size()) {
         // Consider throwing an error
         throw std::out_of_range("Key index out of bounds");
     }
     return keys[idx];
}


//-----------------------------------------------------
// BPTree Implementation
//-----------------------------------------------------

BPTree::BPTree(int minimum_degree) : root(nullptr), t(minimum_degree) {
    if (t < 2) {
        // B+ Tree definition requires t >= 2
        throw std::invalid_argument("B+ Tree minimum degree must be at least 2.");
    }
}

BPTree::~BPTree() {
    deleteTreeRecursive(root);
}

void BPTree::deleteTreeRecursive(BPTreeNode* node) {
    if (node == nullptr) {
        return;
    }
    if (!node->leaf) {
        for (BPTreeNode* child : node->children) {
            deleteTreeRecursive(child);
        }
    }
    delete node;
}

// Helper to find the leftmost leaf node (for printing leaves)
BPTreeNode* BPTree::findLeftmostLeaf() const {
    if (!root) return nullptr;
    BPTreeNode* current = root;
    while (!current->leaf) {
        if (current->children.empty()) return nullptr; // Should not happen in valid tree
        current = current->children[0];
    }
    return current;
}

// Print all keys in the leaf nodes in order
void BPTree::printLeaves() const {
    std::cout << "Leaf Nodes (Data): ";
    BPTreeNode* current = findLeftmostLeaf();
    if (!current) {
        std::cout << "[Empty]" << std::endl;
        return;
    }

    while (current != nullptr) {
        std::cout << "[";
        for (size_t i = 0; i < current->keys.size(); ++i) {
            std::cout << current->keys[i] << (i == current->keys.size() - 1 ? "" : ",");
        }
        std::cout << "] -> ";
        current = current->next;
    }
    std::cout << "NULL" << std::endl;
}

// Search for a key k in the tree
bool BPTree::search(int k) const {
    if (root == nullptr) {
        return false;
    }

    // Traverse down to the leaf
    BPTreeNode* current = root;
    while (!current->leaf) {
        // Find the first key >= k
        auto it = std::lower_bound(current->keys.begin(), current->keys.end(), k);
        int index = std::distance(current->keys.begin(), it);
        // If k is >= the key found, we need to go right of that key's index
        if (it != current->keys.end() && *it == k) {
             index++; // Go to the right child if key matches internal node key
        }
        if (index >= current->children.size()) return false; // Should not happen
        current = current->children[index];
    }

    // Check if the key exists in the found leaf node
    auto it = std::lower_bound(current->keys.begin(), current->keys.end(), k);
    return (it != current->keys.end() && *it == k);
}


// --- Insertion Logic ---

void BPTree::insert(int k) {
    // Case 1: Tree is empty
    if (root == nullptr) {
        root = new BPTreeNode(t, true);
        root->keys.push_back(k);
        return;
    }

    // Traverse down to find the appropriate leaf, keeping track of parent
    BPTreeNode* parent = nullptr;
    BPTreeNode* current = root;
    while (!current->leaf) {
        parent = current;
        // Find the first key >= k
        auto it = std::lower_bound(current->keys.begin(), current->keys.end(), k);
        int index = std::distance(current->keys.begin(), it);
         // If k is >= the key found, we need to go right of that key's index
        if (it != current->keys.end() && *it == k) {
             index++; // Go to the right child if key matches internal node key
        }
        if (index >= current->children.size()) { // Safety check
             std::cerr << "Error: Child index out of bounds during insertion traversal." << std::endl;
             return;
        }
        current = current->children[index];
    }

    // Now 'current' is the target leaf node, 'parent' is its parent (or nullptr if root is leaf)

    // Check for duplicates (optional, based on requirements)
    // auto it_find = std::lower_bound(current->keys.begin(), current->keys.end(), k);
    // if (it_find != current->keys.end() && *it_find == k) {
    //     std::cout << "Key " << k << " already exists. Insertion skipped.\n";
    //     return;
    // }

    // Case 2: Leaf node has space
    if (current->keys.size() < current->getMaxKeys()) {
        auto it_insert = std::lower_bound(current->keys.begin(), current->keys.end(), k);
        current->keys.insert(it_insert, k);
    }
    // Case 3: Leaf node is full -> Split
    else {
        // Create a temporary list with the new key
        std::vector<int> temp_keys = current->keys;
        auto it_insert = std::lower_bound(temp_keys.begin(), temp_keys.end(), k);
        temp_keys.insert(it_insert, k);

        // Create a new leaf node (the right sibling)
        BPTreeNode* new_leaf = new BPTreeNode(t, true);

        // Find the split point (middle key goes up, but stays in right leaf too in B+)
        int split_idx = t; // Index of the first key to move to the new node (size is 2t now)
        int key_to_promote = temp_keys[split_idx]; // This key goes to parent

        // Distribute keys
        current->keys.assign(temp_keys.begin(), temp_keys.begin() + split_idx);
        new_leaf->keys.assign(temp_keys.begin() + split_idx, temp_keys.end());

        // Update leaf pointers
        new_leaf->next = current->next;
        current->next = new_leaf;

        // Promote the middle key to the parent
        insertInternal(parent, current, key_to_promote, new_leaf);
    }
}

// Insert a key and a new right child into an internal node (handles splits recursively)
void BPTree::insertInternal(BPTreeNode* parent, BPTreeNode* child, int key, BPTreeNode* new_child) {
    // Case 1: We split the root (parent is null)
    if (parent == nullptr) {
        root = new BPTreeNode(t, false); // New root is internal
        root->keys.push_back(key);
        root->children.push_back(child);     // Original root (or its left part)
        root->children.push_back(new_child); // New right child from split
        return;
    }

    // Find position to insert the key in the parent
    auto it_key = std::lower_bound(parent->keys.begin(), parent->keys.end(), key);
    int key_insert_idx = std::distance(parent->keys.begin(), it_key);
    // The new child goes *after* the position of the inserted key
    int child_insert_idx = key_insert_idx + 1;

    // Case 2: Parent has space
    if (parent->keys.size() < parent->getMaxKeys()) {
        parent->keys.insert(it_key, key);
        parent->children.insert(parent->children.begin() + child_insert_idx, new_child);
    }
    // Case 3: Parent is full -> Split internal node
    else {
        // Temporary vectors to hold keys and children including the new ones
        std::vector<int> temp_keys = parent->keys;
        std::vector<BPTreeNode*> temp_children = parent->children;

        temp_keys.insert(temp_keys.begin() + key_insert_idx, key);
        temp_children.insert(temp_children.begin() + child_insert_idx, new_child);

        // Create new internal node (right sibling)
        BPTreeNode* new_internal = new BPTreeNode(t, false);

        // Find split point for internal node (middle key goes up)
        int split_idx = t; // Key at index t moves up (size is 2t now)
        int key_to_promote = temp_keys[split_idx];

        // Distribute keys and children
        // Keys before split_idx stay in original node
        parent->keys.assign(temp_keys.begin(), temp_keys.begin() + split_idx);
        // Keys after split_idx go to new node
        new_internal->keys.assign(temp_keys.begin() + split_idx + 1, temp_keys.end());

        // Children up to and including split_idx stay in original node
        parent->children.assign(temp_children.begin(), temp_children.begin() + split_idx + 1);
        // Children after split_idx go to new node
        new_internal->children.assign(temp_children.begin() + split_idx + 1, temp_children.end());

        // Find the parent of the current parent to promote the key further up
        // This requires finding the parent again, or ideally passing the path down.
        // For simplicity here, we'll re-find the parent's parent if needed.
        // A more optimized approach passes the full path or uses parent pointers.

        // Find parent's parent (inefficient way, better to track path)
        BPTreeNode* grandparent = nullptr;
        if (parent != root) {
             BPTreeNode* current_gp_search = root;
             while (current_gp_search && !current_gp_search->leaf) {
                 bool found = false;
                 for(const auto& child_ptr : current_gp_search->children) {
                     if (child_ptr == parent) {
                         grandparent = current_gp_search;
                         found = true;
                         break;
                     }
                 }
                 if (found) break;

                 // Decide which child to descend into next (similar to search)
                 auto it = std::upper_bound(current_gp_search->keys.begin(), current_gp_search->keys.end(), parent->keys[0]); // Use first key as guide
                 int index = std::distance(current_gp_search->keys.begin(), it);
                 if (index >= current_gp_search->children.size()) break; // Should not happen
                 current_gp_search = current_gp_search->children[index];
             }
        }

        // Recursively insert the promoted key into the grandparent
        insertInternal(grandparent, parent, key_to_promote, new_internal);
    }
}


// --- Deletion Logic ---

void BPTree::remove(int k) {
    if (!root) {
        std::cout << "Tree is empty. Cannot delete " << k << std::endl;
        return;
    }

    removeFromNode(k, root, nullptr);

    // Check if the root node became empty or needs adjustment
    if (root && root->keys.empty()) {
        if (root->leaf) {
            // If root was a leaf and is now empty, tree is empty
            delete root;
            root = nullptr;
        } else if (!root->children.empty()) {
            // If root is an internal node and became empty (due to merge),
            // promote its only child to be the new root.
            BPTreeNode* old_root = root;
            root = root->children[0];
            old_root->children.clear(); // Avoid deleting the new root
            delete old_root;
        }
        // If root->children is empty but it's not a leaf, something is wrong.
    }
}

// Recursive helper to find and remove key k starting from 'node'
// 'parent' is the parent of 'node'
void BPTree::removeFromNode(int k, BPTreeNode* node, BPTreeNode* parent) {
    if (!node) return; // Should not happen in normal flow

    if (node->leaf) {
        // --- Case 1: Key is in a leaf node ---
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), k);
        if (it != node->keys.end() && *it == k) {
            // Key found, remove it
            node->keys.erase(it);

            // Check for underflow (only if not root)
            if (node != root && node->keys.size() < node->getMinKeys()) {
                int child_idx = findChildIndex(parent, node);
                handleUnderflow(node, parent, child_idx);
            }
        } else {
            std::cout << "Key " << k << " not found in the tree." << std::endl;
        }
    } else {
        // --- Case 2: Key is in an internal node ---
        // Find the appropriate child to descend into
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), k);
        int index = std::distance(node->keys.begin(), it);

        // If k matches a key in the internal node, we descend into the *right* child
        // because in B+, the actual data is always in the leaves.
        if (it != node->keys.end() && *it == k) {
            index++;
        }

        if (index >= node->children.size()) {
             std::cout << "Key " << k << " potentially out of range (larger than max key)." << std::endl;
             // Or descend into the rightmost child if logic dictates
             // index = node->children.size() - 1; // This might be needed depending on exact search logic match
             // For now, assume not found if index goes out of bounds here.
             return;
        }

        BPTreeNode* child_node = node->children[index];

        // Recursive call
        removeFromNode(k, child_node, node);

        // After returning from recursion, check if the child node underflowed
        // Note: We check size *after* potential modifications in the recursive call
        if (child_node->keys.size() < child_node->getMinKeys()) {
             // The handleUnderflow call might have already happened if child was a leaf.
             // However, if the child was internal and *its* child caused a merge that
             // propagated up, the current 'child_node' might now be underflowed.
             // Re-check and handle if necessary. This ensures internal node underflow is caught.
             // We need the index of child_node within node's children again.
             int child_idx = findChildIndex(node, child_node); // Find index again
             if (child_idx != -1) { // Make sure child still exists (wasn't merged away)
                 // Check size again *before* calling handleUnderflow
                 if(node->children[child_idx]->keys.size() < node->children[child_idx]->getMinKeys()) {
                    handleUnderflow(node->children[child_idx], node, child_idx);
                 }
             }
        }
         // --- Update Internal Key if Necessary ---
         // If k was deleted from a leaf, and k was also present as a separator key
         // in an ancestor internal node, that internal key might need updating
         // if the smallest key in the leaf's right sibling (or the merged node) changed.
         // This is implicitly handled by the merge/borrow logic updating parent keys correctly.
         // For example, mergeLeafNodes updates the parent key. Borrow updates parent key.
    }
}

// Utility to find the index of a child within its parent's children list
int BPTree::findChildIndex(BPTreeNode* parent, BPTreeNode* child) {
    if (!parent || parent->leaf) return -1;
    for (size_t i = 0; i < parent->children.size(); ++i) {
        if (parent->children[i] == child) {
            return i;
        }
    }
    return -1; // Child not found
}

// Central dispatcher for handling underflow
void BPTree::handleUnderflow(BPTreeNode* node, BPTreeNode* parent, int child_idx_in_parent) {
    if (!parent) {
        // This should only happen if the root itself underflows, which is handled
        // after the main remove call returns (checking root->keys.empty()).
        return;
    }

    if (node->leaf) {
        handleLeafUnderflow(node, parent, child_idx_in_parent);
    } else {
        handleInternalUnderflow(node, parent, child_idx_in_parent);
    }
}

// Handle underflow in a leaf node
void BPTree::handleLeafUnderflow(BPTreeNode* node, BPTreeNode* parent, int child_idx_in_parent) {
    BPTreeNode* left_sibling = (child_idx_in_parent > 0) ? parent->children[child_idx_in_parent - 1] : nullptr;
    BPTreeNode* right_sibling = (child_idx_in_parent < parent->children.size() - 1) ? parent->children[child_idx_in_parent + 1] : nullptr;

    // Try borrowing from right sibling first
    if (right_sibling && right_sibling->keys.size() > right_sibling->getMinKeys()) {
        borrowFromRightLeaf(node, right_sibling, parent, child_idx_in_parent); // parent_key_idx is child_idx_in_parent
    }
    // Try borrowing from left sibling
    else if (left_sibling && left_sibling->keys.size() > left_sibling->getMinKeys()) {
        borrowFromLeftLeaf(node, left_sibling, parent, child_idx_in_parent - 1); // parent_key_idx is child_idx_in_parent - 1
    }
    // Merge with a sibling
    else {
        if (right_sibling) {
            // Merge node with right sibling (node becomes left node in merge)
            mergeLeafNodes(node, right_sibling, parent, child_idx_in_parent);
        } else if (left_sibling) {
            // Merge left sibling with node (node becomes right node in merge)
            mergeLeafNodes(left_sibling, node, parent, child_idx_in_parent - 1);
        } else {
            // Should not happen in a valid B+ tree unless root has < t keys
            std::cerr << "Error: Cannot borrow or merge leaf node - no suitable siblings." << std::endl;
        }
    }
}

// Handle underflow in an internal node
void BPTree::handleInternalUnderflow(BPTreeNode* node, BPTreeNode* parent, int child_idx_in_parent) {
     BPTreeNode* left_sibling = (child_idx_in_parent > 0) ? parent->children[child_idx_in_parent - 1] : nullptr;
    BPTreeNode* right_sibling = (child_idx_in_parent < parent->children.size() - 1) ? parent->children[child_idx_in_parent + 1] : nullptr;

    // Try borrowing from right sibling first
    if (right_sibling && right_sibling->keys.size() > right_sibling->getMinKeys()) {
        borrowFromRightInternal(node, right_sibling, parent, child_idx_in_parent); // parent_key_idx is child_idx_in_parent
    }
    // Try borrowing from left sibling
    else if (left_sibling && left_sibling->keys.size() > left_sibling->getMinKeys()) {
        borrowFromLeftInternal(node, left_sibling, parent, child_idx_in_parent - 1); // parent_key_idx is child_idx_in_parent - 1
    }
    // Merge with a sibling
    else {
        if (right_sibling) {
            // Merge node with right sibling
            mergeInternalNodes(node, right_sibling, parent, child_idx_in_parent);
        } else if (left_sibling) {
            // Merge left sibling with node
            mergeInternalNodes(left_sibling, node, parent, child_idx_in_parent - 1);
        } else {
             std::cerr << "Error: Cannot borrow or merge internal node - no suitable siblings." << std::endl;
        }
    }
}


// --- Leaf Borrow/Merge Implementations ---

void BPTree::borrowFromRightLeaf(BPTreeNode* node, BPTreeNode* right_sibling, BPTreeNode* parent, int parent_key_idx) {
    // Move first key from right sibling to end of node
    node->keys.push_back(right_sibling->keys[0]);
    right_sibling->keys.erase(right_sibling->keys.begin());

    // Update parent's separator key
    // The new separator is the *new* first key of the right sibling
    parent->keys[parent_key_idx] = right_sibling->keys[0];
}

void BPTree::borrowFromLeftLeaf(BPTreeNode* node, BPTreeNode* left_sibling, BPTreeNode* parent, int parent_key_idx) {
    // Move last key from left sibling to beginning of node
    node->keys.insert(node->keys.begin(), left_sibling->keys.back());
    left_sibling->keys.pop_back();

    // Update parent's separator key
    // The new separator is the first key of the current node (which just received the key)
    parent->keys[parent_key_idx] = node->keys[0];
}

void BPTree::mergeLeafNodes(BPTreeNode* left_node, BPTreeNode* right_node, BPTreeNode* parent, int parent_key_idx) {
    // Move all keys from right_node to left_node
    left_node->keys.insert(left_node->keys.end(), right_node->keys.begin(), right_node->keys.end());

    // Update linked list pointer
    left_node->next = right_node->next;

    // Remove separator key and right child pointer from parent
    parent->keys.erase(parent->keys.begin() + parent_key_idx);
    parent->children.erase(parent->children.begin() + parent_key_idx + 1); // Remove pointer to right_node

    // Delete the now empty right node
    delete right_node;

    // Parent might underflow now, handle recursively AFTER merge is complete
    // Note: The check for parent underflow is handled when returning up the recursion in removeFromNode
}


// --- Internal Borrow/Merge Implementations ---

void BPTree::borrowFromRightInternal(BPTreeNode* node, BPTreeNode* right_sibling, BPTreeNode* parent, int parent_key_idx) {
    // Move separator key from parent down to end of node's keys
    node->keys.push_back(parent->keys[parent_key_idx]);

    // Move first key from right sibling up to parent (replace old separator)
    parent->keys[parent_key_idx] = right_sibling->keys[0];
    right_sibling->keys.erase(right_sibling->keys.begin());

    // Move first child from right sibling to end of node's children
    node->children.push_back(right_sibling->children[0]);
    right_sibling->children.erase(right_sibling->children.begin());
}

void BPTree::borrowFromLeftInternal(BPTreeNode* node, BPTreeNode* left_sibling, BPTreeNode* parent, int parent_key_idx) {
    // Move separator key from parent down to beginning of node's keys
    node->keys.insert(node->keys.begin(), parent->keys[parent_key_idx]);

    // Move last key from left sibling up to parent (replace old separator)
    parent->keys[parent_key_idx] = left_sibling->keys.back();
    left_sibling->keys.pop_back();

    // Move last child from left sibling to beginning of node's children
    node->children.insert(node->children.begin(), left_sibling->children.back());
    left_sibling->children.pop_back();
}

void BPTree::mergeInternalNodes(BPTreeNode* left_node, BPTreeNode* right_node, BPTreeNode* parent, int parent_key_idx) {
    // Move separator key from parent down to end of left_node's keys
    left_node->keys.push_back(parent->keys[parent_key_idx]);

    // Move all keys from right_node to left_node
    left_node->keys.insert(left_node->keys.end(), right_node->keys.begin(), right_node->keys.end());

    // Move all children from right_node to left_node
    left_node->children.insert(left_node->children.end(), right_node->children.begin(), right_node->children.end());

    // Remove separator key and right child pointer from parent
    parent->keys.erase(parent->keys.begin() + parent_key_idx);
    parent->children.erase(parent->children.begin() + parent_key_idx + 1); // Remove pointer to right_node

    // Delete the now empty right node
    // Clear children first to prevent double deletion if destructor tried deleting them
    right_node->keys.clear();
    right_node->children.clear();
    delete right_node;

    // Parent might underflow now, handle recursively
    // Note: The check for parent underflow is handled when returning up the recursion in removeFromNode
}


// --- Printing Logic ---

void BPTree::printTreeShape() const {
    if (root == nullptr) {
        std::cout << "B+ Tree is empty." << std::endl;
        return;
    }
    std::cout << "B+ Tree Structure (t=" << t << ", minKeys=" << (t-1) << ", maxKeys=" << (2*t-1) << "):" << std::endl;
    std::cout << "------------------------" << std::endl;
    printTreeShapeRecursive(root, 0, "");
    std::cout << "------------------------" << std::endl;
}

void BPTree::printTreeShapeRecursive(const BPTreeNode* node, int level, const std::string& prefix) const {
    if (node == nullptr) return;

    // Print node's keys
    std::cout << prefix << "[";
    for (size_t i = 0; i < node->keys.size(); ++i) {
        std::cout << node->keys[i] << (i == node->keys.size() - 1 ? "" : ", ");
    }
    std::cout << "]";
    if (node->leaf) {
        std::cout << " (Leaf)";
        if (node->next) {
             std::cout << " -> next=[";
             if (!node->next->keys.empty()) std::cout << node->next->keys[0] << "...";
             std::cout << "]";
        } else {
            std::cout << " -> NULL";
        }
    } else {
         std::cout << " (Internal)";
    }
     std::cout << " Addr:" << node << std::endl; // Print address for debugging merges/splits


    // Recursively print children for internal nodes
    if (!node->leaf) {
        std::string childPrefixBase = prefix + "  ";
        std::string connector = "|--";
        for (size_t i = 0; i < node->children.size(); ++i) {
             std::string currentPrefix = childPrefixBase;
             if (i == node->children.size() - 1) { // Last child
                 connector = "`--";
                 currentPrefix = prefix + "   "; // Adjust indentation for last child line
             } else {
                 connector = "|--";
                 currentPrefix = prefix + "  |"; // Vertical line for non-last children
             }

            // Print connector
            std::cout << currentPrefix << connector;

            // Recursive call
            printTreeShapeRecursive(node->children[i], level + 1, prefix + (i == node->children.size() - 1 ? "    " : "  | "));
        }
    }
}


//-----------------------------------------------------
// Driver Code (main function)
//-----------------------------------------------------
int main() {
    try {
        // Create a B+ Tree with minimum degree 2 (t=2 => max 3 keys, max 4 children, min 1 key)
        BPTree tree(2);

        std::cout << "--- Insertion Phase ---" << std::endl;
        std::vector<int> keys_to_insert = {10, 20, 5, 6, 12, 30, 7, 17, 3, 1, 15, 18, 25, 28, 35};
        for (int k : keys_to_insert) {
            std::cout << "\nInserting " << k << "..." << std::endl;
            tree.insert(k);
            tree.printTreeShape();
            tree.printLeaves();
        }

        std::cout << "\n--- Final Tree after Insertions ---" << std::endl;
        tree.printTreeShape();
        tree.printLeaves();

        std::cout << "\n--- Search Phase ---" << std::endl;
        std::vector<int> keys_to_search = {7, 15, 30, 100, 1, 35, 11};
        for (int k : keys_to_search) {
            std::cout << "Searching for " << k << ": " << (tree.search(k) ? "Found" : "Not Found") << std::endl;
        }

        // --- Deletion Phase ---
        std::cout << "\n--- Deletion Phase ---" << std::endl;
        // Keys chosen to trigger various borrow/merge scenarios
        std::vector<int> keys_to_delete = {
            7,  // Simple leaf delete
            15, // Leaf delete, potential borrow/merge
            18, // Leaf delete, potential borrow/merge
            35, // Leaf delete (rightmost)
            30, // Leaf delete, causes underflow, merge?
            28, // Leaf delete, causes underflow, borrow?
            25, // Leaf delete, causes underflow, merge?
            5,  // Delete causing internal node changes
            6,  // Delete causing internal node changes
            17, // Delete causing internal node changes
            12, // Delete causing root changes?
            10, // Delete causing root changes?
            20, // Delete last few
            3,
            1
        };

         for (int k : keys_to_delete) {
             std::cout << "\nAttempting to delete " << k << "..." << std::endl;
             tree.remove(k);
             tree.printTreeShape();
             tree.printLeaves();
         }

         std::cout << "\n--- Final State after Deletions ---" << std::endl;
         tree.printTreeShape();
         tree.printLeaves();

         // Test deleting non-existent key
         std::cout << "\nAttempting to delete 999 (non-existent)..." << std::endl;
         tree.remove(999);
         tree.printTreeShape();
         tree.printLeaves();

         // Test deleting from empty tree
         std::cout << "\nDeleting remaining keys..." << std::endl;
         // Add code to delete any remaining keys if necessary to test empty tree deletion
         // tree.remove(remaining_key); ...
         std::cout << "\nAttempting to delete from empty tree..." << std::endl;
         tree.remove(10); // Try deleting again after it should be empty


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
