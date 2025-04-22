# <b> 1- Singly Linked List

A **Singly Linked List** is a linear data structure where each element (node) contains data and a reference (or link) to the next node in the sequence. This structure allows for efficient insertion, deletion, and traversal operations, but access to elements is sequential.

### Structure

Each node in a singly linked list consists of:
- **Data**: The value stored in the node.
- **Next**: A reference to the next node in the list (or `null` for the last node).

The list also maintains a **head** pointer, which points to the first node, and optionally a **tail** pointer, which points to the last node.

### Visual Representation
```
head -> [data|next] -> [data|next] -> [data|null]
```

### Operations:

### 1. **Insertion**
- **At the beginning**:
  - Create a new node.
  - Set its `next` to the current head.
  - Update the head to point to the new node.
  - Time Complexity: O(1)
- **At the end**:
  - Create a new node.
  - Traverse to the last node.
  - Set the last node's `next` to the new node.
  - Time Complexity: O(n)
- **At a specific position**:
  - Traverse to the node before the desired position.
  - Adjust the `next` pointers to insert the new node.
  - Time Complexity: O(n)

### 2. **Deletion**
- **From the beginning**:
  - Update the head to point to the next node.
  - Time Complexity: O(1)
- **From the end**:
  - Traverse to the second-to-last node.
  - Set its `next` to `null`.
  - Time Complexity: O(n)
- **From a specific position**:
  - Traverse to the node before the one to be deleted.
  - Adjust the `next` pointers to skip the target node.
  - Time Complexity: O(n)

### 3. **Traversal**
- Start at the head and follow the `next` pointers until `null` is reached.
- Time Complexity: O(n)

### 4. **Search**
- Traverse the list sequentially to find a specific value.
- Time Complexity: O(n)

### Advantages
- Dynamic size: Can grow or shrink as needed.
- Efficient insertions/deletions at the head (O(1)).
- Memory efficient: Only allocates memory for nodes as needed.

### Disadvantages
- Sequential access: No direct access to elements (O(n) for random access).
- Extra memory: Requires additional memory for `next` pointers.
- Not cache-friendly: Nodes may not be stored contiguously in memory.



### Applications
- Implementing stacks and queues.
- Managing dynamic memory allocation (e.g., in operating systems).
- Maintaining playlists or histories (e.g., music players, browser history).
- Used as a building block for more complex data structures (e.g., hash tables with chaining).

### Time Complexities Summary
| Operation           | Time Complexity |
|---------------------|-----------------|
| Insertion (Head)    | O(1)            |
| Insertion (End)     | O(n)            |
| Deletion (Head)     | O(1)            |
| Deletion (End)      | O(n)            |
| Search              | O(n)            |
| Traversal           | O(n)            |


<br>

# <b> 2-  Doubly Linked List

A **Doubly Linked List** is a linear data structure where each element (node) contains data and two references: one to the next node and one to the previous node in the sequence. This structure allows for efficient insertion, deletion, and bidirectional traversal, but it requires more memory than a singly linked list.

### Structure

Each node in a doubly linked list consists of:

- **Data**: The value stored in the node.
- **Next**: A reference to the next node in the list (or `null` for the last node).
- **Prev**: A reference to the previous node in the list (or `null` for the first node).

The list maintains a **head** pointer to the first node and optionally a **tail** pointer to the last node.

### Visual Representation

```
head -> [null|data|next] <-> [prev|data|next] <-> [prev|data|null] <- tail
```

### Operations

### 1. **Insertion**

- **At the beginning**:
  - Create a new node.
  - Set its `next` to the current head and `prev` to `null`.
  - Update the head's `prev` (if exists) to the new node.
  - Update the head to the new node.
  - Time Complexity: O(1)
- **At the end**:
  - Create a new node.
  - Set its `prev` to the current tail and `next` to `null`.
  - Update the tail's `next` (if exists) to the new node.
  - Update the tail to the new node.
  - Time Complexity: O(1) (if tail pointer is maintained)
