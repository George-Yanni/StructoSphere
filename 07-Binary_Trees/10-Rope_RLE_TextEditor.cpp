#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>
#include <vector>

// RopeNode represents a node in the Rope's binary tree.
// - Leaf nodes store a string piece (value) and have no children.
// - Internal nodes represent concatenation points and have left/right children.
// - weight: For leaves, the string length; for internal nodes, the length of the left subtree.
struct RopeNode {
    std::unique_ptr<RopeNode> left;   // Left child (nullptr for leaves)
    std::unique_ptr<RopeNode> right;  // Right child (nullptr for leaves)
    std::string value;              // String data (non-empty only for leaves)
    int weight;                     // Length of left subtree (or string length for leaves)

    // Constructor for a leaf node with a string value.
    explicit RopeNode(std::string val)
        : left(nullptr), right(nullptr), value(std::move(val)), weight(static_cast<int>(value.size())) {} // Use static_cast for size_t to int conversion

    // Constructor for an internal node (used internally)
    RopeNode() : left(nullptr), right(nullptr), value(""), weight(0) {}

    // Returns true if this is a leaf node (no children).
    bool is_leaf() const { return !left && !right; }
};

// Rope is a string data structure that uses a binary tree for efficient operations.
// - Each Rope has a root node (or nullptr for an empty Rope).
// - Operations like concat, split, and indexing are optimized by manipulating the tree.
class Rope {
private:
    // Private constructor for creating a Rope from a node (used internally by split/clone).
    // Takes ownership of the node.
    Rope(std::unique_ptr<RopeNode> node) : root(std::move(node)) {}

public:
    // Default constructor: Creates an empty Rope.
    Rope() = default;

    // Constructs a Rope from a string (single leaf node).
    explicit Rope(const std::string& str) {
        if (!str.empty()) {
            root = std::make_unique<RopeNode>(str);
        }
    }

    // Delete copy constructor and copy assignment to prevent accidental copying.
    Rope(const Rope&) = delete;
    Rope& operator=(const Rope&) = delete;

    // Allow move constructor and move assignment for efficient ownership transfer.
    Rope(Rope&&) = default;
    Rope& operator=(Rope&&) = default;

    // Creates a deep copy (clone) of this Rope.
    Rope clone() const {
        return Rope(clone_node(root.get())); // Use private constructor
    }

    // Concatenates this Rope with another, creating a new Rope.
    // Takes other by value and moves from it if possible, otherwise clones.
    // This version is more flexible and potentially avoids a clone if 'other' is an rvalue.
    Rope concat(Rope other) const {
        Rope result;
        std::unique_ptr<RopeNode> left_clone = clone_node(root.get());
        std::unique_ptr<RopeNode> right_node = std::move(other.root); // Take ownership from other

        if (!left_clone) {
            result.root = std::move(right_node);
            return result;
        }
        if (!right_node) {
            result.root = std::move(left_clone);
            return result;
        }

        result.root = std::make_unique<RopeNode>(); // Use internal node constructor
        result.root->left = std::move(left_clone);
        result.root->right = std::move(right_node);
        result.root->weight = compute_total_weight(result.root->left.get()); // Weight is total length of left subtree
        return result;
    }

    // Splits the Rope at the given index, returning two new Ropes.
    std::pair<Rope, Rope> split(int index) const {
        if (index < 0 || index > length()) {
            throw std::out_of_range("Split index out of range");
        }
        // split_node needs to operate on a clone if we want the original rope to remain unchanged
        // However, the common use case (like in TextEditor) is to replace the original
        // with the split parts. Let's assume split modifies the original conceptually
        // and returns the parts. If immutability is strictly required, clone first.
        // For the TextEditor use case, we consume the original rope.
        // Let's refine split_node to take ownership and return ownership.

        // *** Option 1: Split modifies the original (more efficient for editor) ***
        // auto [left, right] = split_node_destructive(std::move(root), index);
        // return {Rope(std::move(left)), Rope(std::move(right))};
        // Requires changing split_node signature and implementation

        // *** Option 2: Split is const, creates new ropes (safer, less efficient if original is discarded) ***
        auto [left, right] = split_node_cloning(root.get(), index);
        return {Rope(std::move(left)), Rope(std::move(right))};
    }

