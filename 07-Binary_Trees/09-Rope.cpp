#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>

// RopeNode represents a node in the Rope's binary tree.
// - Leaf nodes store a string piece (value) and have no children.
// - Internal nodes represent concatenation points and have left/right children.
// - weight: For leaves, the string length; for internal nodes, the length of the left subtree.
struct RopeNode {
    std::unique_ptr<RopeNode> left;   // Left child (nullptr for leaves)
    std::unique_ptr<RopeNode> right;  // Right child (nullptr for leaves)
    std::string value;              // String data (non-empty only for leaves)
    int weight;                       // Length of left subtree (or string length for leaves)

    // Constructor for a leaf node with a string value.
    explicit RopeNode(std::string val)
        : left(nullptr), right(nullptr), value(std::move(val)), weight(0) {}

    // Returns true if this is a leaf node (no children).
    bool is_leaf() const { return !left && !right; }
};

// Rope is a string data structure that uses a binary tree for efficient operations.
// - Each Rope has a root node (or nullptr for an empty Rope).
// - Operations like concat, split, and indexing are optimized by manipulating the tree.
class Rope {
public:
    // Default constructor: Creates an empty Rope.
    Rope() = default;

    // Constructs a Rope from a string (single leaf node).
    explicit Rope(const std::string& str) {
        if (!str.empty()) {
            root = std::make_unique<RopeNode>(str);
            root->weight = str.size(); // Leaf weight is the string length
        }
    }

    // Delete copy constructor and copy assignment to prevent accidental copying.
    Rope(const Rope&) = delete;
    Rope& operator=(const Rope&) = delete;

    // Allow move constructor and move assignment for efficient ownership transfer.
    Rope(Rope&&) = default;
    Rope& operator=(Rope&&) = default;

    // Concatenates this Rope with another, creating a new Rope.
    // - Creates an internal node with this Rope as left child and other as right child.
    // - Time complexity: O(1) for the concatenation, O(n) for cloning if needed.
    Rope concat(const Rope& other) const {
        Rope result;
        if (!root) {
            // If this Rope is empty, clone the other Rope.
            result.root = clone_node(other.root.get());
            return result;
        }
        if (!other.root) {
            // If other Rope is empty, clone this Rope.
            result.root = clone_node(root.get());
            return result;
        }

        // Create a new internal node with empty value.
        result.root = std::make_unique<RopeNode>("");
        result.root->left = clone_node(root.get());         // Left child: this Rope
        result.root->right = clone_node(other.root.get());  // Right child: other Rope
        result.root->weight = compute_weight(result.root->left.get()); // Weight is left subtree length
        return result;
    }

    // Splits the Rope at the given index, returning two new Ropes.
    // - Index 0 means split before the first character.
    // - Uses weight to navigate the tree and split at the correct node.
    // - Throws if index is out of range.
    std::pair<Rope, Rope> split(int index) const {
        if (index < 0 || index > compute_total_weight(root.get())) {
            throw std::out_of_range("Split index out of range");
        }
        auto [left, right] = split_node(root.get(), index);
        return {Rope(std::move(left)), Rope(std::move(right))};
    }

    // Returns the character at the given position.
    // - Traverses the tree using weight to find the correct leaf node.
    // - Throws if pos is out of range.
    char at(int pos) const {
        if (pos < 0 || pos >= compute_total_weight(root.get())) {
            throw std::out_of_range("Index out of range");
        }
        return char_at(root.get(), pos);
    }

    // Reconstructs the full string by traversing the tree.
    // - Concatenates all leaf node values in left-to-right order.
    std::string toString() const {
        std::string result;
        build_string(root.get(), result);
        return result;
    }

    // Prints the tree structure for debugging.
    // - Shows each node's type (leaf or internal), value, and weight.
    void printTree() const {
        print_node(root.get(), 0);
    }

private:
    std::unique_ptr<RopeNode> root; // Root of the binary tree (nullptr if empty)

    // Computes the weight of a node.
    // - For leaves: Returns the string length.
    // - For internal nodes: Returns the weight field (left subtree length).
    // - For nullptr: Returns 0.
    static int compute_weight(const RopeNode* node) {
        if (!node) return 0;
        if (node->is_leaf()) return node->value.size();
        return node->weight;
    }

    // Computes the total length of the string represented by a subtree.
    // - For leaves: Returns the string length.
    // - For internal nodes: Sums left subtree, node value, and right subtree lengths.
    static int compute_total_weight(const RopeNode* node) {
        if (!node) return 0;
        if (node->is_leaf()) return node->value.size();
        return compute_weight(node->left.get()) + node->value.size() + compute_weight(node->right.get());
    }

    // Finds the character at position pos in the subtree rooted at node.
    // - Uses weight to decide whether to traverse left or right.
    // - For leaves: Returns the character at the given index in the value string.
    static char char_at(const RopeNode* node, int pos) {
        if (node->is_leaf()) return node->value[pos];

        if (pos < node->weight) {
            // Position is in the left subtree.
            return char_at(node->left.get(), pos);
        } else {
            // Position is in the right subtree (adjust pos by subtracting weight and node value length).
            return char_at(node->right.get(), pos - node->weight - node->value.size());
        }
    }

