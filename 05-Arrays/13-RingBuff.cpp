#include <iostream>
#include <vector>

template <typename T>
class RingBuffer {
private:
    std::vector<T> buffer;
    size_t head = 0;
    size_t tail = 0;
    size_t maxSize;
    bool full = false;

public:
    RingBuffer(size_t size) : buffer(size), maxSize(size) {}

    bool isEmpty() const {
        return (!full && (head == tail));
    }

    bool isFull() const {
        return full;
    }

    void push(const T& item) {
        buffer[head] = item;
        if (full) {
            tail = (tail + 1) % maxSize; // Overwrite oldest, move tail
        }
        head = (head + 1) % maxSize;
        full = (head == tail);
    }

    bool pop(T& item) {
        if (isEmpty()) return false;

        item = buffer[tail];
        tail = (tail + 1) % maxSize;
        full = false;
        return true;
    }

    void print() const {
        std::cout << "Buffer: ";
        if (isEmpty()) {
            std::cout << "empty";
        } else {
            size_t i = tail;
            do {
                std::cout << buffer[i] << ' ';
                i = (i + 1) % maxSize;
            } while (i != head);
        }
        std::cout << "\n";
    }
};

// Demo
int main() {
    RingBuffer<int> rb(5);

    for (int i = 1; i <= 7; ++i) {
        rb.push(i);
        rb.print();
    }

    int value;
    while (rb.pop(value)) {
        std::cout << "Popped: " << value << '\n';
        rb.print();
    }

    return 0;
}