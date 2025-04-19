#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>

void printDivider(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void printMultimap(const std::multimap<std::string, int>& mm) {
    std::cout << "Contents of multimap:\n";
    for (const auto& [key, value] : mm) {
        std::cout << key << " -> " << value << '\n';
    }
}

int main() {
    std::multimap<std::string, int> students;

    printDivider("Insertion");
    // Insertion: allows duplicate keys
    students.insert({"Alice", 85});
    students.insert({"Bob", 90});
    students.insert({"Alice", 95});
    students.insert({"David", 70});
    students.emplace("Bob", 88); // emplace is more efficient

    printMultimap(students);

    printDivider("Iteration with Equal Range");
    // Retrieve all values associated with a single key
    const std::string target = "Alice";
    auto range = students.equal_range(target);
    std::cout << "All values for key '" << target << "':\n";
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << " -> " << it->second << '\n';
    }

    printDivider("Counting Entries");
    // Counting how many entries are stored for a given key
    std::cout << "Number of entries for 'Bob': " << students.count("Bob") << '\n';

    printDivider("Erasing Elements");
    // Erasing all values for a specific key
    std::cout << "Erasing all 'David' entries...\n";
    students.erase("David");

    printMultimap(students);

    printDivider("Custom Comparator Example");
    // Creating a multimap with custom comparator (reverse alphabetical order)
    auto reverseComp = [](const std::string& a, const std::string& b) {
        return a > b;
    };
    std::multimap<std::string, int, decltype(reverseComp)> reversedMap(reverseComp);

    reversedMap.insert({"Zoe", 100});
    reversedMap.insert({"Eve", 78});
    reversedMap.insert({"Zoe", 92});

    for (const auto& [key, value] : reversedMap) {
        std::cout << key << " -> " << value << '\n';
    }

    printDivider("Range-based Operations (lower_bound & upper_bound)");
    // Demonstrating use of lower_bound and upper_bound
    std::string queryKey = "Bob";
    auto lower = students.lower_bound(queryKey);
    auto upper = students.upper_bound(queryKey);

    std::cout << "Range of elements with key '" << queryKey << "':\n";
    for (auto it = lower; it != upper; ++it) {
        std::cout << it->first << " -> " << it->second << '\n';
    }

    printDivider("Using std::equal_range with structured binding");
    if (auto [begin, end] = students.equal_range("Alice"); begin != end) {
        std::cout << "Entries for 'Alice':\n";
        std::for_each(begin, end, [](const auto& pair) {
            std::cout << pair.first << " -> " << pair.second << '\n';
        });
    } else {
        std::cout << "'Alice' not found.\n";
    }

    return 0;
}
