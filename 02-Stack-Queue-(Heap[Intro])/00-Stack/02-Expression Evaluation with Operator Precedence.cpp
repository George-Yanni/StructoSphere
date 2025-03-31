#include <iostream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <cmath>

/**
 * @brief Generic Stack class template for storing elements
 * @tparam T The type of elements to store in the stack
 */
template <typename T>
class Stack
{
private:
    T *data;                                // Dynamic array to store stack elements
    int topIndex;                           // Index of the top element
    int capacity;                           // Current capacity of the stack
    static const int DEFAULT_CAPACITY = 10; // Initial capacity

    /**
     * @brief Doubles the capacity of the stack when full
     */
    void resize()
    {
        capacity *= 2;
        T *temp = new T[capacity];
        for (int i = 0; i <= topIndex; i++)
        {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }

public:
    /**
     * @brief Constructor - Initializes an empty stack
     */
    Stack() : topIndex(-1), capacity(DEFAULT_CAPACITY)
    {
        data = new T[capacity];
    }

    /**
     * @brief Destructor - Cleans up allocated memory
     */
    ~Stack()
    {
        delete[] data;
    }

    /**
     * @brief Pushes an element onto the stack
     * @param element The element to push
     */
    void push(const T &element)
    {
        if (topIndex + 1 == capacity)
        {
            resize();
        }
        data[++topIndex] = element;
    }

    /**
     * @brief Removes and returns the top element
     * @return The top element
     * @throws std::runtime_error if stack is empty
     */
    T pop()
    {
        if (isEmpty())
        {
            throw std::runtime_error("Stack underflow");
        }
        return data[topIndex--];
    }

    /**
     * @brief Returns the top element without removing it
     * @return The top element
     * @throws std::runtime_error if stack is empty
     */
    T top() const
    {
        if (isEmpty())
        {
            throw std::runtime_error("Stack is empty");
        }
        return data[topIndex];
    }

    /**
     * @brief Checks if the stack is empty
     * @return true if empty, false otherwise
     */
    bool isEmpty() const
    {
        return topIndex < 0;
    }
};

/**
 * @brief Class to evaluate mathematical expressions with operator precedence
 */
class ExpressionEvaluator
{
private:
    Stack<double> numbers; // Stack for storing numbers
    Stack<char> operators; // Stack for storing operators

    /**
     * @brief Returns the precedence level of an operator
     * @param op The operator to check
     * @return Precedence level (higher number = higher precedence)
     */
    int getPrecedence(char op)
    {
        switch (op)
        {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
        }
    }

    /**
     * @brief Applies an operator to two operands
     * @param a First operand
     * @param b Second operand
     * @param op Operator to apply
     * @return Result of the operation
     * @throws std::runtime_error for invalid operator or division by zero
     */
    double applyOperator(double a, double b, char op)
    {
        switch (op)
        {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0)
                throw std::runtime_error("Division by zero");
            return a / b;
        case '^':
            return std::pow(a, b);
        default:
            throw std::runtime_error("Invalid operator");
        }
    }

public:
    /**
     * @brief Evaluates a mathematical expression in infix notation
     * @param expression The expression to evaluate (e.g., "3 + 4 * 2")
     * @return The result of the expression
     * @throws std::runtime_error for invalid expressions
     */
    double evaluate(const std::string &expression)
    {
        for (size_t i = 0; i < expression.length(); i++)
        {
            char c = expression[i];

            // Skip whitespace
            if (std::isspace(c))
                continue;

            // Handle numbers (including decimals)
            if (std::isdigit(c))
            {
                double num = 0;
                // Process integer part
                while (i < expression.length() && std::isdigit(expression[i]))
                {
                    num = num * 10 + (expression[i] - '0');
                    i++;
                }
                // Process decimal part if present
                if (i < expression.length() && expression[i] == '.')
                {
                    double fraction = 0.1;
                    i++;
                    while (i < expression.length() && std::isdigit(expression[i]))
                    {
                        num += (expression[i] - '0') * fraction;
                        fraction /= 10;
                        i++;
                    }
                }
                i--; // Backtrack one position
                numbers.push(num);
            }
            // Handle opening parenthesis
            else if (c == '(')
            {
                operators.push(c);
            }
            // Handle closing parenthesis
            else if (c == ')')
            {
                while (!operators.isEmpty() && operators.top() != '(')
                {
                    double b = numbers.pop();
                    double a = numbers.pop();
                    char op = operators.pop();
                    numbers.push(applyOperator(a, b, op));
                }
                if (operators.isEmpty())
                {
                    throw std::runtime_error("Mismatched parentheses");
                }
                operators.pop(); // Remove '('
            }
            // Handle operators
            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
            {
                while (!operators.isEmpty() && operators.top() != '(' &&
                       getPrecedence(operators.top()) >= getPrecedence(c))
                {
                    double b = numbers.pop();
                    double a = numbers.pop();
                    char op = operators.pop();
                    numbers.push(applyOperator(a, b, op));
                }
                operators.push(c);
            }
            else
            {
                throw std::runtime_error("Invalid character in expression");
            }
        }

        // Process remaining operators
        while (!operators.isEmpty())
        {
            if (operators.top() == '(')
            {
                throw std::runtime_error("Mismatched parentheses");
            }
            double b = numbers.pop();
            double a = numbers.pop();
            char op = operators.pop();
            numbers.push(applyOperator(a, b, op));
        }

        if (numbers.isEmpty())
        {
            throw std::runtime_error("Empty expression");
        }

        return numbers.pop();
    }
};

/**
 * @brief Main function to demonstrate the expression evaluator
 */
int main()
{
    try
    {
        ExpressionEvaluator evaluator;

        // Test cases
        std::string expressions[] = {
            "3 + 4 * 2",        // Basic precedence
            "(3 + 4) * 2",      // Parentheses
            "2.5 + 3.7 * 1.2",  // Decimals
            "2 ^ 3 + 1",        // Exponentiation
            "10 - 2 * (3 + 1)", // Complex expression
            "2 ^ (3 + 1)"};

        for (const auto &expr : expressions)
        {
            double result = evaluator.evaluate(expr);
            std::cout << "Expression: " << expr << "\n";
            std::cout << "Result: " << result << "\n\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}