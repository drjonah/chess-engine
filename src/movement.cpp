#include <iostream>
#include "bits.h"
#include "movement.h"
using namespace std;

/**
 * Generates all possible move for pawns based on positon.
 * 
 * @param color : 0 = white, 1 = black
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
Bitboard generate_pawn_attacks(int color, int square) {
    Bitboard attacks = 0ULL;
    Bitboard board = 0ULL;

    set_bit(board, square); // adds square to theoretical board

    // white pawn moves
    if (!color) {
        if ((board >> 7) & not_a) attacks |= (board >> 7);
        if ((board >> 9) & not_h) attacks |= (board >> 9);
    }
    // black pawn moves
    else {
        if ((board << 7) & not_h) attacks |= (board << 7);
        if ((board << 9) & not_a) attacks |= (board << 9);
    }

    return attacks;
}

/**
 * Generates all possible move for knights based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
Bitboard generate_knight_attacks(int square) {
    Bitboard attacks = 0ULL;
    Bitboard board = 0ULL;

    set_bit(board, square); // adds square to theoretical board

    // knight moves
    if ((board >> 17) & not_h) attacks |= (board >> 17);
    if ((board >> 15) & not_a) attacks |= (board >> 15);
    if ((board >> 10) & not_hg) attacks |= (board >> 10);
    if ((board >> 6) & not_ab) attacks |= (board >> 6);
    if ((board << 17) & not_a) attacks |= (board << 17);
    if ((board << 15) & not_h) attacks |= (board << 15);
    if ((board << 10) & not_ab) attacks |= (board << 10);
    if ((board << 6) & not_hg) attacks |= (board << 6);

    return attacks;
}

/**
 * Generates all possible move for bishops based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
Bitboard generate_bishop_attacks(int square) {
    Bitboard attacks = 0ULL;

    // init target rank and files
    int target_rank = square / 8; // 4
    int target_file = square % 8; // 4

    for (int rank = target_rank+1, file = target_file+1; rank < 8 && file < 8; ++rank, ++file) attacks |= (1ULL << (rank * 8 + file)); // NW
    for (int rank = target_rank-1, file = target_file+1; rank >= 0 && file < 8; --rank, ++file) attacks |= (1ULL << (rank * 8 + file)); // NE
    for (int rank = target_rank+1, file = target_file-1; rank < 8 && file >= 0; ++rank, --file) attacks |= (1ULL << (rank * 8 + file)); // SW
    for (int rank = target_rank-1, file = target_file-1; rank >= 0 && file >= 0; --rank, --file) attacks |= (1ULL << (rank * 8 + file)); // SE

    return attacks;
}

/**
 * Generates all possible move for rooks based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
Bitboard generate_rook_attacks(int square) {
    Bitboard attacks = 0ULL;
    
    
    // init target rank and files
    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (int rank = target_rank+1; rank < 8; ++rank) attacks |= (1ULL << (rank * 8 + target_file)); // W
    for (int rank = target_rank-1; rank >= 0; --rank) attacks |= (1ULL << (rank * 8 + target_file)); // E
    for (int file = target_rank+1; file < 8; ++file) attacks |= (1ULL << (target_rank * 8 + file)); // N
    for (int file = target_rank-1; file >= 0; --file) attacks |= (1ULL << (target_rank * 8 + file)); // S

    return attacks;
}

/**
 * Generates all possible move for queens based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
Bitboard generate_queen_attacks(int square) {
    Bitboard attacks = 0ULL;

    Bitboard bishop_attacks = 0ULL;
    Bitboard rook_attacks = 0ULL;
    
    bishop_attacks = generate_bishop_attacks(square);
    attacks |= bishop_attacks;

    rook_attacks = generate_rook_attacks(square);
    attacks |= rook_attacks;

    return attacks;
}

/**
 * Generates all possible move for kings based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
Bitboard generate_king_attacks(int square) {
    Bitboard attacks = 0ULL;
    Bitboard board = 0ULL;

    set_bit(board, square); // adds square to theoretical board

    // king moves
    if (board >> 8) attacks |= (board >> 8);
    if ((board >> 9) & not_h) attacks |= (board >> 9);
    if ((board >> 7) & not_a) attacks |= (board >> 7);
    if ((board >> 1) & not_h) attacks |= (board >> 1);
    if (board << 8) attacks |= (board << 8);
    if ((board << 9) & not_a) attacks |= (board << 9);
    if ((board << 7) & not_h) attacks |= (board << 7);
    if ((board << 1) & not_a) attacks |= (board << 1);

    return attacks;
}