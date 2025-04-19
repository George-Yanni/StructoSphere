#include <iostream>
#include <cstdint>

using Bitboard = uint16_t;

// Print the board with coordinates
void printBoard(Bitboard x, Bitboard o) {
    std::cout << "  0 1 2\n"; // Column headers
    for (int row = 0; row < 3; ++row) {
        std::cout << row << " "; // Row header
        for (int col = 0; col < 3; ++col) {
            int pos = row * 3 + col;
            if (x & (1 << pos)) std::cout << "X ";
            else if (o & (1 << pos)) std::cout << "O ";
            else std::cout << ". ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool isWin(Bitboard b) {
    const Bitboard wins[] = {
        0b111000000, 0b000111000, 0b000000111, // rows
        0b100100100, 0b010010010, 0b001001001, // columns
        0b100010001, 0b001010100 // diagonals
    };
    
    for (Bitboard win : wins) {
        if ((b & win) == win) return true;
    }
    return false;
}

int main() {
    Bitboard x = 0, o = 0;
    bool x_turn = true;
    
    std::cout << "Tic-Tac-Toe - Enter moves as row.col (e.g. 1.2)\n\n";
    
    while (true) {
        printBoard(x, o);
        
        if (isWin(x)) { std::cout << "X wins!\n"; break; }
        if (isWin(o)) { std::cout << "O wins!\n"; break; }
        if ((x | o) == 0b111111111) { std::cout << "Draw!\n"; break; }
        
        // Get move as row.col
        int row, col;
        char dot;
        std::cout << (x_turn ? "X's turn (row.col): " : "O's turn (row.col): ");
        std::cin >> row >> dot >> col;
        
        // Validate input
        if (row < 0 || row > 2 || col < 0 || col > 2 || dot != '.') {
            std::cout << "Invalid input! Use format like 1.2\n";
            std::cin.clear(); // Clear error flags
            std::cin.ignore(10000, '\n'); // Discard bad input
            continue;
        }
        
        int move = row * 3 + col;
        
        // Check if position is taken
        if ((x | o) & (1 << move)) {
            std::cout << "Position already taken!\n";
            continue;
        }
        
        // Make the move
        if (x_turn) x |= (1 << move);
        else o |= (1 << move);
        
        x_turn = !x_turn;
    }
    
    return 0;
}