- **At a specific position**:
  - Traverse to the desired position.
  - Adjust the `next` and `prev` pointers of the new node and adjacent nodes.
  - Time Complexity: O(n)

### 2. **Deletion**

- **From the beginning**:
  - Update the head to the next node.
  - Set the new head's `prev` to `null` (if exists).
  - Time Complexity: O(1)
- **From the end**:
  - Update the tail to the previous node.
  - Set the new tail's `next` to `null` (if exists).
  - Time Complexity: O(1) (if tail pointer is maintained)
- **From a specific position**:
  - Traverse to the node to be deleted.
  - Adjust the `next` and `prev` pointers of adjacent nodes to skip the target node.
  - Time Complexity: O(n)

### 3. **Traversal**

- **Forward**: Start at the head and follow `next` pointers until `null`.
- **Backward**: Start at the tail and follow `prev` pointers until `null`.
- Time Complexity: O(n)

### 4. **Search**

- Traverse the list (forward or backward) to find a specific value.
- Time Complexity: O(n)

### Advantages

- Bidirectional traversal: Can move forward or backward.
- Efficient insertions/deletions at both ends (O(1) with head and tail pointers).
- Easier deletion: No need to traverse to find the previous node.

### Disadvantages

- Extra memory: Requires additional memory for `prev` pointers.
- More complex operations: Managing `prev` and `next` pointers increases implementation complexity.
- Sequential access: No direct access to elements (O(n) for random access).



###
- Implementing browser history (forward/back navigation).
- Undo/redo functionality in applications.
- Managing playlists with bidirectional navigation.
- Used in data structures like deques (double-ended queues).

### Time Complexities Summary

| Operation | Time Complexity |
| --- | --- |
| Insertion (Head) | O(1) |
| Insertion (End) | O(1) |
| Deletion (Head) | O(1) |
| Deletion (End) | O(1) |
| Search | O(n) |
| Traversal | O(n) |

<br>

# <b> 3- Self-Organizing List

A **self-organizing list** is a type of linked list that rearranges its elements dynamically based on access patterns, optimizing for faster future lookups. It is often used when access frequency is skewed or not uniform — some elements are accessed more often than others.


###  What Is It?

A **self-organizing list** maintains key-value pairs (or any kind of items) in a **linked list** structure and **reorders elements on access** to reduce the average lookup time.

This is especially useful in cases where:
- The cost of a search is significant
- Some items are accessed much more frequently
- Simpler alternatives like arrays or associative arrays are overkill or unavailable



### Key Strategies:

#### 1. **Move-to-Front (MTF)**
- Move the accessed item to the front of the list.
- Simple and often very effective in real-world scenarios.
- Good when recently used items are likely to be reused soon.

#### 2. **Transpose**
- Swap the accessed item with its immediate predecessor.
- Gradually brings frequently accessed items toward the front.
- Less disruptive than MTF.

#### 3. **Count-Based**
- Each item maintains a counter of how many times it has been accessed.
- The list is sorted in descending order of access frequency.
- Most accurate, but incurs overhead to maintain sorted order.



### Example Use Cases

- Symbol tables in compilers or interpreters
- Frequently accessed configuration or cache data
- Environments with limited memory (no hash tables or trees)
- Systems with highly non-uniform access patterns



### Pros and Cons

####  Benefits
- **Adaptive performance:** Becomes faster as usage pattern stabilizes.
- **Simple implementation:** Can be built using basic data structures.
- **Space-efficient:** No hashing or tree pointers required.
- **Flexible:** Supports duplicate keys (if needed).

####  Limitations
- **Linear-time lookup:** Worst-case lookup is still O(n).
- **Overhead on access:** Reordering adds complexity per operation.
- **Not ideal for random access:** No direct indexing support.
- **Poor scalability:** Not suitable for large datasets.

# <b> 4- Skip List Data Structure

A **Skip List** is a probabilistic data structure that allows fast search, insertion, and deletion operations — similar to balanced trees — but with **simpler algorithms and a layered design**.

