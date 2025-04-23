#include <iostream>
#include <string>
using namespace std;

/*
===============================================================================
   Self-Organizing Linked List for Recommender System Based on Search Count
===============================================================================
Each node contains:
- The item (string)
- A count of how many times it was searched

If the search count of an item exceeds a user-defined threshold, it is moved
to the front of the list to make recommendations more responsive to popular items.
===============================================================================
*/

class RecommenderList {
private:
    struct Node {
        string item;
        int searchCount;
        Node* next;

        Node(string val) : item(val), searchCount(0), next(nullptr) {}
    };

    Node* head;
    int threshold;

public:
    RecommenderList(int threshold) : head(nullptr), threshold(threshold) {}

    // Insert a new item at the end
    void insert(const string& item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
    }

    // Search for an item, increment its search count, and reorganize if needed
    bool search(const string& item) {
        if (!head)
            return false;

        if (head->item == item) {
            head->searchCount++;
            return true;
        }

        Node* prev = head;
        Node* curr = head->next;

        while (curr) {
            if (curr->item == item) {
                curr->searchCount++;
                if (curr->searchCount >= threshold) {
                    // Move to front
                    prev->next = curr->next;
                    curr->next = head;
                    head = curr;
                }
                return true;
            }
            prev = curr;
            curr = curr->next;
        }

        return false;
    }

    // Print the current list and search counts
    void print() const {
        Node* temp = head;
        cout << "Recommender List:\n";
        while (temp) {
            cout << "- " << temp->item << " (searches: " << temp->searchCount << ")\n";
            temp = temp->next;
        }
    }

    // Cleanup
    ~RecommenderList() {
        Node* temp = head;
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
};

// Example usage
int main() {
    RecommenderList recList(3); // Threshold = 3

    // Insert initial items into the list
    recList.insert("Pizza");
    recList.insert("Burger");
    recList.insert("Sushi");
    recList.insert("Pasta");
    recList.insert("Salad");

    // Print the list initially
    cout << "Initial List:\n";
    recList.print();

    // Simulate searches
    cout << "\nSearching for items...\n";
    recList.search("Sushi");
    recList.search("Sushi");
    recList.search("Pizza");
    recList.search("Sushi");  // This will move Sushi to the front
    recList.search("Pizza");
    recList.search("Pizza");  // Now Pizza gets promoted to the front as well

    // Print the rearranged list after searches
    cout << "\nList after searches and rearrangement:\n";
    recList.print();

    return 0;
}
