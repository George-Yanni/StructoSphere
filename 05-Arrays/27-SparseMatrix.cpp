#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Sparse Matrix Example (4x4 matrix)
    // Matrix:
    // [ 0, 0, 0, 1]
    // [ 0, 2, 0, 0]
    // [ 3, 0, 0, 4]
    // [ 0, 0, 5, 0]

    // Storing non-zero elements (COO format)
    vector<int> rowIndices = {0, 1, 2, 2, 3};   // Row indices
    vector<int> colIndices = {3, 1, 0, 3, 2};   // Column indices
    vector<int> values = {1, 2, 3, 4, 5};       // Non-zero values

    // Display the sparse matrix in COO format
    cout << "Sparse Matrix (COO format):" << endl;
    for (size_t i = 0; i < values.size(); ++i) {
        cout << "Row: " << rowIndices[i] << ", Col: " << colIndices[i] << ", Value: " << values[i] << endl;
    }

    return 0;
}