Skip lists maintain elements in **sorted order**, and each element may appear in multiple layers, forming "express lanes" that **skip** over elements to speed up traversal.



### Key Concept

A Skip List consists of multiple levels of **linked lists**:
- The **bottom level** contains all the elements in sorted order.
- Each **higher level** acts as a shortcut over the lower levels.

This layered structure allows skip lists to achieve **O(log n)** time complexity for search, insertion, and deletion in **expected** time.



### Structure Overview

```
Level 3:      [A] ------------------------ [Z]
Level 2:      [A] --------- [M] --------- [Z]
Level 1:      [A] ---- [F] ---- [M] ---- [T] ---- [Z]
Level 0:      [A] -> [B] -> [C] -> ... -> [Z]
```

Each node may be "promoted" to higher levels with some probability (commonly 50%). This randomness is what gives skip lists their efficiency without requiring strict balancing like AVL or Red-Black Trees.



### Operations and Complexity

| Operation | Time Complexity |
|-----------|-----------------|
| Search    | O(log n) (expected) |
| Insert    | O(log n) (expected) |
| Delete    | O(log n) (expected) |
| Space     | O(n)             |

Skip lists **do not guarantee** worst-case logarithmic performance, but in practice they perform very efficiently with high probability.



### Why Use a Skip List?

-  Easier to implement than balanced binary search trees
-  Supports efficient ordered traversal
-  Naturally supports **range queries**
-  Performs well for **concurrent access** (lock-free versions exist)
-  Predictable performance with random level assignment



### Use Cases

- **In-memory databases** (e.g., Redis uses skip lists for sorted sets)
- **Search engines**
- **Index structures**
- **Concurrent systems**

# <b> 5- Unrolled Linked List

An **Unrolled Linked List** is a variation of the traditional **linked list** structure, designed to **reduce memory overhead** by grouping multiple elements together into a single node. Each node in the unrolled linked list can store multiple elements in an **array** (instead of just one) and the nodes themselves are still connected via pointers.

This structure improves the performance of operations like **insertion, deletion, and traversal** by reducing the number of nodes and the overhead that comes with maintaining numerous pointers.



### How It Works

Instead of having a separate node for each element in the list, an **unrolled linked list** stores **multiple elements** (typically in an array) within a single node. The node also holds a pointer to the next node, just like a standard linked list.

#### Structure Example

```plaintext
Node 1: [A, B, C] -> Node 2: [D, E] -> Node 3: [F, G, H, I]
```

In this case:
- **Node 1** contains the elements `[A, B, C]`
- **Node 2** contains the elements `[D, E]`
- **Node 3** contains the elements `[F, G, H, I]`

---

### Operations and Complexity

