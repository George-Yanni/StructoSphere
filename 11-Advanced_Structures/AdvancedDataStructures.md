# Advanced Data Structures Overview

This document provides an overview of three advanced data structures: **Disjoint-set (Union-Find)**, **Fenwick Tree**, and **Ternary Search Tree**. Each is designed to solve specific problems efficiently, with unique properties that make them suitable for particular applications.

## 1. Disjoint-Set (Union-Find)

### Overview
The Disjoint-Set data structure, also known as Union-Find, is used to manage a collection of elements partitioned into non-overlapping subsets. It supports two primary operations: **Union** (merging two subsets) and **Find** (determining the subset an element belongs to). It is widely used in algorithms like Kruskal’s for minimum spanning trees and detecting cycles in graphs.

### Key Operations
- **Find**: Determines the representative (or "parent") of the set containing a given element. Uses path compression to optimize future queries.
- **Union**: Merges two sets by attaching the smaller tree to the root of the larger one (union by rank/size).
- **Connected**: Checks if two elements are in the same set.

### Time Complexity
- **Find**: O(α(n)) amortized, where α(n) is the inverse Ackermann function (nearly constant for practical purposes).
- **Union**: O(α(n)) amortized.
- **Connected**: O(α(n)) amortized.

### Applications
- Kruskal’s algorithm for minimum spanning trees.
- Detecting cycles in undirected graphs.
- Connected component analysis in graphs.

### Example Use Case
In a social network, Union-Find can efficiently track groups of friends by merging friend relationships (Union) and checking if two users belong to the same group (Find).

## 2. Fenwick Tree (Binary Indexed Tree)

### Overview
The Fenwick Tree, or Binary Indexed Tree (BIT), is a data structure that efficiently handles dynamic cumulative frequency tables or prefix sum queries. It is particularly useful for problems involving range sum queries or point updates on an array.

### Key Operations
- **Update**: Updates the value at a given index and propagates the change to affected prefix sums.
- **Query**: Computes the prefix sum up to a given index or a range sum by subtracting prefix sums.

### Time Complexity
- **Update**: O(log n).
- **Query**: O(log n).
- **Space Complexity**: O(n).

### Applications
- Calculating cumulative sums in dynamic arrays.
- Range sum queries (e.g., sum of elements between indices i and j).
- Used in competitive programming for problems like range updates and queries.

### Example Use Case
In a financial application, a Fenwick Tree can track cumulative stock price changes over time, allowing quick updates when new data arrives and fast range queries to compute total changes over a period.

## 3. Ternary Search Tree

### Overview
A Ternary Search Tree (TST) is a tree-based data structure used for storing and searching strings, combining the benefits of binary search trees and tries. Each node stores a character, with three children: left (for characters less than the node’s character), middle (for the next character in the string), and right (for characters greater than the node’s character). It is efficient for dictionary operations and autocomplete systems.

### Key Operations
- **Insert**: Adds a string to the tree by traversing or creating nodes for each character.
- **Search**: Checks if a string exists in the tree.
- **Prefix Search**: Retrieves all strings with a given prefix (useful for autocomplete).

### Time Complexity
- **Insert**: O(m), where m is the length of the string.
- **Search**: O(m).
- **Prefix Search**: O(m + k), where k is the number of strings with the prefix.
- **Space Complexity**: O(N * m), where N is the number of strings and m is the average string length.

### Applications
- Autocomplete and spell-checking systems.
- Dictionary implementations for fast string lookups.
- IP routing tables for prefix matching.

### Example Use Case
In a search engine’s autocomplete feature, a TST can store a dictionary of words and quickly retrieve all words starting with a user-typed prefix, such as “cat” yielding “category,” “cater,” and “catnip.”

## Summary
These data structures—Disjoint-Set, Fenwick Tree, and Ternary Search Tree—are powerful tools for solving specialized problems:
- **Disjoint-Set** excels in managing dynamic set relationships.
- **Fenwick Tree** optimizes range queries and updates in numerical arrays.
- **Ternary Search Tree** is ideal for string-based operations like autocomplete.
