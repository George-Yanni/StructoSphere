#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Generic Stack class template for storing elements
 * @tparam T The type of elements to store in the stack
 */
template <typename T>
class Stack {
private:
    T* data;              // Dynamic array to store stack elements
    int topIndex;         // Index of the top element
    int capacity;         // Current capacity of the stack
    static const int DEFAULT_CAPACITY = 10;  // Initial capacity

    /**
     * @brief Doubles the capacity of the stack when full
     */
    void resize() {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i <= topIndex; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }

public:
    /**
     * @brief Constructor - Initializes an empty stack
     */
    Stack() : topIndex(-1), capacity(DEFAULT_CAPACITY) {
        data = new T[capacity];
    }

    /**
     * @brief Destructor - Cleans up allocated memory
     */
    ~Stack() {
        delete[] data;
    }

    /**
     * @brief Pushes an element onto the stack
     * @param element The element to push
     */
    void push(const T& element) {
        if (topIndex + 1 == capacity) {
            resize();
        }
        data[++topIndex] = element;
    }

    /**
     * @brief Removes and returns the top element
     * @return The top element
     * @throws std::runtime_error if stack is empty
     */
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack underflow");
        }
        return data[topIndex--];
    }

    /**
     * @brief Returns the top element without removing it
     * @return The top element
     * @throws std::runtime_error if stack is empty
     */
    T top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data[topIndex];
    }

    /**
     * @brief Checks if the stack is empty
     * @return true if empty, false otherwise
     */
    bool isEmpty() const {
        return topIndex < 0;
    }

    /**
     * @brief Returns the current number of elements
     * @return Size of the stack
     */
    int size() const {
        return topIndex + 1;
    }

    /**
     * @brief Clears all elements from the stack
     */
    void clear() {
        topIndex = -1;
    }
};

/**
 * @brief Class to simulate browser navigation history
 */
class BrowserHistory {
private:
    Stack<std::string> backStack;    // Stack for pages visited before current
    Stack<std::string> forwardStack; // Stack for pages visited after going back
    std::string currentPage;         // Current page being viewed

public:
    /**
     * @brief Constructor - Initializes with a starting page
     * @param homePage The initial page
     */
    BrowserHistory(const std::string& homePage) : currentPage(homePage) {}

    /**
     * @brief Visits a new page, clearing forward history
     * @param url The URL of the new page
     */
    void visit(const std::string& url) {
        backStack.push(currentPage);  // Save current page to back history
        forwardStack.clear();         // New visit invalidates forward history
        currentPage = url;            // Update current page
    }

    /**
     * @brief Goes back to the previous page
     * @return The new current page after going back
     * @throws std::runtime_error if no previous page exists
     */
    std::string back() {
        if (backStack.isEmpty()) {
            throw std::runtime_error("No pages to go back to");
        }
        forwardStack.push(currentPage);  // Save current page to forward history
        currentPage = backStack.pop();   // Move to previous page
        return currentPage;
    }

    /**
     * @brief Goes forward to the next page
     * @return The new current page after going forward
     * @throws std::runtime_error if no forward page exists
     */
    std::string forward() {
        if (forwardStack.isEmpty()) {
            throw std::runtime_error("No pages to go forward to");
        }
        backStack.push(currentPage);     // Save current page to back history
        currentPage = forwardStack.pop(); // Move to next page
        return currentPage;
    }

    /**
     * @brief Gets the current page
     * @return The current page URL
     */
    std::string getCurrentPage() const {
        return currentPage;
    }

    /**
     * @brief Gets the number of pages in back history
     * @return Size of back stack
     */
    int backHistorySize() const {
        return backStack.size();
    }

    /**
     * @brief Gets the number of pages in forward history
     * @return Size of forward stack
     */
    int forwardHistorySize() const {
        return forwardStack.size();
    }
};

/**
 * @brief Main function to demonstrate browser history navigation
 */
int main() {
    try {
        // Initialize browser with home page
        BrowserHistory browser("home.com");
        std::cout << "Current page: " << browser.getCurrentPage() << "\n";

        // Simulate navigation
        browser.visit("page1.com");
        std::cout << "Visited: " << browser.getCurrentPage() << "\n";
        browser.visit("page2.com");
        std::cout << "Visited: " << browser.getCurrentPage() << "\n";
        browser.visit("page3.com");
        std::cout << "Visited: " << browser.getCurrentPage() << "\n";

        // Go back
        std::cout << "Going back to: " << browser.back() << "\n";
        std::cout << "Going back to: " << browser.back() << "\n";
        std::cout << "Back history size: " << browser.backHistorySize() << "\n";
        std::cout << "Forward history size: " << browser.forwardHistorySize() << "\n";

        // Go forward
        std::cout << "Going forward to: " << browser.forward() << "\n";
        std::cout << "Current page: " << browser.getCurrentPage() << "\n";
        std::cout << "Back history size: " << browser.backHistorySize() << "\n";
        std::cout << "Forward history size: " << browser.forwardHistorySize() << "\n";

        // Visit new page (clears forward history)
        browser.visit("page4.com");
        std::cout << "Visited: " << browser.getCurrentPage() << "\n";
        std::cout << "Forward history size after new visit: " << browser.forwardHistorySize() << "\n";

        // Try going back too far (will throw exception)
        std::cout << "Going back to: " << browser.back() << "\n";
        std::cout << "Going back to: " << browser.back() << "\n";
        std::cout << "Going back to: " << browser.back() << "\n";  // Exception here
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}