#include <iostream>
#include <set>
#include <string>
#include <map>

// Function to print the elements of a multiset and their frequency
void printMultiset(const std::multiset<std::string>& ms) {
    std::cout << "Multiset contents:\n";
    for (const auto& element : ms) {
        std::cout << element << '\n';
    }
    std::cout << std::endl;
}

// Function to print frequency of each element in the multiset
void printElementCounts(const std::multiset<std::string>& ms) {
    std::cout << "Element counts:\n";
    std::map<std::string, int> counts;

    // Count occurrences using multiset iterators
    for (const auto& element : ms) {
        counts[element]++;
    }

    for (const auto& pair : counts) {
        std::cout << pair.first << ": " << pair.second << " times\n";
    }
    std::cout << std::endl;
}

int main() {
    // Create a multiset to store strings (can be used for word frequency counting)
    std::multiset<std::string> wordMultiset;

    // Insert words into the multiset (some words are repeated)
    wordMultiset.insert("apple");
    wordMultiset.insert("banana");
    wordMultiset.insert("apple");
    wordMultiset.insert("orange");
    wordMultiset.insert("banana");
    wordMultiset.insert("apple");
    wordMultiset.insert("pear");

    // Print the contents of the multiset
    printMultiset(wordMultiset);

    // Print the frequency of each element in the multiset
    printElementCounts(wordMultiset);

    // Count occurrences of a specific word (e.g., "apple")
    std::string searchWord = "apple";
    auto count = wordMultiset.count(searchWord);
    std::cout << "The word '" << searchWord << "' appears " << count << " times.\n\n";

    // Removing one occurrence of a specific element (e.g., remove "apple")
    auto it = wordMultiset.find("apple");
    if (it != wordMultiset.end()) {
        wordMultiset.erase(it);  // Erase one occurrence of "apple"
    }
    std::cout << "After erasing one 'apple':\n";
    printMultiset(wordMultiset);

    // Erase all occurrences of a specific element (e.g., remove all "banana")
    wordMultiset.erase("banana");  // Erase all occurrences of "banana"
    std::cout << "After erasing all 'banana':\n";
    printMultiset(wordMultiset);

    // Inserting a new word
    wordMultiset.insert("grape");
    std::cout << "After inserting 'grape':\n";
    printMultiset(wordMultiset);

    // Accessing the first element in the multiset (note: the multiset is ordered)
    if (!wordMultiset.empty()) {
        std::cout << "The first element (lexicographically): " << *wordMultiset.begin() << "\n\n";
    }

    // Use of multiset as a sorted collection (elements will be sorted by default)
    std::cout << "Sorted multiset:\n";
    printMultiset(wordMultiset);

    // Example of iterating over the multiset and handling elements manually
    std::cout << "Iterating through the multiset manually:\n";
    for (auto it = wordMultiset.begin(); it != wordMultiset.end(); ++it) {
        std::cout << *it << "\n";
    }

    return 0;
}
