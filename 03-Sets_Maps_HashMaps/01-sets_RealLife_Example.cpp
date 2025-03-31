#include <iostream>
#include <set>
#include <string>

// Utility function to print the set of employee IDs
void printEmployeeIDs(const std::set<int>& employeeIDs, const std::string& label) {
    std::cout << label << " (" << employeeIDs.size() << " employees):\n";
    for (const int& id : employeeIDs) {
        std::cout << "  Employee ID: " << id << "\n";
    }
    std::cout << "\n";
}

int main() {
    // Problem: A company needs to track unique employee IDs in their payroll system.
    // - Each employee has a unique integer ID.
    // - IDs must be stored in sorted order for reporting.
    // - No duplicate IDs are allowed (e.g., hiring the same person twice by mistake).
    // Solution: Use std::set<int> because it:
    // - Automatically ensures uniqueness (no duplicates).
    // - Keeps elements sorted in ascending order.
    // - Provides O(log n) time for insertion, deletion, and search.

    // Initialize an empty set to store employee IDs
    std::set<int> payroll;

    // 1. Adding new employees
    // - insert() adds an ID to the set in O(log n) time.
    // - If the ID already exists, it’s ignored (no duplicates).
    std::cout << "=== Adding Employees ===\n";
    payroll.insert(1001); // Add employee with ID 1001
    payroll.insert(1003); // Add employee with ID 1003
    payroll.insert(1002); // Add employee with ID 1002
    payroll.insert(1001); // Try adding 1001 again (will be ignored)
    printEmployeeIDs(payroll, "Initial Payroll");

    // 2. Checking if an employee ID exists
    // - find() searches for an ID in O(log n) time.
    // - Returns an iterator to the element or end() if not found.
    std::cout << "=== Checking Employee Existence ===\n";
    int searchID = 1002;
    if (payroll.find(searchID) != payroll.end()) {
        std::cout << "Employee ID " << searchID << " exists in payroll.\n";
    } else {
        std::cout << "Employee ID " << searchID << " not found.\n";
    }
    searchID = 1004;
    if (payroll.find(searchID) != payroll.end()) {
        std::cout << "Employee ID " << searchID << " exists in payroll.\n";
    } else {
        std::cout << "Employee ID " << searchID << " not found.\n";
    }
    std::cout << "\n";

    // 3. Removing an employee (e.g., termination)
    // - erase() removes an ID in O(log n) time.
    std::cout << "=== Removing an Employee ===\n";
    int terminatedID = 1003;
    size_t erasedCount = payroll.erase(terminatedID); // Returns number of elements removed (0 or 1)
    if (erasedCount > 0) {
        std::cout << "Employee ID " << terminatedID << " removed from payroll.\n";
    } else {
        std::cout << "Employee ID " << terminatedID << " was not in payroll.\n";
    }
    printEmployeeIDs(payroll, "Payroll After Termination");

    // 4. Finding the lowest and highest employee IDs
    // - begin() points to the smallest ID, rbegin() to the largest (O(1) access).
    std::cout << "=== Lowest and Highest IDs ===\n";
    if (!payroll.empty()) {
        std::cout << "Lowest Employee ID: " << *payroll.begin() << "\n";
        std::cout << "Highest Employee ID: " << *payroll.rbegin() << "\n";
    } else {
        std::cout << "Payroll is empty.\n";
    }
    std::cout << "\n";

    // 5. Clearing the payroll (e.g., end of year reset)
    // - clear() removes all elements in O(n) time.
    std::cout << "=== Clearing Payroll ===\n";
    payroll.clear();
    printEmployeeIDs(payroll, "Payroll After Clear");

    return 0;
}