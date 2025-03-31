#### In this exploration, I will delve into the concepts of sets, maps, and hash maps, focusing on their differences, usage, and properties. We will examine how each data structure works, their strengths and limitations, and when to use them in different scenarios. Additionally, we will provide real-life examples to demonstrate how sets, maps, and hash maps can be applied effectively in various situations, helping to better understand their practical applications

<br>

<h3 style="color: green;">As a quick start, this is a simple comparison between sets and maps:</h3>
<br>
<br>

| Feature                       | `std::set`                              | `std::map`                              |
|------------------------------|-----------------------------------------|-----------------------------------------|
| **Purpose**                  | Stores unique elements in sorted order  | Stores key-value pairs in sorted order  |
| **Element Type**             | Contains only keys (no values)          | Contains key-value pairs                |
| **Sorting**                  | Elements sorted based on key            | Elements sorted based on key (default)  |
| **Value Access**             | Can only access the key                 | Can access both key and value           |
| **Duplicates**               | No duplicates allowed                   | No duplicate keys allowed               |
| **Data Structure**           | Self-balancing BST (e.g., Red-Black Tree) | Self-balancing BST (e.g., Red-Black Tree) |
| **Insertion**                | Uses key only                           | Requires both key and value             |
| **Example**                  | `std::set<int> mySet = {1, 2, 3};`     | `std::map<int, std::string> myMap = {{1, "one"}, {2, "two"}};` |
| **Time Complexity**          | Insert/Delete/Search: O(log n)          | Insert/Delete/Search: O(log n)          |

<h3 style="color: red;">And the following will be a comparison between maps and hash maps</h3>

| Feature                      | `std::map`                              | `std::unordered_map`(Hash Maps)                    |
|------------------------------|-----------------------------------------|-----------------------------------------|
| **Purpose**                  | Stores key-value pairs in sorted order  | Stores key-value pairs with no guaranteed order |
| **Underlying Structure**     | Self-balancing BST (Red-Black Tree)     | Hash table                              |
| **Sorting**                  | Keys are sorted (using `std::less` by default) | Keys are unsorted (order depends on hash function) |
| **Time Complexity**          | - Lookup: O(log n) <br> - Insertion: O(log n) <br> - Deletion: O(log n) | - Average case: O(1) <br> - Worst case: O(n) (hash collisions) |
| **Memory Usage**             | Lower overhead (needs balancing)        | Higher overhead (hash table buckets)    |
| **Duplicates**               | No duplicate keys allowed               | No duplicate keys allowed               |
| **Iterator Behavior**        | Iterates in sorted key order            | Iterates in unspecified order           |
| **Key Requirements**         | Needs comparison operator (`<`)         | Needs hash function and `==` operator   |
| **Performance**              | Slower lookups (tree traversal)         | Faster lookups (average case)           |
| **Use Case**                 | When sorted order or range queries needed | When fast lookups/insertions priority   |
