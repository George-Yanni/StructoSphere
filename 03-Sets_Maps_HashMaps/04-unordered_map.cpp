#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
#include <stdexcept>

using namespace std;

// Custom class to demonstrate unordered_map with user-defined types
class Person {
public:
    string name;
    int age;
    
    Person(string n, int a) : name(n), age(a) {}
    
    // Overload == operator for comparison
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// Hash function for Person class
namespace std {
    template<>
    struct hash<Person> {
        size_t operator()(const Person& p) const {
            return hash<string>()(p.name) ^ hash<int>()(p.age);
        }
    };
}

int main() {
    // 1. Basic Declaration and Initialization
    unordered_map<int, string> basicMap = {
        {1, "Apple"},
        {2, "Banana"},
        {3, "Orange"}
    };
    
    cout << "1. Basic Operations Demo:\n";
    cout << "Initial size: " << basicMap.size() << endl;
    
    // 2. Insertion Methods
    unordered_map<string, int> scores;
    
    // Insert using insert()
    scores.insert({"Alice", 95});
    
    // Insert using operator[]
    scores["Bob"] = 87;
    
    // Insert using emplace() - more efficient as it constructs in-place
    scores.emplace("Charlie", 91);
    
    cout << "\n2. Insertion Methods Demo:\n";
    for(const auto& pair : scores) {
        cout << pair.first << ": " << pair.second << endl;
    }
    
    // 3. Access and Modification
    cout << "\n3. Access and Modification:\n";
    cout << "Bob's score: " << scores.at("Bob") << endl;  // throws if key not found
    cout << "Alice's score: " << scores["Alice"] << endl; // creates if not found
    
    // Modify existing value
    scores["Bob"] = 89;
    cout << "Bob's updated score: " << scores["Bob"] << endl;
    
    // 4. Search Operations
    cout << "\n4. Search Operations:\n";
    string searchKey = "David";
    auto it = scores.find(searchKey);
    if(it != scores.end()) {
        cout << searchKey << " found with score: " << it->second << endl;
    } else {
        cout << searchKey << " not found" << endl;
    }
    
    // Count method
    cout << "Occurrences of Alice: " << scores.count("Alice") << endl;
    
    // 5. Deletion Operations
    cout << "\n5. Deletion Operations:\n";
    cout << "Size before erase: " << scores.size() << endl;
    scores.erase("Charlie");
    cout << "Size after erase: " << scores.size() << endl;
    
    // 6. Custom Type Demo
    unordered_map<Person, string> people;
    people.emplace(Person("John", 25), "Engineer");
    people.emplace(Person("Mary", 30), "Doctor");
    
    cout << "\n6. Custom Type Demo:\n";
    for(const auto& pair : people) {
        cout << pair.first.name << " (" << pair.first.age 
             << "): " << pair.second << endl;
    }
    
    // 7. Bucket Interface Demo
    cout << "\n7. Bucket Interface Demo:\n";
    cout << "Number of buckets: " << scores.bucket_count() << endl;
    cout << "Load factor: " << scores.load_factor() << endl;
    cout << "Max load factor: " << scores.max_load_factor() << endl;
    
    // 8. Exception Handling
    cout << "\n8. Exception Handling Demo:\n";
    try {
        int score = scores.at("David"); // Non-existent key
        cout << "Score: " << score << endl;
    } catch(const out_of_range& e) {
        cout << "Exception caught: Key not found" << endl;
    }
    
    // 9. Iteration Methods
    cout << "\n9. Iteration Demo:\n";
    cout << "Forward iteration:\n";
    for(auto it = scores.begin(); it != scores.end(); ++it) {
        cout << it->first << ": " << it->second << endl;
    }
    
    // 10. Capacity and Rehash
    cout << "\n10. Capacity Management:\n";
    unordered_map<int, int> numbers;
    numbers.reserve(100); // Pre-allocate space for 100 elements
    cout << "Bucket count after reserve: " << numbers.bucket_count() << endl;
    
    // Demonstrate rehash
    numbers.rehash(200);
    cout << "Bucket count after rehash: " << numbers.bucket_count() << endl;
    
    return 0;
}