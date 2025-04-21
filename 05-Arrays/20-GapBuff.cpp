#include <iostream>
#include <vector>
#include <string>

class GapBuffer {
private:
    std::vector<char> buffer;
    int gap_start;
    int gap_end;

public:
    GapBuffer(int capacity = 100) {
        buffer.resize(capacity, '\0');
        gap_start = 0;
        gap_end = capacity;
    }

    int size() const {
        return buffer.size() - (gap_end - gap_start);
    }

    void printBuffer() const {
        for (int i = 0; i < buffer.size(); ++i) {
            if (i == gap_start) std::cout << '|'; // mark cursor
            if (i < gap_start || i >= gap_end) std::cout << buffer[i];
        }
        std::cout << " (gap size: " << gap_end - gap_start << ")\n";
    }

    void insert(char c) {
        if (gap_start == gap_end) expandBuffer();
        buffer[gap_start++] = c;
    }

    void deleteChar() {
        if (gap_start > 0) {
            --gap_start;
        }
    }

    void moveCursorLeft() {
        if (gap_start > 0) {
            buffer[--gap_end] = buffer[--gap_start];
        }
    }

    void moveCursorRight() {
        if (gap_end < buffer.size()) {
            buffer[gap_start++] = buffer[gap_end++];
        }
    }

    void expandBuffer(int new_capacity = 0) {
        if (new_capacity == 0) new_capacity = buffer.size() * 2;
        std::vector<char> new_buffer(new_capacity, '\0');
        int new_gap_end = new_capacity - (buffer.size() - gap_end);

        // Copy text before the gap
        for (int i = 0; i < gap_start; ++i)
            new_buffer[i] = buffer[i];

        // Copy text after the gap
        int tail_len = buffer.size() - gap_end;
        for (int i = 0; i < tail_len; ++i)
            new_buffer[new_gap_end + i] = buffer[gap_end + i];

        gap_end = new_gap_end;
        buffer = std::move(new_buffer);
    }

    std::string getContent() const {
        std::string content;
        for (int i = 0; i < buffer.size(); ++i) {
            if (i < gap_start || i >= gap_end)
                content += buffer[i];
        }
        return content;
    }
};

// 📌 Demonstration
int main() {
    GapBuffer gb;

    std::cout << "Initial state:\n";
    gb.printBuffer();

    std::string input = "hello";
    for (char c : input) gb.insert(c);

    std::cout << "After inserting 'hello':\n";
    gb.printBuffer();

    gb.moveCursorLeft();
    gb.moveCursorLeft();
    std::cout << "After moving cursor left twice:\n";
    gb.printBuffer();

    gb.insert('X');
    std::cout << "After inserting 'X':\n";
    gb.printBuffer();

    gb.deleteChar();
    std::cout << "After deleting character:\n";
    gb.printBuffer();

    std::cout << "Final content: " << gb.getContent() << "\n";
}
