#include <iostream>
#include <string>
#include <map>

class RadixNode
{
public:
    std::string prefix;
    std::map<char, RadixNode *> children;
    bool isEnd;

    RadixNode(const std::string &p = "") : prefix(p), isEnd(false) {}
    ~RadixNode()
    {
        for (auto &child : children)
            delete child.second;
    }
};

class RadixTree
{
private:
    RadixNode *root;

    void insert(RadixNode *node, const std::string &key, int pos)
    {
        std::string remaining = key.substr(pos);
        for (auto &[ch, child] : node->children)
        {
            std::string &childPrefix = child->prefix;
            int i = 0;
            while (i < childPrefix.size() && i < remaining.size() && childPrefix[i] == remaining[i])
                i++;

            if (i == 0)
                continue;

            if (i < childPrefix.size())
            {
                // Split child node
                RadixNode *split = new RadixNode(childPrefix.substr(i));
                split->children = std::move(child->children);
                split->isEnd = child->isEnd;

                child->prefix = childPrefix.substr(0, i);
                child->children.clear();
                child->children[split->prefix[0]] = split;
                child->isEnd = false;
            }

            if (i < remaining.size())
            {
                insert(child, key, pos + i);
            }
            else
            {
                child->isEnd = true;
            }
            return;
        }

        // No common prefix found — insert new leaf
        RadixNode *newLeaf = new RadixNode(remaining);
        newLeaf->isEnd = true;
        node->children[remaining[0]] = newLeaf;
    }

    bool search(RadixNode *node, const std::string &key, int pos)
    {
        std::string remaining = key.substr(pos);
        for (auto &[ch, child] : node->children)
        {
            std::string &p = child->prefix;
            if (remaining.substr(0, p.size()) == p)
            {
                if (remaining.size() == p.size())
                    return child->isEnd;
                return search(child, key, pos + p.size());
            }
        }
        return false;
    }

    void print(RadixNode *node, const std::string &path, int level = 0)
    {
        std::string full = path + node->prefix;
        if (node->isEnd)
            std::cout << std::string(level * 2, ' ') << "- " << full << "\n";

        for (auto &[_, child] : node->children)
            print(child, full, level + 1);
    }

public:
    RadixTree() : root(new RadixNode()) {}
    ~RadixTree() { delete root; }

    void insert(const std::string &key) { insert(root, key, 0); }
    bool search(const std::string &key) { return search(root, key, 0); }
    void print() { print(root, ""); }
};

int main()
{
    RadixTree tree;
    tree.insert("test");
    tree.insert("testings");
    tree.insert("team");
    tree.insert("toast");
    tree.insert("tester");
    tree.insert("testing");

    std::cout << "Radix Tree:\n";
    tree.print();

    std::cout << "\nSearch results:\n";
    for (const std::string &word : {"test", "team", "toast", "tester", "testing", "toaster"})
        std::cout << word << ": " << (tree.search(word) ? "Found" : "Not Found") << "\n";

    return 0;
}
