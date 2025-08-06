#include <iostream>
using namespace std;

struct Node {
    char c;
    bool isEnd = false;
    Node *left = nullptr, *eq = nullptr, *right = nullptr;
};

class TernarySearchTree {
    Node* root = nullptr;

    Node* insert(Node* node, const string& word, int pos) {
        if (pos >= word.size()) return node;
        char ch = word[pos];
        if (!node) node = new Node{ch};
        if (ch < node->c) node->left = insert(node->left, word, pos);
        else if (ch > node->c) node->right = insert(node->right, word, pos);
        else {
            if (pos + 1 == word.size()) node->isEnd = true;
            else node->eq = insert(node->eq, word, pos + 1);
        }
        return node;
    }

    bool search(Node* node, const string& word, int pos) {
        if (!node) return false;
        char ch = word[pos];
        if (ch < node->c) return search(node->left, word, pos);
        else if (ch > node->c) return search(node->right, word, pos);
        else {
            if (pos + 1 == word.size()) return node->isEnd;
            return search(node->eq, word, pos + 1);
        }
    }

public:
    void insert(const string& word) {
        root = insert(root, word, 0);
    }

    bool search(const string& word) {
        return search(root, word, 0);
    }
};

int main() {
    TernarySearchTree tst;
    tst.insert("cat");
    tst.insert("car");
    tst.insert("bat");

    cout << "Searching 'car': " << (tst.search("car") ? "Found" : "Not Found") << endl;
    cout << "Searching 'cap': " << (tst.search("cap") ? "Found" : "Not Found") << endl;
}
