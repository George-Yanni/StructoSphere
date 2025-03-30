#include <iostream>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

// A real-life application: Checking balanced parentheses in an expression
class ExpressionValidator {
public:
    static bool isBalanced(const string& expression) {
        stack<char> s;
        for (char ch : expression) {
            if (ch == '(' || ch == '{' || ch == '[') {
                s.push(ch);
            } else if (ch == ')' || ch == '}' || ch == ']') {
                if (s.empty()) return false;
                char top = s.top();
                if ((ch == ')' && top != '(') ||
                    (ch == '}' && top != '{') ||
                    (ch == ']' && top != '[')) {
                    return false;
                }
                s.pop();
            }
        }
        return s.empty();
    }
};

// Another real-life application: Undo/Redo functionality in a text editor
class TextEditor {
private:
    stack<string> undoStack;
    stack<string> redoStack;
    string currentText;

public:
    void type(const string& text) {
        undoStack.push(currentText);
        currentText += text;
        while (!redoStack.empty()) redoStack.pop(); // Clear redo history
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(currentText);
            currentText = undoStack.top();
            undoStack.pop();
        } else {
            cout << "Nothing to undo!" << endl;
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(currentText);
            currentText = redoStack.top();
            redoStack.pop();
        } else {
            cout << "Nothing to redo!" << endl;
        }
    }

    void display() const {
        cout << "Current text: " << currentText << endl;
    }
};

// Demonstration of stack operations with real-life applications
int main() {
    cout << "Checking Balanced Parentheses:" << endl;
    string expr1 = "{[()]}";
    string expr2 = "{[(])}";
    cout << "Expression: " << expr1 << " -> " << (ExpressionValidator::isBalanced(expr1) ? "Balanced" : "Not Balanced") << endl;
    cout << "Expression: " << expr2 << " -> " << (ExpressionValidator::isBalanced(expr2) ? "Balanced" : "Not Balanced") << endl;

    cout << "\nText Editor Undo/Redo Simulation:" << endl;
    TextEditor editor;
    editor.type("Hello, ");
    editor.display();
    editor.type("World!");
    editor.display();
    
    editor.undo();
    editor.display();
    editor.redo();
    editor.display();
    
    return 0;
}
