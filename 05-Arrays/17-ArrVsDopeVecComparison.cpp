#include <iostream>
#include <vector>

#define ROWS 5
#define COLS 5

// ========== DOPE VECTOR CLASS ==========
class DopeVector2D {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    DopeVector2D(size_t r, size_t c) : rows(r), cols(c) {
        data.resize(r, std::vector<int>(c, 0));
    }

    void setElement(size_t r, size_t c, int value) {
        if (r < rows && c < cols)
            data[r][c] = value;
        else
            std::cerr << "[DopeVec] Index out of bounds!\n";
    }

    int getElement(size_t r, size_t c) const {
        if (r < rows && c < cols)
            return data[r][c];
        else {
            std::cerr << "[DopeVec] Index out of bounds!\n";
            return -1;
        }
    }

    void print() const {
        std::cout << "[DopeVec] Matrix:\n";
        for (const auto& row : data) {
            for (int val : row)
                std::cout << val << " ";
            std::cout << "\n";
        }
    }
};

// ========== RAW ARRAY FUNCTIONS ==========
void initRawArray(int arr[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            arr[i][j] = 0;
}

void setRawArray(int arr[ROWS][COLS], int r, int c, int value) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        arr[r][c] = value;
    else
        std::cerr << "[RawArr] Index out of bounds!\n";
}

int getRawArray(int arr[ROWS][COLS], int r, int c) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        return arr[r][c];
    else {
        std::cerr << "[RawArr] Index out of bounds!\n";
        return -1;
    }
}

void printRawArray(int arr[ROWS][COLS]) {
    std::cout << "[RawArr] Matrix:\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j)
            std::cout << arr[i][j] << " ";
        std::cout << "\n";
    }
}

// ========== MAIN ==========
int main() {
    // -------- RAW ARRAY USAGE --------
    int raw[ROWS][COLS];
    initRawArray(raw);

    setRawArray(raw, 0, 0, 100);
    setRawArray(raw, 2, 2, 200);
    setRawArray(raw, 4, 4, 300);
    setRawArray(raw, 5, 5, 999);  // Out-of-bounds

    printRawArray(raw);
    std::cout << "[RawArr] Pixel at (2,2): " << getRawArray(raw, 2, 2) << "\n\n";

    // -------- DOPE VECTOR USAGE --------
    DopeVector2D dope(ROWS, COLS);
    dope.setElement(0, 0, 100);
    dope.setElement(2, 2, 200);
    dope.setElement(4, 4, 300);
    dope.setElement(5, 5, 999);  // Out-of-bounds

    dope.print();
    std::cout << "[DopeVec] Pixel at (2,2): " << dope.getElement(2, 2) << "\n";

    return 0;
}
