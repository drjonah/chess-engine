/* ============================== *\

    Chess Engine by DrJonah
         bitboard.cpp

    This file manages initialization
    and manipulation of bitboards.

\* ============================== */

#include <iostream>
#include <string>
#include <vector>
#include "bitboard.h"
using namespace std;

const bb not_a = 18374403900871474942ULL;
const bb not_h = 9187201950435737471ULL;
const bb not_ab = 18229723555195321596ULL;
const bb not_hg = 4557430888798830399ULL;

/**
 * This finds where the 1s are on the board aka squares
 * 
 * @param board 64 bit unsigned integer board representation 
 * @return vector of square locations
 */
vector<int> get_piece_squares(bb board) {
    vector<int> squares;
    int curr_square = 0;
    // loop bits
    while(board != 0) {
        if (board & 1) 
            squares.push_back(curr_square);
        board >>= 1;
        ++curr_square;
    }
    // return positions of squares
    return squares;
}

/**
 * This counts the number of 1s on the board
 * 
 * @param board 64 bit unsigned integer board representation 
 * @return number of 1s
 */
int count_bits(bb board) {
    int bit_count = 0;
    while (board) {
        bit_count += board & 1;
        board >>= 1;
    }
    return bit_count;
}

/**
 * This gets the least significant bit
 * 
 * @param board 64 bit unsigned integer board representation 
 * @return least significant bit position
 */
int get_lsb(bb board) {
    if (board) 
        return count_bits((board & ~board) - 1);
    return -1;
}

/**
 * This removes a 1 bit on the board
 * 
 * @param board 64 bit unsigned integer board representation 
 * @param square square to remove
 * @return None
 */
void remove_bit(bb* board, int square) {
    *board &= ~(1ULL << (square));
}

/**
 * This adds a 1 bit on the board
 * 
 * @param board 64 bit unsigned integer board representation 
 * @param square square to remove
 * @return None
 */
void add_bit(bb* board, int square) {
    *board |= (1ULL << (square));
}

/**
 * This moves 1s bit
 * 
 * @param board 64 bit unsigned integer board representation 
 * @param starting_square square to remove
 * @param ending_square square to add
 * @return None
 */
void move_bit(bb* board, int starting_square, int ending_square) {
    // remove bit from the board
    remove_bit(board, starting_square);
    // add bit to the board
    add_bit(board, ending_square);
}

/**
 * This returns if a bit is on the board
 * 
 * @param board 64 bit unsigned integer board representation 
 * @param square square to remove
 * @return true if 1 else 0
 */
bool get_bit(bb bitboard, int square) {
    return (bitboard & (1ULL << square)) != 0;
}

/**
 * Prints a single bitboard with 1s and 0s
 * 
 * @param board 64 bit unsigned integer board representation 
 * @return None
 */
void print_bb(bb board) {
    for (int rank = 0; rank < 8; ++rank) {
        cout << 8 - rank << " | ";
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            cout << ((board & (1ULL << square)) != 0) << " ";
        }
        cout << endl;
    }
    cout << "  -----------------" << endl;
    cout << "    a b c d e f g h" << endl;
    cout << "Position: " << board << endl;
    cout << endl;
}

/**
 * Prints all bitboards with labels
 * 
 * @param pieces 12 bitboard array with each piece, pointer
 * @return None
 */
void print_readable(bb* pieces, bb full_occupancy, string en_passant_state, int castling_state) {
    // vars 
    const char* piece_symbols[6] = {"P", "N", "B", "R", "Q", "K"};
    const string colors[2] = {YELLOW, GREEN};
    int readable_board[64] = {0};

    // load board
    for (int piece = 0; piece < 12; ++piece) {
        bb board = pieces[piece];
        for (int square : get_piece_squares(board)) readable_board[square] = piece+1;
    }
    
    // main print loop
    cout << endl;
    for (int square = 0; square < 64; ++square) {
        int piece = readable_board[square]-1;
        if (square % 8 == 0) {
            if (square != 0) cout << CYAN << "\n  ---------------------------------" << endl;
            cout << WHITE << 8 - (square / 8) << " ";
        }
        if (piece < 0) cout << CYAN << " | " << WHITE << "*";
        else cout << CYAN << " | " << colors[(readable_board[square] % 2 == 0) ? 0 : 1] << piece_symbols[piece / 2];
    }
    cout << endl << WHITE << "\n     A   B   C   D   E   F   G   H" << RESET << endl << endl;

    // game details
    cout << "     Bitboard: " << full_occupancy << endl;
    cout << "     En-passante: " << en_passant_state << endl;
    cout << "     Castling: ";
    cout << ((castling_state & wk) ? "W" : "-");
    cout << ((castling_state & wq) ? "Q" : "-");
    cout << ((castling_state & bk) ? "w" : "-");
    cout << ((castling_state & bq) ? "q" : "-");
    cout << endl;
}
