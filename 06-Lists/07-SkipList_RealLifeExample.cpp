#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <climits>
// We're simulating an IoT system that collects temperature readings
// over time and allows querying for a time range.
using namespace std;

#define MAX_LEVEL 6
#define P 0.5

struct Reading
{
    time_t timestamp;
    double temperature;

    bool operator<(const Reading &other) const
    {
        return timestamp < other.timestamp;
    }

    bool operator==(const Reading &other) const
    {
        return timestamp == other.timestamp;
    }
};

struct Node
{
    Reading data;
    Node **forward;
    Node(int level, Reading data) : data(data)
    {
        forward = new Node *[level + 1];
        for (int i = 0; i <= level; ++i)
            forward[i] = nullptr;
    }
};

class TimeSeriesSkipList
{
    int level;
    Node *header;

public:
    TimeSeriesSkipList()
    {
        level = 0;
        header = new Node(MAX_LEVEL, {INT_MIN, 0.0});
        srand(time(nullptr));
    }

    int randomLevel()
    {
        int lvl = 0;
        while ((double)rand() / RAND_MAX < P && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }

    void insert(time_t timestamp, double temp)
    {
        Reading data = {timestamp, temp};
        Node *current = header;
        Node *update[MAX_LEVEL + 1];

        for (int i = 0; i <= MAX_LEVEL; ++i)
            update[i] = nullptr;

        for (int i = level; i >= 0; --i)
        {
            while (current->forward[i] && current->forward[i]->data.timestamp < timestamp)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];
        if (current && current->data.timestamp == timestamp)
        {
            current->data.temperature = temp; // update if exists
            return;
        }

        int newLevel = randomLevel();
        if (newLevel > level)
        {
            for (int i = level + 1; i <= newLevel; ++i)
                update[i] = header;
            level = newLevel;
        }

        Node *newNode = new Node(newLevel, data);
        for (int i = 0; i <= newLevel; ++i)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    vector<Reading> queryRange(time_t start, time_t end)
    {
        vector<Reading> results;
        Node *current = header;

        for (int i = level; i >= 0; --i)
        {
            while (current->forward[i] && current->forward[i]->data.timestamp < start)
                current = current->forward[i];
        }

        current = current->forward[0];
        while (current && current->data.timestamp <= end)
        {
            results.push_back(current->data);
            current = current->forward[0];
        }

        return results;
    }

    void removeOlderThan(time_t cutoff)
    {
        Node *current = header;
        for (int i = level; i >= 0; --i)
        {
            while (current->forward[i] && current->forward[i]->data.timestamp < cutoff)
                current = current->forward[i];
            header->forward[i] = current->forward[i];
        }
    }

    void printList()
    {
        cout << "\n=== Skip List: Time Series ===\n";
        for (int i = level; i >= 0; --i)
        {
            Node *node = header->forward[i];
            cout << "Level " << i << ": ";
            while (node)
            {
                cout << "(" << node->data.timestamp << "," << node->data.temperature << ") ";
                node = node->forward[i];
            }
            cout << endl;
        }
    }
};

// Utility to simulate timestamps
time_t nowPlus(int seconds)
{
    return time(nullptr) + seconds;
}

// Driver code
int main()
{
    TimeSeriesSkipList list;

    // Insert sensor data (timestamp, temperature)
    list.insert(nowPlus(0), 23.1);
    list.insert(nowPlus(10), 22.8);
    list.insert(nowPlus(20), 22.5);
    list.insert(nowPlus(30), 22.2);
    list.insert(nowPlus(40), 21.9);
    list.insert(nowPlus(50), 21.7);

    list.printList();

    // Query a range
    time_t t1 = nowPlus(10);
    time_t t2 = nowPlus(40);
    auto results = list.queryRange(t1, t2);

    cout << "\nQueried readings between " << t1 << " and " << t2 << ":\n";
    for (const auto &r : results)
        cout << "  Time: " << r.timestamp << ", Temp: " << r.temperature << "\n";

    // Remove old readings (e.g., before now + 30)
    time_t cutoff = nowPlus(30);
    list.removeOlderThan(cutoff);
    cout << "\nAfter removing data older than " << cutoff << ":\n";
    list.printList();

    return 0;
}
