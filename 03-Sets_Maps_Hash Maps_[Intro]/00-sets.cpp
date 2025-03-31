#include <iostream>
#include <set>
#include <string>
#include <algorithm> // For std::min_element, std::max_element
#include <iterator>  // For std::distance

// Custom comparator for descending order
struct DescendingOrder {
    bool operator()(const int& a, const int& b) const {
        return a > b; // Reverse the default ordering (ascending)
    }
};

// Updated printSet: Template both the key type (T) and comparator (Compare)
template<typename T, typename Compare>
void printSet(const std::set<T, Compare>& s, const std::string& label) {
    std::cout << label << ": { ";
    for (const auto& elem : s) {
        std::cout << elem << " ";
    }
    std::cout << "}\n";
}

int main() {
    // 1. Basic std::set with default ordering (ascending)
    std::set<int> basicSet;

    // 2. Insertion - O(log n) complexity, duplicates are automatically ignored
    std::cout << "=== Insertion Demo ===\n";
    basicSet.insert(5);           // Single element insertion
    basicSet.insert({3, 7, 1, 5}); // Insert multiple elements (5 ignored due to uniqueness)
    printSet(basicSet, "After insertions (1, 3, 5, 7, 5)");

    // 3. Size and emptiness check
    std::cout << "\n=== Size and Emptiness ===\n";
    std::cout << "Size of basicSet: " << basicSet.size() << "\n"; // O(1)
    std::cout << "Is basicSet empty? " << (basicSet.empty() ? "Yes" : "No") << "\n";

    // 4. Searching - O(log n) complexity due to balanced BST
    std::cout << "\n=== Searching Demo ===\n";
    int searchKey = 3;
    auto findResult = basicSet.find(searchKey); // Returns iterator to element or end()
    if (findResult != basicSet.end()) {
        std::cout << "Found " << searchKey << " in basicSet\n";
    } else {
        std::cout << searchKey << " not found in basicSet\n";
    }
    std::cout << "Does 6 exist? " << (basicSet.count(6) ? "Yes" : "No") << "\n"; // count() returns 0 or 1

    // 5. Iteration - Elements are always in sorted order
    std::cout << "\n=== Iteration Demo ===\n";
    std::cout << "Forward iteration: ";
    for (auto it = basicSet.begin(); it != basicSet.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\nReverse iteration: ";
    for (auto rit = basicSet.rbegin(); rit != basicSet.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    // 6. Deletion - O(log n) complexity
    std::cout << "\n=== Deletion Demo ===\n";
    basicSet.erase(3); // Erase by key
    printSet(basicSet, "After erasing 3");
    auto it = basicSet.find(7);
    if (it != basicSet.end()) {
        basicSet.erase(it); // Erase by iterator
    }
    printSet(basicSet, "After erasing 7 (via iterator)");

    // 7. Bounds - lower_bound and upper_bound for range queries
    std::cout << "\n=== Bounds Demo ===\n";
    basicSet.insert({2, 4, 6}); // Re-populate for demo
    printSet(basicSet, "Set after adding 2, 4, 6");
    auto lb = basicSet.lower_bound(3); // First element >= 3
    auto ub = basicSet.upper_bound(5); // First element > 5
    std::cout << "Elements in range [3, 5]: ";
    for (auto it = lb; it != ub; ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // 8. Min and Max elements (using iterators)
    std::cout << "\n=== Min/Max Demo ===\n";
    if (!basicSet.empty()) {
        std::cout << "Min element: " << *basicSet.begin() << "\n"; // Smallest key
        std::cout << "Max element: " << *basicSet.rbegin() << "\n"; // Largest key
    }

    // 9. Custom comparator - std::set with descending order
    std::cout << "\n=== Custom Comparator Demo ===\n";
    std::set<int, DescendingOrder> descendingSet{4, 1, 6, 2};
    printSet(descendingSet, "Descending order set");

    // 10. Set operations (e.g., merging two sets)
    std::cout << "\n=== Set Operations Demo ===\n";
    std::set<int> setA{1, 3, 5};
    std::set<int> setB{2, 4, 6};
    std::set<int> unionSet;
    std::set_union(setA.begin(), setA.end(), setB.begin(), setB.end(),
                   std::inserter(unionSet, unionSet.begin()));
    printSet(unionSet, "Union of setA and setB");

    // 11. Clear the set
    std::cout << "\n=== Clear Demo ===\n";
    basicSet.clear();
    printSet(basicSet, "basicSet after clear");

    return 0;
}