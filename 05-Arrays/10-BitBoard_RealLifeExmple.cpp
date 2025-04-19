#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>

constexpr int BOARD_SIZE = 64;  // 8x8 board

// Enum for square labels (A1 = 0, H8 = 63)
enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};

// Alias for clarity
using Bitboard = uint64_t;

// Bitboard utility functions
namespace BitboardUtils {

    // Converts a square index to algebraic notation (e.g., E2 -> "e2")
    std::string squareToString(Square sq) {
        std::string files = "abcdefgh";
        int file = sq % 8;
        int rank = sq / 8 + 1;
        return files[file] + std::to_string(rank);
    }

    // Prints the bitboard as an 8x8 grid with rank/file labels
    void print(Bitboard bb, const std::string& title = "") {
        if (!title.empty()) {
            std::cout << "\n" << title << ":\n";
        }
        std::cout << "   a b c d e f g h\n";
        std::cout << "  +---------------+\n";
        for (int rank = 7; rank >= 0; --rank) {
            std::cout << rank + 1 << " |";
            for (int file = 0; file < 8; ++file) {
                int sq = rank * 8 + file;
                std::cout << ((bb >> sq) & 1ULL ? "1 " : ". ");
            }
            std::cout << "| " << rank + 1 << "\n";
        }
        std::cout << "  +---------------+\n";
        std::cout << "   a b c d e f g h\n";
    }

    // Sets the bit at the given square to 1
    Bitboard setSquare(Bitboard bb, Square sq) {
        return bb | (1ULL << sq);
    }

    // Clears the bit at the given square to 0
    Bitboard clearSquare(Bitboard bb, Square sq) {
        return bb & ~(1ULL << sq);
    }

    // Checks if the given square is occupied (bit is 1)
    bool isOccupied(Bitboard bb, Square sq) {
        return (bb >> sq) & 1ULL;
    }

    // Counts the number of 1s in the bitboard
    int popcount(Bitboard bb) {
        #ifdef __GNUC__
            return __builtin_popcountll(bb);
        #else
            std::bitset<64> bits(bb);
            return bits.count();
        #endif
    }

    // Finds the index of the least significant set bit
    int lsb(Bitboard bb) {
        assert(bb != 0);
        #ifdef __GNUC__
            return __builtin_ctzll(bb);
        #else
            for (int i = 0; i < 64; ++i) {
                if (bb & (1ULL << i)) return i;
            }
            return 0;
        #endif
    }

    // Returns a vector of indices for all set bits
    std::vector<int> getSetBits(Bitboard bb) {
        std::vector<int> bits;
        while (bb) {
            int i = lsb(bb);
            bits.push_back(i);
            bb &= bb - 1;  // Clear the least significant set bit
        }
        return bits;
    }
}

class BitboardDemo {
private:
    Bitboard whitePawns;
    Bitboard blackPawns;
    Bitboard occupied;

public:
    BitboardDemo() : whitePawns(0), blackPawns(0), occupied(0) {}

    void setupInitialPosition() {
        std::cout << "\n=== Setting Up Initial Position ===\n";
        std::cout << "White pawns start on rank 2 (bits 8-15)\n";
        std::cout << "Black pawns start on rank 7 (bits 48-55)\n";
        
        whitePawns = 0x000000000000FF00ULL;  // Bits 8-15 (A2-H2)
        blackPawns = 0x00FF000000000000ULL;  // Bits 48-55 (A7-H7)
        occupied = whitePawns | blackPawns;
    }

    void display() const {
        BitboardUtils::print(whitePawns, "White Pawns Bitboard");
        BitboardUtils::print(blackPawns, "Black Pawns Bitboard");
        BitboardUtils::print(occupied, "Occupied Squares Bitboard");
    }

