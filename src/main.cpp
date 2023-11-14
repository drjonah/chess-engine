#include <iostream>
#include "bits.h"
#include "movement.h"
using namespace std;

void generate_all_attacks();
void printBoard(Bitboard board);

enum { white, black };
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

// Global piece bitboards
// Format: Bitboard piece[color=white,black][a1->h8]
Bitboard pawns[2][64];
Bitboard knights[64];
Bitboard bishops[64];
Bitboard rooks[64];
Bitboard queens[64];
Bitboard kings[64];

int main() {
    // welcome
    cout << "==========================" << endl;
    cout << "= CHESS ENGINE | drjonah =" << endl;
    cout << "==========================\n" << endl;

    // generate all attacks
    cout << "Generating attack moves for each piece...\n" << endl;
    generate_all_attacks();

    printBoard(bishops[e4]);
    printBoard(rooks[e4]);
    printBoard(queens[e4]);

    return 0;
}

/**
 * Generates all possible move for pieces based on positon.
 * 
 * @param None
 * @return void
 */
void generate_all_attacks() {
    for (int square = 0; square < 64; ++square) {
        // pawn attacks
        pawns[white][square] = generate_pawn_attacks(white, square);
        pawns[black][square] = generate_pawn_attacks(black, square);
        // knight attacks
        knights[square] = generate_knight_attacks(square);
        // bishop attacks
        bishops[square] = generate_bishop_attacks(square);
        // rook attacks
        rooks[square] = generate_rook_attacks(square);
        // queen attacks
        queens[square] = generate_queen_attacks(square);
        // king attacks
        kings[square] = generate_king_attacks(square);
    }
}

/**
 * Prints Bitboard
 * 
 * @param board : 64 bit unsigned integer board representation
 * @return void
 */
void printBoard(Bitboard board) {
    for (int rank = 0; rank < 8; ++rank) {
        cout << 8 - rank << " | ";
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            cout << get_bit(board, square) << " ";
        }
        cout << endl;
    }
    cout << "  -----------------" << endl;
    cout << "    a b c d e f g h" << endl;
    cout << "Position: " << board << endl;
    cout << endl;
}