    // Returns the character at the given position.
    char at(int pos) const {
        if (!root) {
             throw std::out_of_range("Index out of range (empty rope)");
        }
        int len = length(); // Cache length calculation
        if (pos < 0 || pos >= len) {
            throw std::out_of_range("Index out of range");
        }
        return char_at(root.get(), pos);
    }

    // Reconstructs the full string by traversing the tree.
    std::string toString() const {
        std::string result;
        if (root) {
            // Pre-allocate string capacity for efficiency if length is known
            result.reserve(length());
            build_string(root.get(), result);
        }
        return result;
    }

    // Prints the tree structure for debugging.
    void printTree() const {
        print_node(root.get(), 0);
    }

    // Returns the total length of the Rope's string.
    int length() const {
        // Optimization: Store total length in the root or Rope class if frequently needed
        return compute_total_weight(root.get());
    }

private:
    std::unique_ptr<RopeNode> root; // Root of the binary tree (nullptr if empty)

    // Computes the total length of the string represented by a subtree.
    // Renamed from compute_weight to avoid confusion with node->weight
    static int compute_total_weight(const RopeNode* node) {
        if (!node) return 0;
        // For internal nodes, the total weight is the sum of children's total weights.
        // The node->weight specifically stores the *left* subtree's total weight.
        if (node->is_leaf()) return node->weight; // Leaf weight is its length
        // This recursive calculation can be inefficient if called often.
        // Consider storing total length if needed frequently.
        return compute_total_weight(node->left.get()) + compute_total_weight(node->right.get());
        // Alternatively, if the tree structure guarantees weight property:
        // return node->weight + compute_total_weight(node->right.get()); // Assumes weight is correctly maintained
    }

    // Finds the character at position pos in the subtree rooted at node.
    static char char_at(const RopeNode* node, int pos) {
        // Added null check for safety, though should be handled by caller 'at'
        if (!node) throw std::logic_error("char_at called on null node");

        if (node->is_leaf()) {
             if (pos < 0 || pos >= node->weight) {
                 // This should ideally not happen if initial checks are correct
                 throw std::logic_error("Invalid index within leaf node");
             }
            return node->value[pos];
        }

        // Internal node: check weight to decide which subtree to descend
        if (pos < node->weight) {
            return char_at(node->left.get(), pos);
        } else {
            // Adjust index for the right subtree
            return char_at(node->right.get(), pos - node->weight);
        }
    }

    // Builds the full string by appending leaf values to output in-order.
    static void build_string(const RopeNode* node, std::string& output) {
        if (!node) return;
        // In-order traversal
        build_string(node->left.get(), output);
        if (node->is_leaf()) {
            output += node->value;
        }
        build_string(node->right.get(), output);
    }

