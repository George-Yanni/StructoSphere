## What Is a Multimap?

A **Multimap** is a generalization of a standard map or dictionary that allows **multiple values to be associated with a single key**. Unlike a typical map where each key maps to a single value, a multimap allows duplicate keys and associates each key with a list or collection of values.

### Key Features:
- Supports insertion of duplicate key-value pairs.
- Efficient retrieval of all values associated with a particular key.
- Removal and update operations.
- Iteration over all key-value pairs.

### Common Use Cases:
- Grouping items by a shared property.
- Representing adjacency lists in graphs.
- Indexing data with one-to-many relationships.

---

## What Is a Multiset?

A **Multiset** (also known as a *bag*) is a modification of a traditional set that allows **multiple instances of the same element**. Unlike a set, where each element is unique, a multiset keeps track of the count of each element.

### Key Features:
- Allows insertion of duplicate elements.
- Maintains the frequency count of each element.
- Supports operations like union, intersection, and difference with multiplicity awareness.
- Efficient access to element counts and total size.

### Common Use Cases:
- Frequency analysis (e.g., word or character frequency).
- Inventory systems where items may have quantities.
- Mathematical applications involving combinations or permutations with repetition.
