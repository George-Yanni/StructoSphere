#include <iostream>
#include <complex>
#include <iomanip>
using namespace std;

int main()
{
    // Boolean: Can hold true or false values
    bool isCodingFun = true;
    cout << "Boolean example: isCodingFun = " << (isCodingFun ? "true" : "false") << endl;

    // Character: Stores a single character
    char letter = 'A';
    cout << "Character example: letter = " << letter << endl;

    // Floating-point numbers (Single and Double precision)
    float singlePrecision = 3.14159f;           // 32-bit precision
    double doublePrecision = 3.141592653589793; // 64-bit precision
    cout << "Floating-point examples: " << singlePrecision << " (float), " << doublePrecision << " (double)" << endl;
    // Fixed-point numbers (Represented as integers in C++, used in financial applications)
    int fixedPointNumber = 12345; // Assume scaled by 100 for two decimal places (e.g., 123.45)
    cout << "Fixed-point representation example: " << fixedPointNumber << " (scaled integer)" << endl;

    // Integer types: Signed and Unsigned
    int signedInteger = -42;
    unsigned int unsignedInteger = 42;
    cout << "Integer examples: " << signedInteger << " (signed), " << unsignedInteger << " (unsigned)" << endl;

    // Reference: A reference points to another variable
    int originalValue = 100;
    int &refValue = originalValue;
    cout << "Reference example: originalValue = " << originalValue << ", refValue = " << refValue << endl;
    refValue = 200; // Changing refValue also changes originalValue
    cout << "After modifying refValue: originalValue = " << originalValue << endl;

    // Symbol (In C++, an enumeration can serve a similar role to symbols in other languages)
    enum Colors
    {
        RED,
        GREEN,
        BLUE
    };
    Colors myColor = GREEN;
    cout << "Enum example: myColor = " << myColor << " (0 for RED, 1 for GREEN, 2 for BLUE)" << endl;

    // Enumerated type (Defines a set of named values)
    enum Days
    {
        SUNDAY,
        MONDAY,
        TUESDAY
    };
    Days today = MONDAY;
    cout << "Enumerated type example: today = " << today << endl;

    // Complex numbers (Using the complex library)
    complex<double> complexNumber(3.0, 4.0); // 3 + 4i
    cout << "Complex number example: " << complexNumber << " (Real: " << complexNumber.real() << ", Imaginary: " << complexNumber.imag() << ")" << endl;

    return 0;
}