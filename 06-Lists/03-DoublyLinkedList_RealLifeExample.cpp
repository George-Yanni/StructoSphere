#include <iostream>
#include <unordered_map>

/*
===============================================================================
                           LRU (Least Recently Used) Cache
===============================================================================
This class implements an LRU Cache with fixed capacity, combining a 
hash map and a doubly linked list to achieve O(1) time complexity for both:
    - get(key): Retrieve a value by key
    - put(key, value): Insert or update a value, evicting the least recently used if needed

--------------------------
 Data Structures Used:
--------------------------
1. Hash Map (unordered_map<int, Node*>):
   - Maps a key to its corresponding node in the linked list
   - Allows O(1) access to cache entries by key

2. Doubly Linked List:
   - Maintains the usage order of cache entries:
       * Most recently used (MRU) at the front
       * Least recently used (LRU) at the back
   - Enables O(1) removal and reordering of nodes

-------------------------
 LRU Logic Description:
-------------------------
- Every time a key is accessed (via get or put), it becomes the "most recently used"
  → Move its node to the front of the linked list.

- When inserting a new key:
  → If the cache is full, remove the node at the end of the list (the least recently used).
  → Insert the new node at the front and update the map.

--------------------------
 Benefits of this Design:
--------------------------
- get(key) → O(1) time
- put(key, value) → O(1) time (amortized)
- Efficient memory usage for real-world scenarios like:
    * Browser cache
    * Database page replacement
    * OS memory management
    * CDN request caching

--------------------------------
 Internal Implementation Notes:
--------------------------------
- The linked list uses dummy 'head' and 'tail' nodes to simplify edge cases
- Nodes are inserted immediately after head (front of list)
- LRU node is always just before tail

*/



class LRUCache {
private:
    struct Node {
        int key, value;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    std::unordered_map<int, Node*> map;
    Node* head;
    Node* tail;

    // Add node right after head
    void addToFront(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    // Remove a node from the list
    void removeNode(Node* node) {
        Node* prevNode = node->prev;
        Node* nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    // Move an existing node to the front
    void moveToFront(Node* node) {
        removeNode(node);
        addToFront(node);
    }

public:
    LRUCache(int cap) : capacity(cap) {
        // Dummy head and tail to simplify logic
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    int get(int key) {
        if (map.find(key) == map.end())
            return -1;
        Node* node = map[key];
        moveToFront(node);
        return node->value;
    }

    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            Node* node = map[key];
            node->value = value;
            moveToFront(node);
        } else {
            if (map.size() == capacity) {
                // Remove least recently used (before tail)
                Node* lru = tail->prev;
                map.erase(lru->key);
                removeNode(lru);
                delete lru;
            }

            Node* newNode = new Node(key, value);
            map[key] = newNode;
            addToFront(newNode);
        }
    }

    void display() {
        Node* curr = head->next;
        std::cout << "Cache state: ";
        while (curr != tail) {
            std::cout << "(" << curr->key << "," << curr->value << ") ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
};

// Usage example
int main() {
    LRUCache cache(3);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    cache.display(); // (3,300)(2,200)(1,100)

    cache.get(2);    // Access key 2
    cache.display(); // (2,200)(3,300)(1,100)

    cache.put(4, 400); // Evicts key 1
    cache.display();   // (4,400)(2,200)(3,300)

    return 0;
}
