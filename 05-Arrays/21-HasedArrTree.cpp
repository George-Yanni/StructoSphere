#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <limits>
#include <algorithm>

// Hashed Array Tree (HAT): A dynamic array with O(1) access and amortized O(1) insertion.
// Uses a top-level array of pointers to fixed-size leaf arrays.
template <typename T>
class HashedArrayTree {
private:
    std::vector<std::unique_ptr<T[]>> top; // Top-level array (stores leaf arrays)
    size_t capacity;                       // Total capacity (top_size * leaf_size)
    size_t top_size;                       // Size of the top-level array
    size_t leaf_size;                      // Size of each leaf array
    size_t size_;                          // Number of elements stored

    // Expand the HAT by doubling the top-level array
    void expand() {
        size_t new_top_size = top_size * 2;
        std::vector<std::unique_ptr<T[]>> new_top(new_top_size);

        // Move existing leaves to new top array
        for (size_t i = 0; i < top_size; ++i) {
            new_top[i] = std::move(top[i]);
        }

        top = std::move(new_top);
        top_size = new_top_size;
        capacity = top_size * leaf_size;
    }

    // Reallocate the leaf array at the given index to the new leaf size
    void reallocate_leaf(size_t top_idx) {
        auto new_leaf = std::make_unique<T[]>(leaf_size);
        for (size_t j = 0; j < leaf_size / 2 && top_idx * (leaf_size / 2) + j < size_; ++j) {
            new_leaf[j] = top[top_idx][j];
        }
        top[top_idx] = std::move(new_leaf);
    }

public:
    // Constructor
    HashedArrayTree(size_t initial_leaf_size = 1)
        : top_size(1), leaf_size(initial_leaf_size), size_(0), capacity(initial_leaf_size) {
        top.push_back(std::make_unique<T[]>(leaf_size));
    }

    // Copy constructor
    HashedArrayTree(const HashedArrayTree& other)
        : top_size(other.top_size), leaf_size(other.leaf_size),
          size_(other.size_), capacity(other.capacity) {
        top.resize(top_size);
        for (size_t i = 0; i < top_size; ++i) {
            if (other.top[i]) {
                top[i] = std::make_unique<T[]>(leaf_size);
                std::copy(other.top[i].get(), other.top[i].get() + leaf_size, top[i].get());
            }
        }
    }

    // Copy assignment operator
    HashedArrayTree& operator=(const HashedArrayTree& other) {
        if (this != &other) {
            HashedArrayTree temp(other);
            std::swap(top, temp.top);
            std::swap(top_size, temp.top_size);
            std::swap(leaf_size, temp.leaf_size);
            std::swap(size_, temp.size_);
            std::swap(capacity, temp.capacity);
        }
        return *this;
    }

    // Move constructor
    HashedArrayTree(HashedArrayTree&& other) noexcept
        : top(std::move(other.top)), top_size(other.top_size),
          leaf_size(other.leaf_size), size_(other.size_),
          capacity(other.capacity) {
        other.top_size = 1;
        other.leaf_size = 1;
        other.size_ = 0;
        other.capacity = 1;
        other.top = std::vector<std::unique_ptr<T[]>>(1);
    }

    // Move assignment operator
    HashedArrayTree& operator=(HashedArrayTree&& other) noexcept {
        if (this != &other) {
            top = std::move(other.top);
            top_size = other.top_size;
            leaf_size = other.leaf_size;
            size_ = other.size_;
            capacity = other.capacity;
            other.top_size = 1;
            other.leaf_size = 1;
            other.size_ = 0;
            other.capacity = 1;
            other.top = std::vector<std::unique_ptr<T[]>>(1);
        }
        return *this;
    }

    // Destructor (handled automatically by unique_ptr)
    ~HashedArrayTree() = default;

    // Insert an element at the end (amortized O(1))
    void push_back(const T& value) {
        if (size_ >= std::numeric_limits<size_t>::max() - 1) {
            throw std::overflow_error("HashedArrayTree size overflow");
        }

        if (size_ == capacity) {
            if (size_ == top_size * leaf_size) {
                expand(); // Double the top array
            } else {
                // Double leaf size and reallocate existing leaf if necessary
                size_t old_leaf_size = leaf_size;
                leaf_size *= 2;
                capacity = top_size * leaf_size;

                // Reallocate the current leaf to the new size
                size_t top_idx = size_ / old_leaf_size;
                reallocate_leaf(top_idx);
            }
        }

        size_t top_idx = size_ / leaf_size;
        size_t leaf_idx = size_ % leaf_size;

        if (!top[top_idx]) {
            top[top_idx] = std::make_unique<T[]>(leaf_size);
        }

        top[top_idx][leaf_idx] = value;
        size_++;
    }

    // Access element at index (O(1))
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        size_t top_idx = index / leaf_size;
        size_t leaf_idx = index % leaf_size;
        if (!top[top_idx]) {
            throw std::runtime_error("Accessing uninitialized leaf");
        }
        return top[top_idx][leaf_idx];
    }

    // Const access element at index (O(1))
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        size_t top_idx = index / leaf_size;
        size_t leaf_idx = index % leaf_size;
        if (!top[top_idx]) {
            throw std::runtime_error("Accessing uninitialized leaf");
        }
        return top[top_idx][leaf_idx];
    }

    // Get current size
    size_t size() const { return size_; }

    // Get current capacity
    size_t getCapacity() const { return capacity; }
};

int main() {
    HashedArrayTree<int> hat;

    // Insert elements
    for (int i = 0; i < 20; ++i) {
        hat.push_back(i * 1);
        std::cout << "Inserted: " << i * 1
                  << ", Size: " << hat.size()
                  << ", Capacity: " << hat.getCapacity() << std::endl;
    }

    // Access elements
    std::cout << "\nElements: ";
    for (size_t i = 0; i < hat.size(); ++i) {
        std::cout << hat[i] << " ";
    }
    std::cout << std::endl;

    // Test copy constructor
    HashedArrayTree<int> hat_copy = hat;
    std::cout << "\nCopied HAT Elements: ";
    for (size_t i = 0; i < hat_copy.size(); ++i) {
        std::cout << hat_copy[i] << " ";
    }
    std::cout << std::endl;

    // Test move constructor
    HashedArrayTree<int> hat_moved = std::move(hat_copy);
    std::cout << "\nMoved HAT Elements: ";
    for (size_t i = 0; i < hat_moved.size(); ++i) {
        std::cout << hat_moved[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
