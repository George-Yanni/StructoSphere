// Browser URL Cache with Splay Tree

// Splay trees are excellent for this because:
// Frequently visited URLs will be near the root (cache locality)
// Recently accessed URLs are quickly accessible (O(log n) amortized time)
// Infrequently used URLs automatically move deeper in the tree

// This can help in managing a cache of URLs efficiently, especially in web browsers or proxy servers.

// The Code:
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

struct URLNode {
    string url;
    string content;
    URLNode* left;
    URLNode* right;
    URLNode* parent;

    URLNode(string u, string c) : url(u), content(c), left(nullptr), right(nullptr), parent(nullptr) {}
};

class BrowserCache {
private:
    URLNode* root;
    unordered_map<string, URLNode*> urlMap;
    const int CACHE_LIMIT = 10;
    int currentSize = 0;

    // Splay Tree operations
    void leftRotate(URLNode* x) {
        URLNode* y = x->right;
        if (!y) return;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(URLNode* x) {
        URLNode* y = x->left;
        if (!y) return;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void splay(URLNode* x) {
        while (x->parent) {
            if (!x->parent->parent) {
                if (x == x->parent->left) rightRotate(x->parent);
                else leftRotate(x->parent);
            } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
                rightRotate(x->parent->parent);
                rightRotate(x->parent);
            } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
                leftRotate(x->parent->parent);
                leftRotate(x->parent);
            } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
                leftRotate(x->parent);
                rightRotate(x->parent);
            } else {
                rightRotate(x->parent);
                leftRotate(x->parent);
            }
        }
    }

    URLNode* findNode(const string& url) {
        URLNode* x = root;
        while (x) {
            if (url < x->url) x = x->left;
            else if (url > x->url) x = x->right;
            else return x;
        }
        return nullptr;
    }

    void removeLRU() {
        // Find the deepest node (least recently used)
        URLNode* lru = root;
        while (lru && (lru->left || lru->right)) {
            lru = lru->left ? lru->left : lru->right;
        }

        if (lru) {
            cout << "Evicting from cache: " << lru->url << endl;
            urlMap.erase(lru->url);
            if (lru->parent) {
                if (lru->parent->left == lru) lru->parent->left = nullptr;
                else lru->parent->right = nullptr;
            } else {
                // If lru is the root, set root to nullptr
                root = nullptr;
            }
            delete lru;
            currentSize--;
        }
    }

public:
    BrowserCache() : root(nullptr) {}

    void visitURL(const string& url, const string& content) {
        // Check if URL exists
        URLNode* node = findNode(url);

        if (node) {
            // URL exists - update content and splay
            node->content = content;
            splay(node);
            cout << "Updated in cache: " << url << endl;
        } else {
            // New URL - check cache limit
            if (currentSize >= CACHE_LIMIT) {
                removeLRU();
            }

            // Insert new URL
            node = new URLNode(url, content);
            URLNode* p = nullptr;
            URLNode* x = root;

            while (x) {
                p = x;
                if (url < x->url) x = x->left;
                else x = x->right;
            }

            node->parent = p;
            if (!p) root = node;
            else if (url < p->url) p->left = node;
            else p->right = node;

            splay(node);
            urlMap[url] = node;
            currentSize++;
            cout << "Added to cache: " << url << endl;
        }
    }

    string getURLContent(const string& url) {
        URLNode* node = findNode(url);
        if (node) {
            splay(node);
            cout << "Retrieved from cache: " << url << endl;
            return node->content;
        }
        return "Not in cache";
    }

    void printCache() {
        cout << "\nCurrent Cache Contents (Most to Least Recent):\n";
        printPreOrder(root);
        cout << endl;
    }

    void printPreOrder(URLNode* node) {
        if (!node) return;
        cout << "- " << node->url << endl;
        printPreOrder(node->left);
        printPreOrder(node->right);
    }
};

int main() {
    BrowserCache cache;

    // Simulate browsing activity
    cache.visitURL("https://google.com", "Google Search Page");
    cache.visitURL("https://github.com", "GitHub Homepage");
    cache.visitURL("https://stackoverflow.com", "StackOverflow Q&A");
    cache.visitURL("https://reddit.com", "Reddit Frontpage");
    cache.printCache();

    // Access some pages multiple times
    cache.getURLContent("https://github.com");
    cache.getURLContent("https://stackoverflow.com");
    cache.printCache();

    // Continue browsing until cache fills up
    cache.visitURL("https://wikipedia.org", "Wikipedia Main");
    cache.visitURL("https://youtube.com", "YouTube Videos");
    cache.visitURL("https://twitter.com", "Twitter Feed");
    cache.visitURL("https://linkedin.com", "LinkedIn Network");
    cache.visitURL("https://amazon.com", "Amazon Shopping");
    cache.visitURL("https://netflix.com", "Netflix Shows");
    cache.printCache();

    // Access old pages to bring them to front
    cout << "\nRevisiting Google...\n";
    cache.getURLContent("https://google.com");
    cache.getURLContent("https://reddit.com");
    cache.printCache();

    return 0;
}
