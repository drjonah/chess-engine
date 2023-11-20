#include <iostream>
#include <string>
#include <vector>

#include "bitboard.h"

using namespace std;

const bb not_a = 18374403900871474942ULL;
const bb not_h = 9187201950435737471ULL;
const bb not_ab = 4557430888798830399ULL;
const bb not_hg = 18229723555195321596ULL;

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
    if (board) {
        return count_bits((board & ~board) - 1);
    }
    return -1;
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
 * @param pawns 64 bit unsigned integer board representation 
 * @param knights 64 bit unsigned integer board representation 
 * @param bishops 64 bit unsigned integer board representation 
 * @param rooks 64 bit unsigned integer board representation 
 * @param queens 64 bit unsigned integer board representation 
 * @param kings 64 bit unsigned integer board representation 
 * @return None
 */
void print_readable(bb* pawns, bb* knights, bb* bishops, bb* rooks, bb* queens, bb* kings) {
    // vars 
    const char* piece_symbols[6] = {"P", "N", "B", "R", "Q", "K"};
    const string colors[2] = {YELLOW, GREEN};
    int readable_board[64] = {0};

    // load board
    for (int square : get_piece_squares(pawns[white])) readable_board[square] = pawn_val;
    for (int square : get_piece_squares(pawns[black])) readable_board[square] = -pawn_val;
    for (int square : get_piece_squares(knights[white])) readable_board[square] = knight_val;
    for (int square : get_piece_squares(knights[black])) readable_board[square] = -knight_val;
    for (int square : get_piece_squares(bishops[white])) readable_board[square] = bishop_val;
    for (int square : get_piece_squares(bishops[black])) readable_board[square] = -bishop_val;
    for (int square : get_piece_squares(rooks[white])) readable_board[square] = rook_val;
    for (int square : get_piece_squares(rooks[black])) readable_board[square] = -rook_val;
    for (int square : get_piece_squares(queens[white])) readable_board[square] = queen_val;
    for (int square : get_piece_squares(queens[black])) readable_board[square] = -queen_val;
    for (int square : get_piece_squares(kings[white])) readable_board[square] = king_val;
    for (int square : get_piece_squares(kings[black])) readable_board[square] = -king_val;
    
    // main print loop
    cout << endl;
    for (int square = 0; square < 64; ++square) {
        int piece = abs(readable_board[square]);
        if (square % 8 == 0) {
            if (square != 0) cout << CYAN << "\n  ---------------------------------" << endl;
            cout << WHITE << 8 - (square / 8) << " ";
        }
        if (piece == 0) cout << CYAN << " | " << WHITE << "*";
        else cout << CYAN << " | " << colors[readable_board[square] > 0 ? 0 : 1] << piece_symbols[piece - 1];
    }
    cout << endl << WHITE << "\n     A   B   C   D   E   F   G   H" << RESET << endl << endl;
}
