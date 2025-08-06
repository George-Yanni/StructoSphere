#include <iostream>
#include <string>

const int ALPHABET_SIZE = 26;

class TrieNode {
public:
    TrieNode* children[ALPHABET_SIZE];
    bool isEnd;

    TrieNode() : isEnd(false) {
        for (int i = 0; i < ALPHABET_SIZE; i++) children[i] = nullptr;
    }

    ~TrieNode() {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            delete children[i];
        }
    }
};

class Trie {
private:
    TrieNode* root;

    void print(TrieNode* node, std::string prefix) {
        if (node->isEnd) std::cout << prefix << "\n";
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i])
                print(node->children[i], prefix + char('a' + i));
        }
    }

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { delete root; }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (c < 'a' || c > 'z') continue; // skip invalid chars
            int index = c - 'a';
            if (!node->children[index]) node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->isEnd = true;
    }

    bool search(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (c < 'a' || c > 'z') return false;
            int index = c - 'a';
            if (!node->children[index]) return false;
            node = node->children[index];
        }
        return node->isEnd;
    }

    bool startsWith(const std::string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') return false;
            int index = c - 'a';
            if (!node->children[index]) return false;
            node = node->children[index];
        }
        return true;
    }

    void print() {
        print(root, "");
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("bat");
    trie.insert("banana");

    std::cout << "Stored words:\n";
    trie.print();

    std::cout << "\nSearch 'apple': " << (trie.search("apple") ? "Found" : "Not Found") << "\n";
    std::cout << "Search 'app': "   << (trie.search("app")   ? "Found" : "Not Found") << "\n";
    std::cout << "Starts with 'ba': " << (trie.startsWith("ba") ? "Yes" : "No") << "\n";
    std::cout << "Starts with 'cat': " << (trie.startsWith("cat") ? "Yes" : "No") << "\n";

    return 0;
}
