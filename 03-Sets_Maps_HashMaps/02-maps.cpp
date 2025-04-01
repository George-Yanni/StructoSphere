#include <iostream>
#include <map>
#include <string>
#include <utility> // for std::pair
#include <algorithm> // for std::for_each

using namespace std;

// Functor for printing map elements (demonstrates custom iteration)
struct MapPrinter {
    void operator()(const pair<const int, string>& p) const {
        cout << "Key: " << p.first << ", Value: " << p.second << endl;
    }
};

// Function to demonstrate map bounds checking
void printBounds(const map<int, string>& m, int key) {
    cout << "\nChecking bounds for key " << key << ":" << endl;
    auto lb = m.lower_bound(key);
    auto ub = m.upper_bound(key);
    
    if (lb != m.end())
        cout << "Lower bound: Key " << lb->first << ", Value " << lb->second << endl;
    else
        cout << "Lower bound: End of map" << endl;
        
    if (ub != m.end())
        cout << "Upper bound: Key " << ub->first << ", Value " << ub->second << endl;
    else
        cout << "Upper bound: End of map" << endl;
}

int main() {
    // 1. Creating a map (ordered by keys automatically)
    map<int, string> studentRecords;
    
    // 2. Inserting elements - Multiple methods
    // Method 1: Using insert with pair
    studentRecords.insert(pair<int, string>(1001, "Alice"));
    
    // Method 2: Using make_pair
    studentRecords.insert(make_pair(1002, "Bob"));
    
    // Method 3: Using operator[]
    studentRecords[1003] = "Charlie";  // Note: Creates element if doesn't exist
    
    // Method 4: Using emplace (more efficient - constructs in place)
    studentRecords.emplace(1004, "David");
    
    cout << "Initial map contents:" << endl;
    for (const auto& [key, value] : studentRecords) {  // Structured binding (C++17)
        cout << "ID: " << key << ", Name: " << value << endl;
    }
    
    // 3. Size and emptiness checking
    cout << "\nMap size: " << studentRecords.size() << endl;
    cout << "Is map empty? " << (studentRecords.empty() ? "Yes" : "No") << endl;
    
    // 4. Finding elements
    int searchKey = 1002;
    auto it = studentRecords.find(searchKey);
    if (it != studentRecords.end()) {
        cout << "\nFound key " << searchKey << ": " << it->second << endl;
    } else {
        cout << "\nKey " << searchKey << " not found" << endl;
    }
    
    // 5. Using count (returns 0 or 1 since map has unique keys)
    cout << "Number of elements with key 1001: " << studentRecords.count(1001) << endl;
    
    // 6. Modifying existing elements
    studentRecords[1003] = "Charles";  // Updates existing value
    cout << "\nAfter modification of key 1003: " << studentRecords[1003] << endl;
    
    // 7. Erasing elements
    // Method 1: By key
    studentRecords.erase(1004);
    cout << "\nAfter erasing key 1004, size: " << studentRecords.size() << endl;
    
    // Method 2: By iterator
    auto eraseIt = studentRecords.find(1002);
    if (eraseIt != studentRecords.end()) {
        studentRecords.erase(eraseIt);
        cout << "Erased key 1002, new size: " << studentRecords.size() << endl;
    }
    
    // 8. Custom iteration using functor
    cout << "\nUsing custom functor to print:" << endl;
    for_each(studentRecords.begin(), studentRecords.end(), MapPrinter());
    
    // 9. Bounds checking
    printBounds(studentRecords, 1002);
    
    // 10. Clearing the map
    studentRecords.clear();
    cout << "\nAfter clear(), size: " << studentRecords.size() << endl;
    
    // 11. Exception handling with at() method
    try {
        cout << studentRecords.at(1001) << endl;  // Throws if key doesn't exist
    } catch (const out_of_range& e) {
        cout << "Exception caught: " << e.what() << endl;
    }
    
    // 12. Demonstrating map with custom comparator (descending order)
    map<int, string, greater<int>> reverseMap;
    reverseMap[1] = "One";
    reverseMap[2] = "Two";
    reverseMap[3] = "Three";
    
    cout << "\nMap with reverse ordering:" << endl;
    for (const auto& pair : reverseMap) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}