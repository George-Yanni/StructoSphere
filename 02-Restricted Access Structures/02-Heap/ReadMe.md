## Heap Data Structure
### Overview
A heap is a specialized tree-based data structure that satisfies the heap property. It is commonly used to implement priority queues, manage dynamic data efficiently, and solve problems requiring ordered elements. Heaps are particularly useful in algorithms like Heap Sort, Dijkstra's shortest path, and Prim's minimum spanning tree.
### What is a Heap?
A heap is a complete binary tree where the value of each node follows a specific order relative to its children:

#### Max-Heap: The value of each parent node is greater than or equal to the values of its children.
#### Min-Heap: The value of each parent node is less than or equal to the values of its children.

Due to its "complete" nature, a heap is typically represented as an array, making it memory-efficient and easy to navigate.

### Key Properties

#### Complete Binary Tree: All levels of the tree are fully filled, except possibly the last level, which is filled from left to right.
#### Heap Property: Ensures the hierarchical ordering (max or min) is maintained across the tree.
#### Height: The height of a heap with n nodes is O(log n), which contributes to its efficiency in operations.
### Array Representation:

For a node at index i:

Left child: 2i + 1
Right child: 2i + 2
Parent: (i-1)/2 (floor division)





## Types of Heaps

#### Max-Heap: The largest value is at the root, and it decreases as you move down the tree. Ideal for retrieving the maximum element quickly.
#### Min-Heap: The smallest value is at the root, and it increases as you move down the tree. Useful for retrieving the minimum element efficiently.

## Common Operations

#### Insertion: Add a new element while maintaining the heap property. O(log n)
#### Deletion: Remove the root (max or min element) and reorganize the heap. O(log n)
#### Heapify: Convert an unordered array into a heap by adjusting elements to satisfy the heap property. O(n)
#### Peek: Access the root element (max or min) without removing it. O(1)