    void makeMove(Square from, Square to, bool isWhite) {
        std::cout << "\n=== Attempting Move ===\n";
        std::cout << (isWhite ? "White" : "Black") << " pawn from " 
                  << BitboardUtils::squareToString(from) << " to "
                  << BitboardUtils::squareToString(to) << "\n";

        if (from < A1 || from > H8 || to < A1 || to > H8) {
            std::cerr << "ERROR: Square out of bounds!\n";
            return;
        }

        Bitboard& pawns = isWhite ? whitePawns : blackPawns;

        if (!BitboardUtils::isOccupied(pawns, from)) {
            std::cerr << "ERROR: No " << (isWhite ? "white" : "black")
                      << " pawn on " << BitboardUtils::squareToString(from) << "!\n";
            return;
        }

        if (BitboardUtils::isOccupied(occupied, to)) {
            std::cerr << "ERROR: " << BitboardUtils::squareToString(to)
                      << " is already occupied!\n";
            return;
        }

        // Show bitboard before move
        std::cout << "\nBefore move:\n";
        BitboardUtils::print(pawns, (isWhite ? "White" : "Black") + std::string(" Pawns"));

        // Make the move
        pawns = BitboardUtils::clearSquare(pawns, from);
        pawns = BitboardUtils::setSquare(pawns, to);
        occupied = whitePawns | blackPawns;

        // Show bitboard after move
        std::cout << "\nAfter move:\n";
        BitboardUtils::print(pawns, (isWhite ? "White" : "Black") + std::string(" Pawns"));
        std::cout << "Move successful!\n";
    }

    void showPawnPushes() const {
        std::cout << "\n=== Calculating White Pawn Pushes ===\n";
        std::cout << "1. Shift white pawns up one rank (<< 8)\n";
        Bitboard shifted = whitePawns << 8;
        BitboardUtils::print(shifted, "Shifted White Pawns");
        
        std::cout << "2. Mask with unoccupied squares (~occupied)\n";
        BitboardUtils::print(~occupied, "Unoccupied Squares (inverted)");
        
        Bitboard pushes = shifted & ~occupied;
        BitboardUtils::print(pushes, "Legal Push Targets (AND of shifted and unoccupied)");
        
        std::vector<int> pushSquares = BitboardUtils::getSetBits(pushes);
        if (pushSquares.empty()) {
            std::cout << "No legal pawn pushes available.\n";
        } else {
            std::cout << "Possible push squares: ";
            for (int sq : pushSquares) {
                std::cout << BitboardUtils::squareToString(static_cast<Square>(sq)) << " ";
            }
            std::cout << "\n";
        }
    }

    void showBitOperations() const {
        std::cout << "\n=== Bitboard Operations ===\n";
        
        std::cout << "Counting white pawns:\n";
        std::cout << "There are " << BitboardUtils::popcount(whitePawns) 
                  << " white pawns on the board\n";
        
        std::cout << "\nListing occupied squares:\n";
        std::vector<int> bits = BitboardUtils::getSetBits(whitePawns);
        std::cout << "White pawns are located at: ";
        for (int sq : bits) {
            std::cout << BitboardUtils::squareToString(static_cast<Square>(sq)) << " ";
        }
        std::cout << "\n";
        
        std::cout << "\nFinding the first white pawn (LSB):\n";
        if (whitePawns) {
            int first = BitboardUtils::lsb(whitePawns);
            std::cout << "First white pawn is at " 
                      << BitboardUtils::squareToString(static_cast<Square>(first)) << "\n";
        } else {
            std::cout << "No white pawns on the board\n";
        }
    }
};

void printIntroduction() {
    std::cout << "============================================\n";
    std::cout << "       Chess Bitboard Demonstration\n";
    std::cout << "============================================\n\n";
    
    std::cout << "This program demonstrates how bitboards can be used to represent\n";
    std::cout << "chess positions and calculate moves efficiently.\n\n";
    
    std::cout << "Key Concepts:\n";
    std::cout << "- Each bit in a 64-bit integer represents a square (A1=0, H8=63)\n";
    std::cout << "- Bitwise operations efficiently calculate attacks and moves\n";
    std::cout << "- Multiple boards can represent different piece types/colors\n\n";
    
    std::cout << "We'll demonstrate with pawns only for simplicity.\n";
    std::cout << "============================================\n";
}

int main() {
    printIntroduction();
    
    BitboardDemo demo;
    demo.setupInitialPosition();
    demo.display();

    std::cout << "\n=== Demonstration 1: Making a Move ===\n";
    std::cout << "We'll move the white pawn from e2 to e4\n";
    demo.makeMove(E2, E4, true);
    demo.display();

    std::cout << "\n=== Demonstration 2: Calculating Pawn Pushes ===\n";
    demo.showPawnPushes();

    std::cout << "\n=== Demonstration 3: Bitboard Operations ===\n";
    demo.showBitOperations();

    std::cout << "\n=== Demonstration Complete ===\n";
    std::cout << "This shows the basic principles of how bitboards work in chess engines.\n";
    std::cout << "Actual implementations would include more piece types and complex move generation.\n";

    return 0;
}