#### 1. **Insertion**
- When inserting into an unrolled linked list, the insertion point is found by traversing the linked list.
- If there is space in the current node (i.e., the internal array isn't full), the element is inserted into the array.
- If the node is full, the array is split, and a new node is created to hold the overflow.

**Time Complexity**: 
- **O(n)** for traversal to find the position (in the worst case).
- **O(1)** for insertion into the internal array.

#### 2. **Deletion**
- Deletion is similar to insertion: traverse to the node, remove the element from the internal array, and if necessary, shift elements within the array.
- If a node becomes empty after deletion, it may be removed from the list.

**Time Complexity**: 
- **O(n)** for traversal to find the node to delete.
- **O(1)** for deletion inside the internal array.

#### 3. **Search**
- The search operation traverses the linked list and performs a linear search on the internal arrays in each node.
  
**Time Complexity**: 
- **O(n)** in the worst case, since you have to search through both the linked list and the array inside each node.



### Advantages of Unrolled Linked List

- **Reduced Pointer Overhead**: Instead of storing one pointer per element (as in a traditional linked list), you only need one pointer per node, reducing overhead.
- **Cache-Friendly**: Since elements are stored contiguously in arrays inside each node, the list is more cache-friendly, which leads to faster access times compared to a traditional linked list.
- **Efficient Memory Usage**: Grouping multiple elements into a single node helps reduce memory fragmentation, which is a common issue with traditional linked lists.
- **Improved Traversal Speed**: If the array inside the node is large, traversal can skip several elements at once, improving performance.



### Disadvantages of Unrolled Linked List

- **More Complex Implementation**: Handling dynamic resizing of internal arrays and managing splitting/merging of nodes can complicate the implementation.
- **Inefficient for Small Lists**: If the list is small or has sparse access patterns, the overhead of managing nodes and arrays might outweigh the benefits.
- **Less Flexibility in Operations**: Insertion and deletion are not as straightforward as in a basic array or linked list due to the need for array splitting or merging.



### Use Cases

- **Memory-Intensive Applications**: Where reducing pointer overhead and improving memory efficiency is critical.
- **Cache-Optimized Systems**: Applications that require frequent traversal of data can benefit from the cache-friendly nature of the unrolled linked list.
- **Custom Data Structures**: Often used when implementing specialized data structures like **priority queues** or **multi-level indexing**.



### Comparison with Other Data Structures

| Feature               | Unrolled Linked List | Linked List | Array | Skip List |
|-----------------------|----------------------|-------------|-------|-----------|
| Memory Usage          | Lower (array + pointer) | High (pointer per element) | Low | Moderate |
| Cache Efficiency      | High (contiguous blocks) | Low | High | Moderate |
| Insertion/Deletion    | O(1) in node, O(n) in traversal | O(1) | O(n) | O(log n) |
| Search                | O(n)                  | O(n)        | O(1)  | O(log n) |

---

### Summary

An **unrolled linked list** is a specialized **linked list** that combines the simplicity of a linked list with the performance benefits of array-based storage. It reduces pointer overhead, improves cache efficiency, and can be a great option when you need efficient memory usage with frequent traversal.

If you're dealing with systems that require high-speed data processing and memory efficiency — especially when large datasets are involved — the **unrolled linked list** might be a perfect choice.

# <b> 6- Free List

A **Free List** is a data structure used in memory management to track and allocate free memory blocks in a heap. It maintains a list of unused memory blocks, allowing efficient allocation and deallocation of memory in systems like operating systems or custom allocators.

### Structure

A free list consists of nodes, where each node represents a free memory block and contains:
- **Size**: The size of the free memory block.
- **Next**: A pointer to the next free block (or `null` for the last block).
- The memory block itself follows the node metadata.

The list is typically implemented as a singly linked list, with a **head** pointer to the first free block.

#### Visual Representation
```
head -> [size|next] -> [size|next] -> [size|null]
```

### Operations

#### 1. **Allocation**
- Traverse the list to find a block with size ≥ requested size.
- If found, split the block (if large enough) or remove it from the list.
- Return the block’s address to the user.
- Time Complexity: O(n) (depends on traversal).

#### 2. **Deallocation**
- Add the freed block back to the list.
- Optionally merge with adjacent free blocks to reduce fragmentation.
- Time Complexity: O(1) (without merging), O(n) (with merging).

#### 3. **Merging**
- Combine adjacent free blocks to form a larger block.
- Time Complexity: O(n).

### Advantages
- Efficient memory reuse in dynamic allocation.
- Simple to implement for basic memory management.
- Reduces external fragmentation (with merging).

### Disadvantages
- Slow allocation: Requires traversal to find a suitable block.
- Internal fragmentation: Splitting blocks may leave small, unusable chunks.
- Complex merging: Requires tracking adjacent blocks.

### Applications
- Memory management in operating systems (e.g., kernel heap).
- Custom memory allocators in embedded systems.
- Memory pools for specific object sizes.

### Time Complexities
| Operation    | Time Complexity |
|--------------|-----------------|
| Allocation   | O(n)            |
| Deallocation | O(1) or O(n)    |
| Merging      | O(n)            |

