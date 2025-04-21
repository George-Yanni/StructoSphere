#include <iostream>
#include <vector>
#include <string>
// A code implementing a ring buffer (circular buffer) data structure
// that stores the last N words entered by the user
class RingBuffer {
    std::vector<std::string> buffer;
    int head = 0;
    int size;
public:
    explicit RingBuffer(int n) : buffer(n), size(n) {}

    void push(const std::string& word) {
        buffer[head] = word;
        head = (head + 1) % size;
    }

    void print() const {
        std::cout << "Buffer contents: ";
        for (const auto& word : buffer) {
            std::cout << word << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    RingBuffer ring(5); // store last 5 words (adjust as needed)

    std::string word;
    std::cout << "Start typing words. Type 'RingBuff' to end:\n";

    while (std::cin >> word) {
        ring.push(word);
        if (word == "RingBuff") {
            std::cout << "Secret captured\n";
            ring.print();  // Print buffer contents after secret is captured
            break;
        }
    }

    return 0;
}