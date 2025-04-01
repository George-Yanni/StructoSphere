#include <iostream>
#include <map>
#include <string>
#include <iomanip>

using namespace std;

class BudgetTracker {
private:
    // Map to store category -> total expense amount
    map<string, double> expenses;

public:
    // Add an expense to a category
    void addExpense(const string& category, double amount) {
        if (amount < 0) {
            cout << "Warning: Amount cannot be negative. Ignoring expense." << endl;
            return;
        }

        // If category exists, add to it; if not, creates new entry
        expenses[category] += amount;
        cout << "Added $" << fixed << setprecision(2) << amount 
             << " to " << category << endl;
    }

    // Remove or adjust an expense (e.g., refund)
    void removeExpense(const string& category, double amount) {
        auto it = expenses.find(category);
        if (it == expenses.end()) {
            cout << "Category '" << category << "' not found." << endl;
            return;
        }

        if (amount > it->second) {
            cout << "Cannot remove $" << fixed << setprecision(2) << amount 
                 << " from " << category << " (only $" << it->second << " spent)." << endl;
            return;
        }

        it->second -= amount;
        cout << "Removed $" << amount << " from " << category << endl;

        // Clean up if category total becomes 0
        if (it->second == 0) {
            expenses.erase(it);
            cout << "Category '" << category << "' removed (no expenses left)." << endl;
        }
    }

    // Display all expenses and total
    void displayBudget() const {
        if (expenses.empty()) {
            cout << "No expenses recorded yet." << endl;
            return;
        }

        double total = 0.0;
        cout << "\n=== MONTHLY BUDGET ===" << endl;
        for (const auto& [category, amount] : expenses) {
            cout << category << ": $" << fixed << setprecision(2) << amount << endl;
            total += amount;
        }
        cout << "----------------" << endl;
        cout << "Total Spending: $" << fixed << setprecision(2) << total << endl;
    }

    // Get spending for a specific category
    double getCategorySpending(const string& category) const {
        auto it = expenses.find(category);
        return (it != expenses.end()) ? it->second : 0.0;
    }
};

int main() {
    BudgetTracker budget;

    // Adding some typical monthly expenses
    budget.addExpense("Food", 45.75);        // Dinner
    budget.addExpense("Rent", 1200.00);      // Monthly rent
    budget.addExpense("Entertainment", 12.50); // Movie ticket
    budget.addExpense("Food", 23.40);        // Groceries
    budget.addExpense("Transport", 15.00);   // Bus fare

    // Display initial budget
    budget.displayBudget();

    // Remove an expense (e.g., refund)
    budget.removeExpense("Entertainment", 12.50);

    // Check spending in a category
    cout << "\nFood spending: $" << fixed << setprecision(2) 
         << budget.getCategorySpending("Food") << endl;

    // Display updated budget
    budget.displayBudget();

    // Try some edge cases
    budget.addExpense("Utilities", -5.00);   // Negative amount
    budget.removeExpense("Rent", 2000.00);   // Too large removal

    return 0;
}