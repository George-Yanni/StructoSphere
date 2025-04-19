#include <iostream>
#include <map>
#include <vector>

using namespace std;

// Demonstrating basic array operations
void demonstrateArray() {
    // Static Array
    int arr[5] = {1, 2, 3, 4, 5};
    cout << "Static Array: ";
    for (int i = 0; i < 5; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Dynamic Array (using vector)
    vector<int> dynArr = {10, 20, 30, 40, 50};
    cout << "Dynamic Array (vector): ";
    for (int i = 0; i < dynArr.size(); ++i) {
        cout << dynArr[i] << " ";
    }
    cout << endl;

    // Accessing elements
    cout << "Accessing dynamic array element at index 2: " << dynArr[2] << endl;

    // Array Operations
    dynArr.push_back(60); // Adding new element
    cout << "After push_back(60), dynamic array: ";
    for (auto elem : dynArr) {
        cout << elem << " ";
    }
    cout << endl;

    // Removing an element from the back
    dynArr.pop_back();
    cout << "After pop_back(), dynamic array: ";
    for (auto elem : dynArr) {
        cout << elem << " ";
    }
    cout << endl;
}

// Demonstrating associative array (std::map) operations
void demonstrateAssociativeArray() {
    // Creating a map (associative array)
    map<int, string> ageToName;

    // Inserting data into map
    ageToName[25] = "Alice";
    ageToName[30] = "Bob";
    ageToName[35] = "Charlie";

    cout << "Associative Array (std::map):" << endl;
    for (const auto& pair : ageToName) {
        cout << "Age: " << pair.first << " -> Name: " << pair.second << endl;
    }

    // Accessing elements by key
    cout << "Accessing value by key (age 30): " << ageToName[30] << endl;

    // Checking if a key exists
    if (ageToName.find(40) != ageToName.end()) {
        cout << "Age 40 exists: " << ageToName[40] << endl;
    } else {
        cout << "Age 40 does not exist in the map." << endl;
    }

    // Modifying a value
    ageToName[25] = "Alicia";
    cout << "After modification, age 25: " << ageToName[25] << endl;

    // Erasing an element
    ageToName.erase(30);
    cout << "After erasing age 30:" << endl;
    for (const auto& pair : ageToName) {
        cout << "Age: " << pair.first << " -> Name: " << pair.second << endl;
    }

    // Iterating through the map in reverse order
    cout << "Reversed map order: " << endl;
    for (auto it = ageToName.rbegin(); it != ageToName.rend(); ++it) {
        cout << "Age: " << it->first << " -> Name: " << it->second << endl;
    }
}

int main() {
    cout << "Array Data Structure Demonstration:" << endl;
    demonstrateArray();
    cout << endl;

    cout << "Associative Array (std::map) Demonstration:" << endl;
    demonstrateAssociativeArray();
    cout << endl;

    // Summary of differences
    cout << "Differences between Array and Associative Array (std::map):" << endl;
    cout << "1. Array is a linear data structure, while associative array (map) stores key-value pairs." << endl;
    cout << "2. Arrays have fixed sizes (except dynamic arrays like vector), while maps dynamically grow as new key-value pairs are inserted." << endl;
    cout << "3. Array indices are integers (sequential), while map keys can be any data type (integers, strings, etc.)." << endl;
    cout << "4. Arrays allow fast indexing, while map provides fast access and manipulation based on keys but requires logarithmic time for lookups." << endl;

    return 0;
}
