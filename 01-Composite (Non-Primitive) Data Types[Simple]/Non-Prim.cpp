#include <iostream>
#include <tuple>   // For tuple
#include <variant> // For variant
#include <string>

using namespace std;

// 1. Structure (Record)
struct Person
{
    string name;
    int age;
    float height;
};

// 5. Union - One value at a time
union Data
{
    int intValue;
    float floatValue;
};

int main()
{
    // 1. Array - Sequence of elements stored contiguously
    int numbers[5] = {10, 20, 30, 40, 50};
    cout << "Array example: numbers[2] = " << numbers[2] << endl;

    // 2. Struct (Record) - Collection of fields
    Person p1 = {"Alice", 25, 5.6};
    cout << "Struct example: Name = " << p1.name << ", Age = " << p1.age << ", Height = " << p1.height << endl;

    // 3. Tuple - Product type (no named fields)
    tuple<int, string, double> data(1, "Alice", 5.6);
    cout << "Tuple example: ID = " << get<0>(data) << ", Name = " << get<1>(data) << ", Height = " << get<2>(data) << endl;

    // 4. String - Sequence of characters
    string message = "Hello, World!";
    cout << "String example: " << message << " (Length: " << message.length() << ")" << endl;

    // 5. Union - Stores only one value at a time
    Data d;
    d.intValue = 42;
    cout << "Union example: intValue = " << d.intValue << endl;
    d.floatValue = 3.14f; // Overwrites intValue
    cout << "Union example after assigning floatValue = " << d.floatValue << endl;
    cout << "intValue after the corruption made by assigning floatValue = " << d.intValue << endl; // Undefined behavior
    // Note: std::variant is available only in C++17 and later so compile with: g++ -std=c++17 Non-Prim.cpp -o Non-Prim
    //  6. Tagged Union (Variant)
    variant<int, string> var;
    var = 100;
    cout << "Variant example (int): " << get<int>(var) << endl;
    var = "Variant String";
    cout << "Variant example (string): " << get<string>(var) << endl;

    return 0;
}
