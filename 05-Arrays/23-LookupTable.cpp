#include <iostream>
#include <vector>

// Function to compute factorial (used to populate the lookup table)
unsigned long long computeFactorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Function to retrieve factorial from the lookup table
unsigned long long getFactorial(const std::vector<unsigned long long>& lookupTable, int n) {
    if (n < 0 || n >= lookupTable.size()) {
        std::cerr << "Error: Input out of range for the lookup table." << std::endl;
        return 0;
    }
    return lookupTable[n];
}

int main() {
    // Define the range for the lookup table
    const int maxRange = 10;

    // Create and populate the lookup table
    std::vector<unsigned long long> factorialLookupTable(maxRange + 1);
    for (int i = 0; i <= maxRange; ++i) {
        factorialLookupTable[i] = computeFactorial(i);
    }

    // Retrieve and print factorials using the lookup table
    int number;
    std::cout << "Enter a number between 0 and 10 to get its factorial: ";
    std::cin >> number;

    unsigned long long factorial = getFactorial(factorialLookupTable, number);
    if (factorial != 0) {
        std::cout << "Factorial of " << number << " is " << factorial << std::endl;
    }

    return 0;
}