    // Splits the subtree represented by node at the given index.
    // This version creates new nodes (cloning parts) to ensure the original node is unchanged.
    static std::pair<std::unique_ptr<RopeNode>, std::unique_ptr<RopeNode>>
    split_node_cloning(const RopeNode* node, int index) {
        if (!node) return {nullptr, nullptr};

        if (node->is_leaf()) {
            // Split the leaf string into two new leaf nodes
            std::unique_ptr<RopeNode> left = nullptr;
            if (index > 0) {
                left = std::make_unique<RopeNode>(node->value.substr(0, index));
            }
            std::unique_ptr<RopeNode> right = nullptr;
            if (index < node->weight) {
                 right = std::make_unique<RopeNode>(node->value.substr(index));
            }
            return {std::move(left), std::move(right)};
        }

        // Internal node split
        if (index < node->weight) {
            // Split occurs in the left subtree
            auto [ll, lr] = split_node_cloning(node->left.get(), index); // Split left child

            // The new right part consists of the right part of the left split (lr)
            // concatenated with a clone of the original right child.
            auto right = std::make_unique<RopeNode>();
            right->left = std::move(lr);
            right->right = clone_node(node->right.get()); // Clone original right
            right->weight = compute_total_weight(right->left.get());

            // The new left part is just the left part of the left split (ll)
            return {std::move(ll), std::move(right)};
        } else if (index > node->weight) {
             // Split occurs in the right subtree
            auto [rl, rr] = split_node_cloning(node->right.get(), index - node->weight); // Split right child

            // The new left part consists of a clone of the original left child
            // concatenated with the left part of the right split (rl).
            auto left = std::make_unique<RopeNode>();
            left->left = clone_node(node->left.get()); // Clone original left
            left->right = std::move(rl);
            left->weight = compute_total_weight(left->left.get()); // Original left weight

            // The new right part is just the right part of the right split (rr)
            return {std::move(left), std::move(rr)};
        } else { // index == node->weight
            // Split occurs exactly between left and right children
            // Return clones of the original children
            return {clone_node(node->left.get()), clone_node(node->right.get())};
        }
    }

    // Creates a deep copy of a subtree.
    static std::unique_ptr<RopeNode> clone_node(const RopeNode* node) {
        if (!node) return nullptr; // Corrected typo here

        std::unique_ptr<RopeNode> new_node;
        if (node->is_leaf()) {
            // Clone leaf by copying string
            new_node = std::make_unique<RopeNode>(node->value);
        } else {
            // Clone internal node recursively
            new_node = std::make_unique<RopeNode>(); // Use internal node constructor
            new_node->left = clone_node(node->left.get());
            new_node->right = clone_node(node->right.get());
            new_node->weight = node->weight; // Copy weight (length of left subtree)
        }
        // Ensure weight is correct (redundant if constructor/split/concat maintain it)
        // new_node->weight = compute_total_weight(new_node->left.get());
        return new_node;
    }

    // Prints the tree structure with indentation.
    static void print_node(const RopeNode* node, int depth) {
        if (!node) return;
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        if (node->is_leaf()) {
            std::cout << "Leaf: \"" << node->value << "\" (weight/len: " << node->weight << ")\n";
        } else {
            // Also print total weight for clarity if desired
            // int total_w = compute_total_weight(node);
            std::cout << "Internal Node (left weight: " << node->weight << ")\n"; // total weight: " << total_w << ")\n";
        }
        print_node(node->left.get(), depth + 1);
        print_node(node->right.get(), depth + 1);
    }
};

// TextEditor uses a Rope to manage a text document.
class TextEditor {
public:
    explicit TextEditor(const std::string& initial = "") : document(initial) {}

    void insert(int pos, const std::string& text) {
        int current_length = document.length(); // Get length before potential modification
        if (pos < 0 || pos > current_length) {
            throw std::out_of_range("Insert position out of range");
        }
        if (text.empty()) return; // Nothing to insert

        // Clone the current state *before* modification and move it to history
        history.push_back(document.clone());

        // Perform the insertion using split and concat
        // Since split/concat create new Ropes, we need to move the result back
        auto [left, right] = document.split(pos); // Consumes document if split is destructive, otherwise just uses it
        Rope new_text(text);

        // Rebuild the document by concatenating parts
        // Use move semantics for efficiency
        Rope temp_left_concat = left.concat(std::move(new_text));
        document = temp_left_concat.concat(std::move(right));
    }

    void delete_range(int start, int length) {
         int current_length = document.length(); // Get length before potential modification
        if (start < 0 || length < 0 || start > current_length || start + length > current_length) {
             // Check start separately to allow deleting 0 chars at the end
            throw std::out_of_range("Delete range out of range");
        }
        if (length == 0) return; // Nothing to delete

        // Clone the current state *before* modification and move it to history
        history.push_back(document.clone());

        // Perform deletion using split and concat
        auto [left, temp] = document.split(start);
        auto [middle_discarded, right] = temp.split(length); // Discard the middle part

        // Rebuild the document by concatenating left and right parts
        document = left.concat(std::move(right));
    }