    // Builds the full string by appending leaf values to output in-order.
    // - Recursively traverses left subtree, appends node value (if leaf), then traverses right subtree.
    static void build_string(const RopeNode* node, std::string& output) {
        if (!node) return;
        build_string(node->left.get(), output);
        if (node->is_leaf()) output += node->value; // Only leaves contribute to the string
        build_string(node->right.get(), output);
    }

    // Splits the subtree at node at the given index, returning two new subtrees.
    // - For leaves: Splits the string value at the index.
    // - For internal nodes: Uses weight to decide which subtree to split and reconstructs the tree.
    static std::pair<std::unique_ptr<RopeNode>, std::unique_ptr<RopeNode>>
    split_node(const RopeNode* node, int index) {
        if (!node) return {nullptr, nullptr};

        if (node->is_leaf()) {
            // Split the leaf's string value into two parts.
            auto left = std::make_unique<RopeNode>(node->value.substr(0, index));
            auto right = std::make_unique<RopeNode>(node->value.substr(index));
            return {std::move(left), std::move(right)};
        }

        if (index < node->weight) {
            // Split occurs in the left subtree.
            auto [left_left, left_right] = split_node(node->left.get(), index);
            // Right Rope includes the split right part and the original right subtree.
            auto right = std::make_unique<RopeNode>("");
            right->left = std::move(left_right);
            right->right = clone_node(node->right.get());
            right->weight = compute_weight(right->left.get());
            return {std::move(left_left), std::move(right)};
        } else {
            // Split occurs in the right subtree (adjust index).
            auto [right_left, right_right] = split_node(node->right.get(), index - node->weight - node->value.size());
            // Left Rope includes the original left subtree and the split left part.
            auto left = std::make_unique<RopeNode>("");
            left->left = clone_node(node->left.get());
            left->right = std::move(right_left);
            left->weight = node->weight;
            return {std::move(left), std::move(right_right)};
        }
    }

    // Creates a deep copy of a subtree.
    // - Recursively clones all nodes, preserving value and weight.
    static std::unique_ptr<RopeNode> clone_node(const RopeNode* node) {
        if (!node) return nullptr;
        auto new_node = std::make_unique<RopeNode>(node->value);
        new_node->left = clone_node(node->left.get());
        new_node->right = clone_node(node->right.get());
        new_node->weight = node->weight;
        return new_node;
    }

    // Prints the tree structure with indentation to show hierarchy.
    // - Displays node type, value (for leaves), and weight.
    static void print_node(const RopeNode* node, int depth) {
        if (!node) return;
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        if (node->is_leaf()) {
            std::cout << "Leaf: \"" << node->value << "\" (weight: " << node->weight << ")\n";
        } else {
            std::cout << "Internal Node (weight: " << node->weight << ")\n";
        }
        print_node(node->left.get(), depth + 1);
        print_node(node->right.get(), depth + 1);
    }

    // Private constructor for creating a Rope from a node (used by split).
    Rope(std::unique_ptr<RopeNode> node) : root(std::move(node)) {}
};

// Demonstrates Rope operations with small strings to clarify the binary tree structure.
// - Creates two simple Ropes, concatenates them, splits the result, and accesses a character.
// - Prints the tree and string at each step to visualize the changes.
int main() {
    // Step 1: Create two Ropes with short strings.
    std::cout << "=== Creating Initial Ropes ===\n";
    Rope rope1("Hi");
    std::cout << "rope1 string: " << rope1.toString() << "\n";
    std::cout << "rope1 tree:\n";
    rope1.printTree();
    std::cout << "\n";

    Rope rope2("Go");
    std::cout << "rope2 string: " << rope2.toString() << "\n";
    std::cout << "rope2 tree:\n";
    rope2.printTree();
    std::cout << "\n";

    // Step 2: Concatenate rope1 and rope2 to form "HiGo".
    std::cout << "=== Concatenating rope1 and rope2 ===\n";
    Rope concatenated = rope1.concat(rope2);
    std::cout << "Concatenated string: " << concatenated.toString() << "\n";
    std::cout << "Concatenated tree (note the internal node):\n";
    concatenated.printTree();
    std::cout << "\n";

    // Step 3: Split the concatenated Rope at index 2 (after "Hi").
    // - Left Rope should be "Hi", Right Rope should be "Go".
    std::cout << "=== Splitting at index 2 ===\n";
    auto [left, right] = concatenated.split(2);
    std::cout << "Left Rope string: " << left.toString() << "\n";
    std::cout << "Left Rope tree:\n";
    left.printTree();
    std::cout << "\n";
    std::cout << "Right Rope string: " << right.toString() << "\n";
    std::cout << "Right Rope tree:\n";
    right.printTree();
    std::cout << "\n";

    // Step 4: Access a character at index 3 (should be 'o' from "Go").
    std::cout << "=== Accessing character at index 3 ===\n";
    std::cout << "Character at index 3: " << concatenated.at(3) << "\n";

    return 0;
}