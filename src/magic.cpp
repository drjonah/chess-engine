#include <cstdint>
#include <iostream>
#include "bitboard.h"
#include "magic.h"
using namespace std;

int count_bits(bb board) {
    int bit_count = 0;
    while (board) {
        bit_count += board & 1;
        board >>= 1;
    }
    return bit_count;
}

int get_lsb(bb board) {
    if (board) {
        return count_bits((board & ~board) - 1);
    }
    return -1;
}

int generate_key(bb blockers, bb magic, int shift) {
    return static_cast<int>((blockers * magic) >> (64 - shift));
}


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

void init_bishop_table(bb* bishop_mask, bb (*bishop_table)[512]) {
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

            bishop_table[square][key] = attacks;
        }
    }
}

void init_rook_table(bb* rook_mask, bb (*rook_table)[4096]) {
    for (int square = 0; square < 64; ++square) {
        bb curr_mask = rook_mask[square];
        int bits = (1 << count_bits(curr_mask));

        // iterate through all ones on mask
        for (int i = 0; i < bits; ++i) {
            bb blockers = get_blockers(i, curr_mask);
            bb attacks = 0ULL; // bishop_positon

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

            rook_table[square][key] = attacks;
        }
    }
}

bb get_bishop_magic_attack(int square, bb curr_occupancy, bb* bishop_mask, bb (*bishop_table)[512]) {
    // get bishop attacks assuming current board occupancy
    curr_occupancy &= bishop_mask[square];
    curr_occupancy *= bishop_magic[square];
    curr_occupancy >>= 64 - bishop_bits[square];
    
    // return bishop attacks
    return bishop_table[square][curr_occupancy];
}

bb get_rook_magic_attack(int square, bb curr_occupancy, bb* rook_mask, bb (*rook_table)[4096]) {
    // get rook attacks assuming current board occupancy
    curr_occupancy &= rook_mask[square];
    curr_occupancy *= rook_magic[square];
    curr_occupancy >>= 64 - rook_bits[square];
    
    // return rook attacks
    return rook_table[square][curr_occupancy];
}

bb get_queen_magic_attack(int square, bb curr_occupancy, bb* queen_mask, bb* bishop_mask, bb* rook_mask, bb (*bishop_table)[512], bb (*rook_table)[4096]) {
    // get bishops magic
    bb temp_bishop = get_bishop_magic_attack(square, curr_occupancy, bishop_mask, bishop_table);
    // get rooks magic 
    bb temp_rook = get_rook_magic_attack(square, curr_occupancy, rook_mask, rook_table);

    return temp_bishop | temp_rook;

}