#include <iostream>
#include <vector>

using namespace std;

// Function to print a matrix
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int elem : row) {
            cout << elem << " ";
        }
        cout << endl;
    }
}

// Function to add two matrices
vector<vector<int>> addMatrices(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB) {
    int rows = matrixA.size();
    int cols = matrixA[0].size();
    vector<vector<int>> result(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }

    return result;
}

// Function to multiply a matrix by a scalar
vector<vector<int>> scalarMultiply(const vector<vector<int>>& matrix, int scalar) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<vector<int>> result(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrix[i][j] * scalar;
        }
    }

    return result;
}

// Function to multiply two matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB) {
    int rowsA = matrixA.size();
    int colsA = matrixA[0].size();
    int rowsB = matrixB.size();
    int colsB = matrixB[0].size();

    if (colsA != rowsB) {
        throw invalid_argument("Number of columns in matrixA must be equal to number of rows in matrixB");
    }

    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return result;
}

int main() {
    // Creating two 2x2 matrices
    vector<vector<int>> matrixA = {{1, 2}, {3, 4}};
    vector<vector<int>> matrixB = {{5, 6}, {7, 8}};

    // Printing the matrices
    cout << "Matrix A:" << endl;
    printMatrix(matrixA);
    cout << "\nMatrix B:" << endl;
    printMatrix(matrixB);

    // Matrix Addition
    vector<vector<int>> matrixSum = addMatrices(matrixA, matrixB);
    cout << "\nMatrix A + Matrix B:" << endl;
    printMatrix(matrixSum);

    // Scalar Multiplication
    int scalar = 3;
    vector<vector<int>> matrixScalarMult = scalarMultiply(matrixA, scalar);
    cout << "\n3 * Matrix A:" << endl;
    printMatrix(matrixScalarMult);

    // Matrix Multiplication
    vector<vector<int>> matrixProduct = multiplyMatrices(matrixA, matrixB);
    cout << "\nMatrix A * Matrix B:" << endl;
    printMatrix(matrixProduct);

    return 0;
}
