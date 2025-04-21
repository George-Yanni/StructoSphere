#include <iostream>
#include <vector>

// Define a class for a 2D Dope Vector-like structure
class DopeVector2D {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    // Constructor to initialize the dimensions of the matrix
    DopeVector2D(size_t r, size_t c) : rows(r), cols(c) {
        data.resize(r, std::vector<int>(c, 0));  // Fill with zeros
    }

    // Set value at specific position
    void setElement(size_t r, size_t c, int value) {
        if (r < rows && c < cols) {
            data[r][c] = value;
        } else {
            std::cerr << "Index out of bounds!\n";
        }
    }

    // Get value at specific position
    int getElement(size_t r, size_t c) const {
        if (r < rows && c < cols) {
            return data[r][c];
        } else {
            std::cerr << "Index out of bounds!\n";
            return -1;
        }
    }

    // Print the full matrix
    void print() const {
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                std::cout << data[r][c] << " ";
            }
            std::cout << '\n';
        }
    }
};

int main() {
    DopeVector2D image(5, 5);  // Create a 5x5 matrix

    std::cout << "Initial image (all zeros):\n";
    image.print();

    // Set some pixels
    image.setElement(2, 2, 255);  // Center pixel
    image.setElement(0, 0, 128);  // Top-left
    image.setElement(4, 4, 64);   // Bottom-right

    std::cout << "\nImage after pixel updates:\n";
    image.print();

    // Example of retrieving a pixel value
    std::cout << "\nPixel at (2, 2): " << image.getElement(2, 2) << "\n";

    return 0;
}
