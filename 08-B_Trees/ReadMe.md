### <b>First, let's start by comparing Binary Trees (as previously covered) with B-trees.

# Binary Tree vs B Tree Comparison

| Feature | Binary Tree | B Tree |
|---------|-------------|--------|
| **Definition** | A tree data structure where each node has at most two children (left and right). | A self-balancing tree data structure that maintains sorted data and allows multiple children per node. |
| **Node Structure** | Each node contains a value and pointers to left and right children. | Each node contains multiple keys and pointers to children, with a defined minimum and maximum number of keys. |
| **Height** | Can become unbalanced, leading to a height of O(n) in worst cases (e.g., skewed tree). | Self-balancing, with a height of O(log n), ensuring efficient operations. |
| **Balancing** | Not inherently balanced; requires explicit balancing (e.g., AVL or Red-Black trees). | Automatically balanced during insertions and deletions. |
| **Search Time** | O(log n) in balanced cases, O(n) in unbalanced cases. | O(log n) due to balanced structure and multiple keys per node. |
| **Insert/Delete Time** | O(log n) in balanced cases, O(n) in unbalanced cases; may require rebalancing. | O(log n), with efficient handling of splits and merges. |
| **Storage** | Minimal storage overhead; each node has two pointers. | Higher storage overhead due to multiple keys and pointers per node. |
| **Use Case** | Suitable for simple hierarchical data, like expression trees or basic search trees. | Ideal for databases and file systems, where large datasets require efficient disk access. |
| **Disk I/O** | Less optimized for disk access; more nodes may need to be read. | Optimized for disk access; fewer nodes read due to high branching factor. |
| **Branching Factor** | Fixed at 2 (binary). | Variable, typically high (e.g., 100+), reducing tree height. |
| **Examples** | Binary Search Tree, AVL Tree, Red-Black Tree. | B+ Tree, B* Tree, used in databases like MySQL. |

### <B>We will cover the following Trees in this B-Tree Topic:

- B-tree
- B+ tree
- B*-tree
- 2–3 tree
- 2–3–4 tree
- Fusion tree <span style="color:#FF0000">(COMPLEX, NOT IMPLEMENTED)</span>