    void undo() {
        if (history.empty()) {
            std::cout << "Nothing to undo.\n";
            return;
        }
        // Move the last saved state back to document
        document = std::move(history.back());
        history.pop_back(); // Remove the state from history
    }

    std::string get_content() const {
        return document.toString();
    }

    void print_tree() const {
        std::cout << "Rope Tree Structure (length: " << document.length() << "):\n";
        document.printTree();
    }

private:
    Rope document;
    std::vector<Rope> history; // Stores previous states of the document Rope
};

int main() {
    std::cout << "=== Initializing Text Editor with Code Snippet ===\n";
    TextEditor editor("void main() {\n    // Code here\n}");
    std::cout << "Initial content:\n" << editor.get_content() << "\n";
    std::cout << "Initial tree:\n";
    editor.print_tree();
    std::cout << "\n";

    std::cout << "=== Inserting 'printf(\"Hello\");' at position 20 ===\n";
    // "void main() {\n    // " is 20 chars
    editor.insert(20, "printf(\"Hello\");");
    std::cout << "Content after insertion:\n" << editor.get_content() << "\n";
    std::cout << "Tree after insertion:\n";
    editor.print_tree();
    std::cout << "\n";

    std::cout << "=== Deleting 'printf' (7 chars) at position 20 ===\n";
    editor.delete_range(20, 7); // Delete "printf("
    std::cout << "Content after deletion:\n" << editor.get_content() << "\n";
    std::cout << "Tree after deletion:\n";
    editor.print_tree();
    std::cout << "\n";

    std::cout << "=== Undoing Deletion ===\n";
    editor.undo();
    std::cout << "Content after undo:\n" << editor.get_content() << "\n";
    std::cout << "Tree after undo:\n";
    editor.print_tree();
    std::cout << "\n";

     std::cout << "=== Undoing Insertion ===\n";
    editor.undo();
    std::cout << "Content after second undo:\n" << editor.get_content() << "\n";
    std::cout << "Tree after second undo:\n";
    editor.print_tree();
    std::cout << "\n";

    std::cout << "=== Trying to undo again ===\n";
    editor.undo(); // Should print "Nothing to undo."
    std::cout << "\n";


    // Test edge cases
    std::cout << "=== Edge Case Tests ===\n";
    TextEditor edge_editor("abc");
    std::cout << "Initial: " << edge_editor.get_content() << "\n";
    edge_editor.insert(0, "X"); // Insert at beginning
    std::cout << "Insert 0: " << edge_editor.get_content() << "\n";
    edge_editor.insert(4, "Y"); // Insert at end
    std::cout << "Insert end: " << edge_editor.get_content() << "\n";
    edge_editor.insert(2, "Z"); // Insert in middle
    std::cout << "Insert middle: " << edge_editor.get_content() << "\n"; // XaZbcY
    edge_editor.delete_range(0, 1); // Delete first char
    std::cout << "Delete first: " << edge_editor.get_content() << "\n"; // aZbcY
    edge_editor.delete_range(4, 1); // Delete last char
    std::cout << "Delete last: " << edge_editor.get_content() << "\n"; // aZbc
    edge_editor.delete_range(1, 1); // Delete middle char
    std::cout << "Delete middle: " << edge_editor.get_content() << "\n"; // abc
    edge_editor.undo();
    std::cout << "Undo delete middle: " << edge_editor.get_content() << "\n"; // aZbc
    edge_editor.undo();
    std::cout << "Undo delete last: " << edge_editor.get_content() << "\n"; // aZbcY
    edge_editor.undo();
    std::cout << "Undo delete first: " << edge_editor.get_content() << "\n"; // XaZbcY


    return 0;
}
