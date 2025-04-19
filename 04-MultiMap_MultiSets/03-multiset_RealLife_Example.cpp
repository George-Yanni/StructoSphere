// Inventory Management System

#include <iostream>
#include <set>
#include <string>

int main() {
    // Create a multiset to store phone models
    std::multiset<std::string> inventory;

    // Add phone models to the inventory (duplicates represent multiple units)
    inventory.insert("iPhone 13");
    inventory.insert("Samsung Galaxy S21");
    inventory.insert("iPhone 13");
    inventory.insert("Samsung Galaxy S21");
    inventory.insert("iPhone 13");
    inventory.insert("Google Pixel 6");
    inventory.insert("iPhone 13");
    inventory.insert("OnePlus 9");

    // Print out the inventory list (ordered by phone model)
    std::cout << "Inventory:\n";
    for (const auto& phone : inventory) {
        std::cout << phone << std::endl;
    }
    std::cout << std::endl;

    // Use a set to keep track of unique phone models
    std::set<std::string> uniquePhones(inventory.begin(), inventory.end());

    // Count the number of each phone in stock
    std::cout << "Phone Model Counts:\n";
    for (const auto& phone : uniquePhones) {
        std::cout << phone << ": " << inventory.count(phone) << " units\n";
    }

    

    return 0;
}
