#include <cstdint>
#include <iostream>
#include "bitboard.h"
#include "magic.h"
using namespace std;

bb bishop_magic_table[64][512];
bb rook_magic_table[64][4096];

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
 * This generates a magic key to be used in finding magic positions
 * 
 * @param blockers 64 bit unsigned integer board representation including blocked positions
 * @param magic magic number for transformation
 * @param shift number of bits to shift the result to the right. It determines the index range used in the final key calculation
 * @return unique key for magic bitboard
 */
int generate_key(bb blockers, bb magic, int shift) {
    return static_cast<int>((blockers * magic) >> (64 - shift));
}

/**
 * Generates a bitboard configuration of blocker pieces based on a permutation and a mask
 *
 * @param permutation index representing a specific permutation of blocker pieces
 * @param mask bitboard mask indicating the squares where blocker pieces can be placed
 * @return bitboard configuration of blocker pieces based on the given permutation and mask
 */
bb get_blockers(int permutation, bb mask) {
    bb blockers = 0;
    int count = 0;

    for (int square = 0; square < 64; ++square) {
        // Check if the square is set in the mask
        if ((mask & (1ULL << square)) != 0) {
            // Check if the current bit in the permutation is set
            if ((permutation & (1 << count)) != 0) {
                blockers |= (1ULL << square);
            }
            ++count;
        }
    }
    return blockers;
}

/**
 * Initializes the magic bitboard table for bishop attacks
 *
 * @param bishop_mask array of bitboards representing possible bishop movements on each square
 *        each bitboard in the array corresponds to the diagonals a bishop can move along
 */
void init_bishop_table(bb* bishop_mask) {
    for (int square = 0; square < 64; ++square) {
        bb curr_mask = bishop_mask[square];
        int bits = (1 << count_bits(curr_mask));

        // iterate through all ones on mask
        for (int i = 0; i < bits; ++i) {
            bb blockers = get_blockers(i, curr_mask);
            bb attacks = 0ULL; // bishop_positon

            int target_rank = square / 8;
            int target_file = square % 8;

            for (int rank = target_rank+1, file = target_file+1; rank <= 7 && file <= 7; ++rank, ++file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }
            
            for (int rank = target_rank-1, file = target_file+1; rank >= 0 && file <= 7; --rank, ++file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }
            
            for (int rank = target_rank+1, file = target_file-1; rank <= 7 && file >= 0; ++rank, --file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }
            
            for (int rank = target_rank-1, file = target_file-1; rank >= 0 && file >= 0; --rank, --file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }

            // magic key to store in table
            int key = generate_key(blockers, bishop_magic[square], count_bits(curr_mask));

            bishop_magic_table[square][key] = attacks;
        }
    }
}

/**
 * Initializes the magic bitboard table for rook attacks
 *
 * @param rook_mask array of bitboards representing possible rook movements on each square
 *        each bitboard in the array corresponds to the diagonals a rook can move along
 */
void init_rook_table(bb* rook_mask) {
    for (int square = 0; square < 64; ++square) {
        bb curr_mask = rook_mask[square];
        int bits = (1 << count_bits(curr_mask));

        // iterate through all ones on mask
        for (int i = 0; i < bits; ++i) {
            bb blockers = get_blockers(i, curr_mask);
            bb attacks = 0ULL; // rook_positon

            int target_rank = square / 8;
            int target_file = square % 8;

            for (int rank = target_rank+1; rank < 8; ++rank) {
                attacks |= (1ULL << (rank * 8 + target_file));
                if ((1ULL << (rank * 8 + target_file)) & blockers) break;
            }
            
            for (int rank = target_rank-1; rank >= 0; --rank) {
                attacks |= (1ULL << (rank * 8 + target_file));
                if ((1ULL << (rank * 8 + target_file)) & blockers) break;
            }
            
            for (int file = target_file+1; file < 8; ++file) {
                attacks |= (1ULL << (target_rank * 8 + file));
                if ((1ULL << (target_rank * 8 + file)) & blockers) break;
            }
            
            for (int file = target_file-1; file >= 0; --file) {
                attacks |= (1ULL << (target_rank * 8 + file));
                if ((1ULL << (target_rank * 8 + file)) & blockers) break;
            }

            // magic key to store in table
            int key = generate_key(blockers, rook_magic[square], count_bits(curr_mask));

            rook_magic_table[square][key] = attacks;
        }
    }
}

/**
 * Gets the bishops available moves based on current positioning and occupancy
 *
 * @param square current square the bishop is occupying
 * @param curr_occupancy bitboard of all occupied squares
 * @param bishop_mask array of bitboards representing possible bishop movements on each square
 * @return bitboard of available spaces the bishop can reach
 */
bb get_bishop_magic_attack(int square, bb curr_occupancy, bb* bishop_mask) {
    // get bishop attacks assuming current board occupancy
    curr_occupancy &= bishop_mask[square];
    curr_occupancy *= bishop_magic[square];
    curr_occupancy >>= 64 - bishop_bits[square];
    
    // return bishop attacks
    return bishop_magic_table[square][curr_occupancy];
}

/**
 * Gets the rook available moves based on current positioning and occupancy
 *
 * @param square current square the rook is occupying
 * @param curr_occupancy bitboard of all occupied squares
 * @param rook_mask array of bitboards representing possible rook movements on each square
 * @return bitboard of available spaces the rook can reach
 */
bb get_rook_magic_attack(int square, bb curr_occupancy, bb* rook_mask) {
    // get rook attacks assuming current board occupancy
    curr_occupancy &= rook_mask[square];
    curr_occupancy *= rook_magic[square];
    curr_occupancy >>= 64 - rook_bits[square];
    
    // return rook attacks
    return rook_magic_table[square][curr_occupancy];
}

/**
 * Gets the queen available moves based on current positioning and occupancy
 *
 * @param square current square the bishop is occupying
 * @param curr_occupancy bitboard of all occupied squares
 * @param queen_mask array of bitboards representing possible queen movements on each square
 * @param bishop_mask array of bitboards representing possible bishop movements on each square
 * @param rook_mask array of bitboards representing possible rook movements on each square
 * @return bitboard of available spaces the queen can reach
 */
bb get_queen_magic_attack(int square, bb curr_occupancy, bb* queen_mask, bb* bishop_mask, bb* rook_mask) {
    // get bishops magic
    bb temp_bishop = get_bishop_magic_attack(square, curr_occupancy, bishop_mask);
    // get rooks magic 
    bb temp_rook = get_rook_magic_attack(square, curr_occupancy, rook_mask);

    return temp_bishop | temp_rook;

}