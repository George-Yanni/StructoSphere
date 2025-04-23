#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
/*
 * Skip List Implementation (Basic implementation)
 * 
 * This code implements a probabilistic data structure called Skip List, which allows 
 * O(log n) search complexity with simplified balancing compared to binary trees.
 * 
 * Key Components:
 * - Node class: Represents elements with value and forward pointers to next nodes at each level
 * - SkipList class: Manages the skip list with insert, search and display operations
 * 
 * Main Features:
 * - Dynamic node levels determined by random probability (0.5 in this case)
 * - Insertion maintains sorted order across all levels
 * - Search starts from highest level, dropping down when needed
 * - Visual display shows structure at each level
 * - Search operation includes step counting for performance analysis
 * 
 * The main() function demonstrates:
 * - Insertion of multiple values
 * - Displaying list structure after each insert
 * - Searching for existing and non-existing values
 * 
 * Note: Uses random number generation for probabilistic level assignment
 */


class Node
{
public:
    int value;
    Node **forward;
    Node(int value, int level)
    {
        this->value = value;
        forward = new Node *[level + 1];
        std::fill(forward, forward + level + 1, nullptr);
    }
    ~Node()
    {
        delete[] forward;
    }
};

class SkipList
{
private:
    int maxLevel;
    float probability;
    int level;
    Node *header;

    int randomLevel()
    {
        int lvl = 0;
        while (((float)rand() / RAND_MAX) < probability && lvl < maxLevel)
        {
            lvl++;
        }
        return lvl;
    }

public:
    SkipList(int maxLevel, float probability)
    {
        this->maxLevel = maxLevel;
        this->probability = probability;
        level = 0;
        header = new Node(INT_MIN, maxLevel);
        srand(time(0));
    }

    ~SkipList()
    {
        Node *current = header;
        while (current)
        {
            Node *next = current->forward[0];
            delete current;
            current = next;
        }
    }

    void insert(int value)
    {
        Node *update[maxLevel + 1];
        Node *current = header;

        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] && current->forward[i]->value < value)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->value != value)
        {
            int lvl = randomLevel();
            if (lvl > level)
            {
                for (int i = level + 1; i <= lvl; i++)
                {
                    update[i] = header;
                }
                level = lvl;
            }

            Node *newNode = new Node(value, lvl);
            for (int i = 0; i <= lvl; i++)
            {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
            std::cout << "Inserted " << value << "\n";
        }
    }

bool search(int value) {
    Node* current = header;
    int steps = 0; // Initialize step counter
    std::cout << "Searching for " << value << ":\n";

    for (int i = level; i >= 0; i--) {
        std::cout << "Level " << i << ": ";
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
            std::cout << current->value << " -> ";
            steps++; // Increment step counter
        }

        // Check if the value is found at the current level
        if (current->forward[i] && current->forward[i]->value == value) {
            std::cout << current->forward[i]->value << "\n";
            std::cout << "Found " << value << " in " << steps << " steps\n";
            return true;
        }

        std::cout << (current->forward[i] ? std::to_string(current->forward[i]->value) : "nullptr") << "\n";
        steps++; // Increment step counter for the final comparison at this level
    }

    std::cout << "Not Found " << value << " in " << steps << " steps\n";
    return false;
}


    void display()
    {
        for (int i = 0; i <= level; i++)
        {
            Node *node = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (node != nullptr)
            {
                std::cout << node->value << " ";
                node = node->forward[i];
            }
            std::cout << "\n";
        }
    }
};

int main()
{
    SkipList list(3, 0.5);
    list.insert(1);
    list.display();

    list.insert(2);
    list.display();

    list.insert(3);
    list.display();
    list.insert(4);
    list.display();
    list.insert(6);
    list.display();
    list.insert(7);
    list.display();
    list.insert(8);
    list.insert(9);
    list.display();
    list.insert(10);
    list.display();
    list.insert(11);
    list.display();
    list.insert(5);

    list.display();
    printf("\nstart searching..................\n\n");
    list.search(9);
    printf("\nstart searching..................\n\n");
    list.search(15);

    return 0;
}