> **Note:** I currently do not intend to implement this, as it has already been implemented in [fusiontree](https://github.com/6851-2017/fusiontree).


<a id="b-tree"></a>
## 1. B-Tree

### Definition
A B-tree is a self-balancing tree data structure that maintains sorted data and allows searches, sequential access, insertions, and deletions in logarithmic time. It is optimized for systems that read and write large blocks of data and is commonly used in databases and file systems.

### Properties
- Every node has at most `m` children
- Every non-leaf node (except the root) has at least ⌈m/2⌉ children
- The root has at least 2 children if it is not a leaf node
- All leaves appear on the same level
- A non-leaf node with k children contains k-1 keys

### Structure
In a B-tree of order m:
- Each internal node can contain up to m-1 keys
- Each internal node (except the root) must contain at least ⌈m/2⌉-1 keys
- All leaf nodes must be at the same level (height)
- Keys within a node are sorted in ascending order

### Visual Representation
Example of a B-tree of order 5:

```
         [10, 20, 30]
        /    |    \    \
       /     |     \     \
  [1,5,8]  [12,15] [22,25] [31,37,42]
```

### Operations

#### Search
1. Start at the root node
2. Compare the search key with the keys in the current node
3. If found, return the key
4. If not found, follow the appropriate pointer to a child node
5. Repeat until found or reach a leaf node without finding the key

Time Complexity: O(log n)

#### Insertion
1. Search for the key to be inserted
2. If the key already exists, insertion fails
3. If the node has space, insert the key in the correct position
4. If the node is full, split the node and propagate changes upward

Time Complexity: O(log n)

#### Deletion
1. Search for the key to be deleted
2. If the key is in a leaf node, simply delete it
3. If the key is in an internal node, replace it with its successor or predecessor
4. Rebalance the tree if necessary by merging or redistributing keys

Time Complexity: O(log n)

### Applications
- Databases (MySQL, PostgreSQL)
- File systems (NTFS, HFS+)
- Secondary storage retrieval systems

<a id="b-tree-1"></a>
## 2. B+ Tree

### Definition
A B+ tree is a variation of the B-tree that only stores keys in internal nodes, while storing all data (keys and values) in leaf nodes. Additionally, all leaf nodes are linked together in a linked list, facilitating sequential scanning.

### Properties
- All leaf nodes are at the same level
- Internal nodes store only keys, not data values
- All data records are stored in leaf nodes
- Leaf nodes are linked together in a linked list
- Keys in internal nodes can appear in leaf nodes

### Structure
In a B+ tree:
- Internal nodes contain only keys and pointers to child nodes
- Leaf nodes contain keys and data or pointers to data
- Leaf nodes are connected in a linked list for sequential access
- Every key in an internal node appears as the largest key in its left subtree

### Visual Representation
Example of a B+ tree:

```
             [15, 30]
            /    |    \
           /     |     \
     [5, 10] [15, 20, 25] [30, 35, 40]
       /  \      /  |  \      /  |  \
  data  data  data data data data data data
     
  Leaf nodes linked: [5,10] → [15,20,25] → [30,35,40]
```

### Operations

#### Search
1. Start at the root and traverse down to the leaf node
2. In internal nodes, use keys to determine the path
3. In leaf nodes, search for the key linearly
4. For range queries, find the first key and follow the linked list

Time Complexity: O(log n)

#### Insertion
1. Traverse to the leaf node where the key should be inserted
2. Insert the key in the correct position
3. If the leaf node overflows, split it and propagate changes upward
4. Update the linked list pointers

Time Complexity: O(log n)

#### Deletion
1. Find the leaf node containing the key
2. Delete the key
3. If the node becomes underfull, borrow from siblings or merge
4. Update internal nodes if necessary
5. Maintain the linked list integrity

Time Complexity: O(log n)

### Advantages over B-tree
- Better for range queries due to linked leaves
- Higher fanout for internal nodes (more child pointers per node)
- Consistent query times regardless of data location

### Applications
- Database indexing systems
- File systems (especially for range queries)
- Directory indexing

<a id="b-tree-2"></a>
## 3. B*-Tree

### Definition
A B*-tree is a variant of the B-tree that increases space utilization and further reduces the need for rebalancing operations. It maintains higher occupancy in nodes by redistributing keys before splitting nodes.

### Properties
- Nodes are at least 2/3 full (compared to 1/2 in B-trees)
- When a node gets full, keys are redistributed with siblings before splitting
- Only when two adjacent nodes are full do they split into three nodes
- More efficient space utilization than standard B-trees

### Structure
Similar to B-trees, but with stricter fill factor requirements:
- Internal nodes must be at least 2/3 full instead of 1/2 full
- Nodes split only when necessary after attempting redistribution

### Visual Representation
B*-tree has similar visualization to B-tree but maintains higher node occupancy:

```
          [20, 40]
         /    |    \
        /     |     \
  [10,15]  [25,30,35]  [45,50,55]
```

### Operations

#### Search
Identical to B-tree search.
Time Complexity: O(log n)

#### Insertion
1. Search for the appropriate leaf node
2. If not full, insert the key in sorted position
3. If full, attempt to redistribute keys with adjacent siblings
4. Only if redistribution isn't possible, perform a split into three nodes

Time Complexity: O(log n)

#### Deletion
Similar to B-tree deletion, but with additional redistribution steps.
Time Complexity: O(log n)

### Advantages
- Higher space utilization (at least 66% vs. 50% for B-trees)
- Fewer node splits
- More stable performance under heavy insertions/deletions

### Applications
- Database systems with high update rates
- File systems requiring high space utilization
- Systems where split/merge operations are costly

<a id="2-3-tree"></a>
## 4. (2-3 Tree)

### Definition
A 2-3 tree is a specific type of B-tree where each internal node has either 2 or 3 children (hence the name). It is a perfectly balanced tree where all leaf nodes are at the same level.

### Properties
- Every internal node has either 2 or 3 children
- A 2-node contains 1 key and has 2 children
- A 3-node contains 2 keys and has 3 children
- All leaf nodes are at the same level (perfect balance)
- Keys are maintained in ascending order

### Structure
In a 2-3 tree:
- 2-nodes have 1 key and 2 children
- 3-nodes have 2 keys and 3 children
- All paths from root to any leaf have the same length

### Visual Representation
Example of a 2-3 tree:

```
              [15]
            /      \
           /        \
       [5,10]      [20,25]
      /  |  \      /  |  \
     1   7  12    17  22  30
```

### Operations

#### Search
1. Start at the root node
2. Compare the search key with the keys in the node
3. Follow the appropriate branch based on the comparison
4. Repeat until found or reach a leaf node

Time Complexity: O(log n)

#### Insertion
1. Search to find the appropriate leaf node
2. Insert the key in the correct position
3. If the node becomes a 4-node (3 keys), split it into two 2-nodes
4. Propagate the middle key upward
5. Continue splitting as needed, potentially growing the tree height

Time Complexity: O(log n)

#### Deletion
1. If the key is in a leaf, remove it
2. If removal creates an empty node, borrow from siblings or merge
3. If the key is in an internal node, replace with successor/predecessor
4. Ensure all nodes have proper number of children

Time Complexity: O(log n)

### Properties as a Special Case
- A 2-3 tree is a B-tree of order 3
- Height is logarithmic relative to the number of elements
- Perfect balance ensures consistent search times

### Applications
- Used as the foundation for Red-Black trees
- Educational purposes (simpler to understand than general B-trees)
- Implementation of set and map data structures

<a id="2-3-4-tree"></a>
## 5. (2-3-4 Tree)

### Definition
A 2-3-4 tree is a self-balancing search tree where each node can have 2, 3, or 4 children (and correspondingly 1, 2, or 3 keys). It's a special case of a B-tree of order 4.

### Properties
- Every internal node has 2, 3, or 4 children
- A node with k children contains k-1 keys
- All leaf nodes appear at the same level
- Keys within a node are in ascending order
- For each node, all keys in left subtree < node's first key < all keys in middle subtree < node's second key (if any) < all keys in right subtree

### Structure
In a 2-3-4 tree:
- 2-nodes: 1 key, 2 children
- 3-nodes: 2 keys, 3 children
- 4-nodes: 3 keys, 4 children

### Visual Representation
Example of a 2-3-4 tree:

```
              [10, 20]
            /     |     \
           /      |      \
      [5,7]     [15]    [30,40,50]
     / |  \     /  \    /   |   \  \
    1  6   8   12  17  25  35  45  60
```

### Operations

#### Search
Similar to B-tree search.
Time Complexity: O(log n)

#### Insertion
1. Search for the insertion position
2. If inserting into a 4-node, split it first (bottom-up approach)
3. After splitting, insert the key in the appropriate position
4. Continue splitting 4-nodes upward as needed

Time Complexity: O(log n)

#### Deletion
1. Find the node containing the key
2. If it's a leaf, remove it
3. If it's an internal node, replace with successor/predecessor
4. Ensure nodes don't become too small by merging or redistributing

Time Complexity: O(log n)

### Relationship to Red-Black Trees
- 2-3-4 trees have a direct correspondence to Red-Black trees
- Each 2-3-4 tree can be represented as a Red-Black tree
- Operations on 2-3-4 trees can be translated to equivalent operations on Red-Black trees

### Applications
- Foundation for understanding Red-Black trees
- Implementation of balanced search trees
- Used in memory-efficient database systems

<a id="fusion-tree"></a>
## 6. Fusion Tree

### Definition
A fusion tree is an advanced data structure that combines aspects of B-trees with word-level parallelism to achieve better theoretical performance than traditional comparison-based trees in certain scenarios.

### Properties
- Achieves O(log n / log log n) search time
- Uses word-level parallelism (bit manipulation)
- Combines ideas from B-trees with word operations
- Theoretically faster than standard comparison-based trees

### Technical Details
Fusion trees use several specialized techniques:
- **Sketching**: Compressing multiple keys into a single machine word
- **Parallel comparison**: Comparing multiple keys simultaneously using word operations
- **Approximate median finding**: For efficient splitting
- **Packed B-trees**: High-degree B-trees with compressed keys

### Operations

#### Search
Uses parallelism to compare multiple keys at once:
1. At each node, use sketching to compress the comparison
2. Use word operations to determine the appropriate child
3. Navigate through the tree with fewer comparisons

Time Complexity: O(log n / log log n)

#### Insertion and Deletion
1. Search for the position
2. Modify the structure using similar sketching techniques
3. Rebalance as needed

Time Complexity: O(log n / log log n)

### Theoretical Significance
- Represents a theoretical breakthrough in search data structures
- Demonstrates how word-level parallelism can improve performance
- Serves as a bridge between comparison-based and non-comparison data structures

### Practical Limitations
- Implementation complexity
- Hardware-dependent performance
- Constant factors often make traditional B-trees more practical

### Applications
- Primarily of theoretical interest
- Specialized applications in computational geometry
- Foundation for other theoretically efficient data structures
<br><br>
<a id="comparison"></a>
#  Comparison of Tree Structures

| Tree Structure | Node Capacity | Min Fill Factor | Key Location | Height | Typical Use Cases |
|----------------|---------------|----------------|--------------|--------|-------------------|
| B-Tree         | m-1 keys      | 1/2 (except root) | All nodes | O(log n) | Databases, file systems |
| B+ Tree        | m-1 keys      | 1/2 (except root) | Data only in leaves | O(log n) | Databases (indexes), file systems |
| B* Tree        | m-1 keys      | 2/3 (except root) | All nodes | O(log n) | High-update databases |
| 2-3 Tree       | 1-2 keys      | 1/2 | All nodes | O(log n) | Foundation for RB trees |
| 2-3-4 Tree     | 1-3 keys      | 1/2 | All nodes | O(log n) | Foundation for RB trees |
| Fusion Tree    | O(w) keys     | 1/2 | All nodes | O(log n / log log n) | Theoretical applications |

*Note: w represents the word size in bits in the fusion tree.*

### Performance Comparison

| Operation | B-Tree | B+ Tree | B* Tree | 2-3 Tree | 2-3-4 Tree | Fusion Tree |
|-----------|--------|---------|---------|----------|------------|-------------|
| Search    | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | O(log n / log log n) |
| Insert    | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | O(log n / log log n) |
| Delete    | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | O(log n / log log n) |
| Sequential Access | O(n) | O(n) | O(n) | O(n) | O(n) | O(n) |
| Range Query | O(log n + k) | O(log n + k) | O(log n + k) | O(log n + k) | O(log n + k) | O(log n / log log n + k) |

*Note: k represents the number of elements in the range.*



### Trade-offs

- **B-Tree**: Good general-purpose balance between search and update performance
- **B+ Tree**: Optimized for range queries and sequential access
- **B* Tree**: Better space utilization, fewer structural modifications
- **2-3 Tree**: Conceptually simpler, foundation for other data structures
- **2-3-4 Tree**: Easier top-down rebalancing, direct mapping to Red-Black trees
- **Fusion Tree**: Theoretically faster, but complex implementation









