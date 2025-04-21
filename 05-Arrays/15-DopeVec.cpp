#include <vector>
#include <stdexcept>
#include <iostream>

// DopeVector class to manage multidimensional array metadata and operations
template <typename T>
class DopeVector {
private:
    T* data;                          // Pointer to the array's data (base address)
    std::vector<size_t> extents;      // Extents (size) of each dimension
    std::vector<size_t> strides;      // Strides (memory offset) for each dimension
    size_t rank;                      // Number of dimensions
    size_t total_size;                // Total number of elements
    std::vector<size_t> lower_bounds; // Lower bounds for each dimension (default 0)

    // Calculate total size and strides based on extents
    void initializeStridesAndSize() {
        total_size = 1;
        strides.resize(rank);
        strides[rank - 1] = 1; // Innermost dimension has stride 1
        for (size_t i = rank - 1; i > 0; --i) {
            strides[i - 1] = strides[i] * extents[i];
            total_size *= extents[i];
        }
        total_size *= extents[0];
    }

public:
    // Constructor: Initialize with extents and optional lower bounds
    DopeVector(const std::vector<size_t>& extents_, const std::vector<size_t>& lower_bounds_ = {})
        : extents(extents_), rank(extents_.size()), lower_bounds(extents_.size(), 0) {
        if (rank == 0) {
            throw std::invalid_argument("DopeVector must have at least one dimension.");
        }
        for (size_t ext : extents) {
            if (ext == 0) {
                throw std::invalid_argument("Extents must be non-zero.");
            }
        }

        // Set lower bounds if provided, otherwise default to 0
        if (!lower_bounds_.empty()) {
            if (lower_bounds_.size() != rank) {
                throw std::invalid_argument("Lower bounds size must match rank.");
            }
            lower_bounds = lower_bounds_;
        }

        initializeStridesAndSize();

        // Allocate memory
        data = new T[total_size]();
    }

    // Destructor: Free allocated memory
    ~DopeVector() {
        delete[] data;
    }

    // Copy constructor
    DopeVector(const DopeVector& other)
        : extents(other.extents), strides(other.strides), rank(other.rank),
          total_size(other.total_size), lower_bounds(other.lower_bounds) {
        data = new T[total_size];
        std::copy(other.data, other.data + total_size, data);
    }

    // Copy assignment operator
    DopeVector& operator=(const DopeVector& other) {
        if (this != &other) {
            delete[] data;
            extents = other.extents;
            strides = other.strides;
            rank = other.rank;
            total_size = other.total_size;
            lower_bounds = other.lower_bounds;
            data = new T[total_size];
            std::copy(other.data, other.data + total_size, data);
        }
        return *this;
    }

    // Access element at given indices with bounds checking
    T& operator()(const std::vector<size_t>& indices) {
        if (indices.size() != rank) {
            throw std::out_of_range("Number of indices does not match rank.");
        }

        size_t offset = 0;
        for (size_t i = 0; i < rank; ++i) {
            size_t idx = indices[i];
            if (idx < lower_bounds[i] || idx >= lower_bounds[i] + extents[i]) {
                throw std::out_of_range("Index out of bounds.");
            }
            offset += (idx - lower_bounds[i]) * strides[i];
        }
        return data[offset];
    }

    // Const version of element access
    const T& operator()(const std::vector<size_t>& indices) const {
        if (indices.size() != rank) {
            throw std::out_of_range("Number of indices does not match rank.");
        }

        size_t offset = 0;
        for (size_t i = 0; i < rank; ++i) {
            size_t idx = indices[i];
            if (idx < lower_bounds[i] || idx >= lower_bounds[i] + extents[i]) {
                throw std::out_of_range("Index out of bounds.");
            }
            offset += (idx - lower_bounds[i]) * strides[i];
        }
        return data[offset];
    }

    // Get rank (number of dimensions)
    size_t getRank() const { return rank; }

    // Get extents
    const std::vector<size_t>& getExtents() const { return extents; }

    // Get strides
    const std::vector<size_t>& getStrides() const { return strides; }

    // Get lower bounds
    const std::vector<size_t>& getLowerBounds() const { return lower_bounds; }

    // Get total size
    size_t getTotalSize() const { return total_size; }

    // Slice operation: Extract a subarray
    DopeVector<T> slice(const std::vector<size_t>& start,
                        const std::vector<size_t>& slice_extents) const {
        if (start.size() != rank || slice_extents.size() != rank) {
            throw std::invalid_argument("Start or slice extents size must match rank.");
        }

        // Validate slice bounds
        for (size_t i = 0; i < rank; ++i) {
            if (start[i] < lower_bounds[i] ||
                start[i] + slice_extents[i] > lower_bounds[i] + extents[i]) {
                throw std::out_of_range("Slice bounds out of range.");
            }
            if (slice_extents[i] == 0) {
                throw std::invalid_argument("Slice extents must be non-zero.");
            }
        }

        // Create new dope vector for the slice
        DopeVector<T> result(slice_extents, start);

        // Copy data to the new dope vector
        std::vector<size_t> indices(rank);
        for (size_t i = 0; i < rank; ++i) {
            indices[i] = start[i];
        }

        // Recursive copy for simplicity (could be optimized for performance)
        copySliceData(result, indices, start, slice_extents, 0);

        return result;
    }

private:
    // Helper function to copy data for slicing
    void copySliceData(DopeVector<T>& result, std::vector<size_t>& indices,
                       const std::vector<size_t>& start,
                       const std::vector<size_t>& slice_extents,
                       size_t dim) const {
        if (dim == rank - 1) {
            for (size_t i = 0; i < slice_extents[dim]; ++i) {
                indices[dim] = start[dim] + i;
                result(indices) = (*this)(indices);
            }
        } else {
            for (size_t i = 0; i < slice_extents[dim]; ++i) {
                indices[dim] = start[dim] + i;
                copySliceData(result, indices, start, slice_extents, dim + 1);
            }
        }
    }
};

// Example usage
int main() {
    try {
        // Create a 3x4 2D dope vector with lower bounds [1, 1]
        std::vector<size_t> extents = {3, 4};
        std::vector<size_t> lower_bounds = {1, 1};
        DopeVector<int> array(extents, lower_bounds);

        // Populate the array
        for (size_t i = 1; i <= 3; ++i) {
            for (size_t j = 1; j <= 4; ++j) {
                array({i, j}) = i * 10 + j;
            }
        }

        // Print the array
        std::cout << "Original 3x4 array:\n";
        for (size_t i = 1; i <= 3; ++i) {
            for (size_t j = 1; j <= 4; ++j) {
                std::cout << array({i, j}) << " ";
            }
            std::cout << "\n";
        }

        // Perform a slice operation (2x2 subarray starting at [2, 2])
        std::vector<size_t> start = {2, 2};
        std::vector<size_t> slice_extents = {2, 2};
        DopeVector<int> sliced = array.slice(start, slice_extents);

        // Print the sliced array
        std::cout << "\nSliced 2x2 array (starting at [2, 2]):\n";
        for (size_t i = 2; i <= 3; ++i) {
            for (size_t j = 2; j <= 3; ++j) {
                std::cout << sliced({i, j}) << " ";
            }
            std::cout << "\n";
        }

        // Demonstrate bounds checking
        try {
            std::cout << "\nTrying to access out-of-bounds element...\n";
            array({4, 4});
        } catch (const std::out_of_range& e) {
            std::cout << "Caught error: " << e.what() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}