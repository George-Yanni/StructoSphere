#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>

class MerkleTree {
private:
    std::vector<std::string> dataBlocks;
    std::vector<std::vector<std::string>> tree;

    // Use std::hash for demo purposes; in real-world, use SHA-256 or Keccak
    std::string hash(const std::string& data) {
        std::hash<std::string> hasher;
        size_t hashed = hasher(data);
        std::stringstream ss;
        ss << std::hex << hashed;
        return ss.str();
    }

    void buildTree() {
        std::vector<std::string> currentLevel;
        for (const auto& data : dataBlocks)
            currentLevel.push_back(hash(data));

        tree.push_back(currentLevel);

        while (currentLevel.size() > 1) {
            std::vector<std::string> nextLevel;
            for (size_t i = 0; i < currentLevel.size(); i += 2) {
                std::string left = currentLevel[i];
                std::string right = (i + 1 < currentLevel.size()) ? currentLevel[i + 1] : left;
                nextLevel.push_back(hash(left + right));
            }
            tree.push_back(nextLevel);
            currentLevel = nextLevel;
        }
    }

public:
    MerkleTree(const std::vector<std::string>& blocks) : dataBlocks(blocks) {
        buildTree();
    }

    std::string getMerkleRoot() const {
        return tree.back()[0];
    }

    void printTree() const {
        std::cout << "Merkle Tree Structure:\n";
        for (int level = tree.size() - 1; level >= 0; --level) {
            std::cout << "Level " << level << ": ";
            for (const auto& h : tree[level])
                std::cout << "[" << h.substr(0, 8) << "] "; // Shortened hash
            std::cout << "\n";
        }
    }

    // Generate Merkle proof for block at index
    std::vector<std::string> generateProof(size_t index) {
        std::vector<std::string> proof;
        if (index >= tree[0].size()) return proof;

        for (size_t level = 0; level < tree.size() - 1; ++level) {
            size_t siblingIndex = (index % 2 == 0) ? index + 1 : index - 1;
            if (siblingIndex >= tree[level].size())
                proof.push_back(tree[level][index]); // duplicate last if no sibling
            else
                proof.push_back(tree[level][siblingIndex]);

            index /= 2;
        }
        return proof;
    }

    bool verifyProof(const std::string& data, const std::vector<std::string>& proof, const std::string& root, size_t index) {
        std::string currentHash = hash(data);
        for (const auto& siblingHash : proof) {
            if (index % 2 == 0)
                currentHash = hash(currentHash + siblingHash);
            else
                currentHash = hash(siblingHash + currentHash);
            index /= 2;
        }
        return currentHash == root;
    }
};

int main() {
    std::vector<std::string> blocks = {"Alice pays Bob", "Bob pays Carol", "Carol pays Dave", "Dave pays Eve"};

    MerkleTree tree(blocks);
    tree.printTree();

    std::string root = tree.getMerkleRoot();
    std::cout << "\nMerkle Root: " << root << "\n\n";

    // Example Merkle proof
    size_t index = 1; // Bob pays Carol
    std::string data = blocks[index];
    std::vector<std::string> proof = tree.generateProof(index);

    std::cout << "Merkle Proof for block \"" << data << "\" at index " << index << ":\n";
    for (const auto& h : proof)
        std::cout << "- [" << h.substr(0, 8) << "]\n";

    bool valid = tree.verifyProof(data, proof, root, index);
    std::cout << "\nVerification result: " << (valid ? "Valid" : "Invalid") << "\n";

    std::cout << "\n--- Merkle Tree Strengths ---\n";
    std::cout << "1. Efficient integrity verification (log(N) proof size).\n";
    std::cout << "2. Small Merkle root represents entire data set.\n";
    std::cout << "3. Secure against tampering (any change alters root).\n";
    std::cout << "4. Used in Bitcoin, Git, IPFS, blockchains.\n";

    return 0;
}
