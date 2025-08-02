/**
 * @file treap.cpp
 * @brief A comprehensive implementation of the Treap data structure
 *
 * This file implements a Treap data structure, which is a randomized binary search tree
 * that combines the properties of a binary search tree (BST) and a heap.
 * - BST property: The keys follow in-order traversal (left < node < right)
 * - Heap property: Each node has a priority, with parent priority > child priority
 *
 * Time complexities:
 * - Insert: O(log n) average case
 * - Delete: O(log n) average case
 * - Search: O(log n) average case
 * - Split: O(log n)
 * - Merge: O(log n)
 */

 #include <iostream>
 #include <random>
 #include <string>   // For printing prefixes
 #include <algorithm> // For std::max in getHeight
 #include <stdexcept> // For runtime_error in findMin/Max
 
 // Random number generator for priorities
 std::random_device rd;
 std::mt19937 gen(rd());
 // Increased range slightly for potentially better distribution if many nodes are inserted
 std::uniform_int_distribution<int> dist(1, 1000000);
 
 /**
  * @class TreapNode
  * @brief Node structure for the Treap
  */
 class TreapNode {
 public:
     int key;            // Value stored in the node (fulfills BST property)
     int priority;       // Random priority (fulfills heap property)
     TreapNode* left;    // Pointer to left child
     TreapNode* right;   // Pointer to right child
 
     /**
      * @brief Constructor for TreapNode
      * @param key The key value to be stored
      * @param priority The priority value (random if not specified)
      */
     TreapNode(int key, int priority = -1) {
         this->key = key;
         // If priority not specified, generate a random one
         this->priority = (priority == -1) ? dist(gen) : priority;
         this->left = nullptr;
         this->right = nullptr;
     }
 };
 
 /**
  * @class Treap
  * @brief Implementation of Treap data structure
  */
 class Treap {
 private:
     TreapNode* root;  // Root of the Treap
 
     /**
      * @brief Perform a right rotation at the given node
      * @param y Node to rotate around
      * @return New root after rotation
      *
      *     y          x
      *    / \        / \
      *   x   C  =>  A   y
      *  / \            / \
      * A   B          B   C
      */
     TreapNode* rightRotate(TreapNode* y) {
         if (!y || !y->left) return y; // Cannot rotate if y or left child is null
         TreapNode* x = y->left;
         TreapNode* B = x->right;
 
         // Perform rotation
         x->right = y;
         y->left = B;
 
         // Return new root
         return x;
     }
 
     /**
      * @brief Perform a left rotation at the given node
      * @param x Node to rotate around
      * @return New root after rotation
      *
      *   x              y
      *  / \            / \
      * A   y    =>    x   C
      *    / \        / \
      *   B   C      A   B
      */
     TreapNode* leftRotate(TreapNode* x) {
         if (!x || !x->right) return x; // Cannot rotate if x or right child is null
         TreapNode* y = x->right;
         TreapNode* B = y->left;
 
         // Perform rotation
         y->left = x;
         x->right = B;
 
         // Return new root
         return y;
     }
 
     /**
      * @brief Helper function to insert a key with given priority into the Treap
      * @param currentRoot Root of current subtree
      * @param key Key to insert
      * @param priority Priority for the new key
      * @return Root of the subtree after insertion
      */
     TreapNode* insert(TreapNode* currentRoot, int key, int priority = -1) {
         // Base case: If tree is empty, return a new node
         if (currentRoot == nullptr) {
             // Generate priority here if needed, ensuring it's always set
             int nodePriority = (priority == -1) ? dist(gen) : priority;
             return new TreapNode(key, nodePriority);
         }
 
         // BST insert: If key is smaller, insert into left subtree
         if (key < currentRoot->key) {
             currentRoot->left = insert(currentRoot->left, key, priority);
 
             // Fix heap property: If left child has higher priority than root
             if (currentRoot->left != nullptr && currentRoot->left->priority > currentRoot->priority) {
                 currentRoot = rightRotate(currentRoot);
             }
         }
         // BST insert: If key is larger, insert into right subtree
         else if (key > currentRoot->key) {
             currentRoot->right = insert(currentRoot->right, key, priority);
 
             // Fix heap property: If right child has higher priority than root
             if (currentRoot->right != nullptr && currentRoot->right->priority > currentRoot->priority) {
                 currentRoot = leftRotate(currentRoot);
             }
         }
         // If key already exists, we don't insert a duplicate (could be modified as needed)
         // else { std::cerr << "Warning: Key " << key << " already exists. Insertion ignored." << std::endl; }
 
 
         return currentRoot;
     }
 
     /**
      * @brief Helper function to search for a key in the Treap
      * @param currentRoot Root of current subtree
      * @param key Key to search for
      * @return Node containing the key, or nullptr if not found
      */
     TreapNode* search(TreapNode* currentRoot, int key) {
         // Base case: root is null or key found
         if (currentRoot == nullptr || currentRoot->key == key) {
             return currentRoot;
         }
 
         // If key is smaller than root's key, search in left subtree
         if (key < currentRoot->key) {
             return search(currentRoot->left, key);
         }
 
         // If key is greater than root's key, search in right subtree
         return search(currentRoot->right, key);
     }
 
     /**
      * @brief Helper function to delete a node with the given key
      * @param currentRoot Root of current subtree
      * @param key Key to delete
      * @return Root of the subtree after deletion
      */
     TreapNode* remove(TreapNode* currentRoot, int key) {
         // Base case: If tree is empty
         if (currentRoot == nullptr) {
             return currentRoot;
         }
 
         // Search for the key
         if (key < currentRoot->key) {
             // Key is in left subtree
             currentRoot->left = remove(currentRoot->left, key);
         }
         else if (key > currentRoot->key) {
             // Key is in right subtree
             currentRoot->right = remove(currentRoot->right, key);
         }
         // Found the key
         else {
             // Case 1 & 2: Node with only one child or no child
             if (currentRoot->left == nullptr) {
                 TreapNode* temp = currentRoot->right;
                 delete currentRoot;
                 currentRoot = temp; // Becomes nullptr if right was also null
             }
             else if (currentRoot->right == nullptr) {
                 TreapNode* temp = currentRoot->left;
                 delete currentRoot;
                 currentRoot = temp;
             }
             // Case 3: Node with two children - rotate the higher priority child up
             else {
                 if (currentRoot->left->priority > currentRoot->right->priority) {
                     currentRoot = rightRotate(currentRoot);
                     // After rotation, the node to delete is now in the right subtree
                     currentRoot->right = remove(currentRoot->right, key);
                 }
                 else {
                     currentRoot = leftRotate(currentRoot);
                     // After rotation, the node to delete is now in the left subtree
                     currentRoot->left = remove(currentRoot->left, key);
                 }
             }
         }
         return currentRoot; // Return the potentially new root of this subtree
     }
 
 
     /**
      * @brief Helper function for splitting a Treap into two subtrees
      * @param currentRoot Root of the Treap to split
      * @param key Split key - all nodes with keys <= key go to left, others to right
      * @param left Reference to root of left subtree
      * @param right Reference to root of right subtree
      */
     void split(TreapNode* currentRoot, int key, TreapNode*& left, TreapNode*& right) {
         if (currentRoot == nullptr) {
             left = nullptr;
             right = nullptr;
             return;
         }
 
         // Create a temporary node to insert for splitting logic if needed,
         // or use existing logic which seems correct. Let's stick to the existing logic.
 
         if (key >= currentRoot->key) {
             // Current node belongs to the left treap.
             // Its left child also belongs to the left treap.
             // Its right child might contain nodes for both left and right treaps.
             split(currentRoot->right, key, currentRoot->right, right);
             left = currentRoot; // currentRoot is the root of the resulting left treap segment
         }
         else {
             // Current node belongs to the right treap.
             // Its right child also belongs to the right treap.
             // Its left child might contain nodes for both left and right treaps.
             split(currentRoot->left, key, left, currentRoot->left);
             right = currentRoot; // currentRoot is the root of the resulting right treap segment
         }
     }
 
 
     /**
      * @brief Helper function for merging two Treaps
      * @param left Root of left Treap (all keys assumed < all keys in right)
      * @param right Root of right Treap (all keys assumed > all keys in left)
      * @return Root of the merged Treap
      */
     TreapNode* merge(TreapNode* left, TreapNode* right) {
         // Base cases
         if (left == nullptr) return right;
         if (right == nullptr) return left;
 
         // If left's priority is higher, it becomes the root of merged Treap
         if (left->priority > right->priority) {
             // Left root stays root, merge its right child with the right treap
             left->right = merge(left->right, right);
             return left;
         }
         // If right's priority is higher, it becomes the root of merged Treap
         else {
             // Right root stays root, merge its left child with the left treap
             right->left = merge(left, right->left);
             return right;
         }
     }
 
     /**
      * @brief Helper function to get the minimum key in a Treap
      * @param currentRoot Root of the Treap
      * @return The minimum key
      * @throws std::runtime_error if the Treap is empty
      */
     int findMin(TreapNode* currentRoot) {
         // The leftmost node contains the minimum key
         if (currentRoot == nullptr) {
             throw std::runtime_error("Cannot find minimum in empty Treap");
         }
 
         TreapNode* current = currentRoot;
         while (current->left != nullptr) {
             current = current->left;
         }
         return current->key;
     }
 
     /**
      * @brief Helper function to get the maximum key in a Treap
      * @param currentRoot Root of the Treap
      * @return The maximum key
      * @throws std::runtime_error if the Treap is empty
      */
     int findMax(TreapNode* currentRoot) {
         // The rightmost node contains the maximum key
         if (currentRoot == nullptr) {
             throw std::runtime_error("Cannot find maximum in empty Treap");
         }
 
         TreapNode* current = currentRoot;
         while (current->right != nullptr) {
             current = current->right;
         }
         return current->key;
     }
 
     /**
      * @brief Helper function to clear the Treap (free all memory)
      * @param currentRoot Root of the subtree to clear
      */
     void clear(TreapNode* currentRoot) {
         if (currentRoot == nullptr) {
             return;
         }
 
         // Post-order traversal to delete all nodes
         clear(currentRoot->left);
         clear(currentRoot->right);
         delete currentRoot;
     }
 
     /**
      * @brief Helper function to print tree in-order
      * @param currentRoot Root of current subtree
      */
     void inOrderTraversal(TreapNode* currentRoot) {
         if (currentRoot != nullptr) {
             inOrderTraversal(currentRoot->left);
             std::cout << currentRoot->key << "(p:" << currentRoot->priority << ") ";
             inOrderTraversal(currentRoot->right);
         }
     }
 
     /**
      * @brief Helper function to get the height of a node
      * @param node Node to find height of
      * @return Height of the node (number of edges on longest path)
      */
     int getHeightRecursive(TreapNode* node) {
         if (node == nullptr) {
             return -1; // Height of empty subtree is -1
         }
         return 1 + std::max(getHeightRecursive(node->left), getHeightRecursive(node->right));
     }
 
 
     /**
      * @brief Recursive helper function to print the tree structure with ASCII connectors
      * @param node The current node
      * @param prefix The prefix string for drawing lines
      * @param isLeft True if the current node is a left child, false otherwise
      */
     void printTreapRecursive(TreapNode* node, const std::string& prefix, bool isLeft) {
         if (node == nullptr) {
             return;
         }
 
         // Print the right subtree first (it will appear above the current node)
         // Use '|' for vertical lines
         printTreapRecursive(node->right, prefix + (isLeft ? "|   " : "    "), false);
 
         // Print the current node connection and value
         std::cout << prefix;
         // Use '\' for left child corner, ''' for right child corner, '-' for horizontal lines
         std::cout << (isLeft ? "\\-- " : "'-- "); // Use ASCII characters
         std::cout << node->key << "(" << node->priority << ")" << std::endl;
 
         // Print the left subtree (it will appear below the current node)
         // Use '|' for vertical lines
         printTreapRecursive(node->left, prefix + (isLeft ? "    " : "|   "), true);
     }
 
 
 public:
     /**
      * @brief Constructor for Treap
      */
     Treap() : root(nullptr) {} // Use initializer list
 
     /**
      * @brief Destructor for Treap
      */
     ~Treap() {
         clear(root);
     }
 
     // Disable copy constructor and assignment operator to prevent shallow copies
     Treap(const Treap&) = delete;
     Treap& operator=(const Treap&) = delete;
 
     // Allow move constructor and assignment operator (optional but good practice)
     Treap(Treap&& other) noexcept : root(other.root) {
         other.root = nullptr; // Prevent double deletion
     }
     Treap& operator=(Treap&& other) noexcept {
         if (this != &other) {
             clear(root); // Clear current resources
             root = other.root;
             other.root = nullptr; // Prevent double deletion
         }
         return *this;
     }
 
 
     /**
      * @brief Insert a key into the Treap
      * @param key Key to insert
      * @param priority Priority for the new key (optional, random by default)
      */
     void insert(int key, int priority = -1) {
         // Pass the explicit priority if provided, otherwise let the helper handle -1
         root = insert(root, key, priority);
     }
 
     /**
      * @brief Remove a key from the Treap
      * @param key Key to remove
      */
     void remove(int key) {
         root = remove(root, key);
     }
 
     /**
      * @brief Search for a key in the Treap
      * @param key Key to search for
      * @return true if found, false otherwise
      */
     bool search(int key) {
         return search(root, key) != nullptr;
     }
 
     /**
      * @brief Get the minimum key in the Treap
      * @return The minimum key
      * @throws std::runtime_error if the Treap is empty
      */
     int getMin() {
         // No need to check for empty here, findMin helper does it
         return findMin(root);
     }
 
     /**
      * @brief Get the maximum key in the Treap
      * @return The maximum key
      * @throws std::runtime_error if the Treap is empty
      */
     int getMax() {
         // No need to check for empty here, findMax helper does it
         return findMax(root);
     }
 
     /**
      * @brief Check if the Treap is empty
      * @return true if empty, false otherwise
      */
     bool isEmpty() const { // Mark as const as it doesn't modify the treap
         return root == nullptr;
     }
 
     /**
      * @brief Get the height of the Treap
      * @return Height of the Treap (number of edges on longest path from root to leaf)
      */
     int getHeight() {
         return getHeightRecursive(root);
     }
 
     /**
      * @brief Print the Treap elements in-order
      */
     void printInOrder() {
         if (isEmpty()) {
             std::cout << "In-order traversal: (Empty Treap)" << std::endl;
             return;
         }
         std::cout << "In-order traversal: ";
         inOrderTraversal(root);
         std::cout << std::endl;
     }
 
     /**
      * @brief Print the Treap structure vertically with ASCII connectors
      */
     void printTreap() {
         if (root == nullptr) {
             std::cout << "Treap structure: (Empty Treap)" << std::endl;
             return;
         }
         std::cout << "Treap structure:" << std::endl;
         // Start the recursive print from the root. Pass "" prefix and false for isLeft.
         printTreapRecursive(root, "", false);
         std::cout << std::endl; // Add a final newline for spacing
     }
 
 
     /**
      * @brief Split the Treap at the given key
      * @param key Split key. Nodes with key <= key go to leftTreap, others to rightTreap.
      * @param leftTreap Reference to the Treap that will hold keys <= key.
      * @param rightTreap Reference to the Treap that will hold keys > key.
      * @note The original Treap becomes empty after the split.
      */
     void splitTreap(int key, Treap& leftTreap, Treap& rightTreap) {
         // Ensure the target treaps are empty before splitting into them
         leftTreap.clear(leftTreap.root);
         leftTreap.root = nullptr;
         rightTreap.clear(rightTreap.root);
         rightTreap.root = nullptr;
 
         TreapNode* leftRoot = nullptr;
         TreapNode* rightRoot = nullptr;
 
         // Split the current treap's root
         split(root, key, leftRoot, rightRoot);
 
         // Assign the resulting roots to the target treaps
         leftTreap.root = leftRoot;
         rightTreap.root = rightRoot;
 
         // The original treap is now empty, as its nodes have been moved
         root = nullptr;
     }
 
     /**
      * @brief Merge this Treap with another Treap
      * @param other The other Treap to merge with.
      * @pre All keys in this Treap must be less than all keys in the `other` Treap.
      * @note The `other` Treap becomes empty after the merge.
      */
     void mergeTreap(Treap& other) {
         // Add a check for the precondition (optional, depends on use case)
         // if (!isEmpty() && !other.isEmpty() && getMax() >= other.getMin()) {
         //     throw std::runtime_error("Merge precondition violated: Max key of left treap >= Min key of right treap.");
         // }
 
         // Merge the roots
         root = merge(root, other.root);
 
         // The other treap has given up its nodes
         other.root = nullptr;
     }
 };
 
 /**
  * @brief Main function demonstrating Treap operations
  */
 int main() {
     Treap treap;
 
     // Insert some values with specific priorities for demonstration
     // Using fixed priorities for reproducible example
     std::cout << "Inserting elements with priorities..." << std::endl;
     treap.insert(50, 80);  // Key: 50, Priority: 80
     treap.insert(30, 70);  // Key: 30, Priority: 70
     treap.insert(70, 75);  // Key: 70, Priority: 75
     treap.insert(20, 65);  // Key: 20, Priority: 65
     treap.insert(40, 60);  // Key: 40, Priority: 60
     treap.insert(60, 55);  // Key: 60, Priority: 55
     treap.insert(80, 50);  // Key: 80, Priority: 50
     treap.insert(90, 90);  // Add one more for structure
     treap.insert(10, 85);  // Add one more for structure
 
     // Print the treap in-order
     treap.printInOrder();
     std::cout << std::endl; // Add spacing
 
     // Print the treap structure using the improved method
     treap.printTreap();
 
     // Search for elements
     std::cout << "\nSearching for elements:" << std::endl;
     std::cout << "Is 30 in the treap? " << (treap.search(30) ? "Yes" : "No") << std::endl;
     std::cout << "Is 100 in the treap? " << (treap.search(100) ? "Yes" : "No") << std::endl;
 
     // Find min and max
     try {
         std::cout << "\nMin element: " << treap.getMin() << std::endl;
         std::cout << "Max element: " << treap.getMax() << std::endl;
         std::cout << "Height: " << treap.getHeight() << std::endl;
     } catch (const std::runtime_error& e) {
         std::cerr << "Error: " << e.what() << std::endl;
     }
 
 
     // Remove an element
     std::cout << "\nRemoving element 30..." << std::endl;
     treap.remove(30);
     treap.printInOrder();
     treap.printTreap();
 
     std::cout << "\nRemoving element 50 (root)..." << std::endl;
     treap.remove(50);
     treap.printInOrder();
     treap.printTreap();
 
     // Demonstrate split and merge
     std::cout << "\nSplitting treap at key 60..." << std::endl;
     Treap leftTreap, rightTreap;
     // Note: The original 'treap' will become empty after splitTreap
     treap.splitTreap(60, leftTreap, rightTreap);
 
     std::cout << "--- Left Treap (<= 60) ---" << std::endl;
     leftTreap.printInOrder();
     leftTreap.printTreap();
 
     std::cout << "--- Right Treap (> 60) ---" << std::endl;
     rightTreap.printInOrder();
     rightTreap.printTreap();
 
     std::cout << "--- Original Treap (should be empty) ---" << std::endl;
     treap.printInOrder();
     treap.printTreap();
 
 
     // Merge back into leftTreap
     std::cout << "\nMerging treaps back into Left Treap..." << std::endl;
     leftTreap.mergeTreap(rightTreap); // rightTreap becomes empty
 
     std::cout << "--- Merged Treap (in leftTreap) ---" << std::endl;
     leftTreap.printInOrder();
     leftTreap.printTreap();
 
     std::cout << "--- Right Treap (should be empty) ---" << std::endl;
     rightTreap.printInOrder();
     rightTreap.printTreap();
 
 
     return 0;
 }
 