/* ============================== *\

    Chess Engine by DrJonah
            magic.cpp

    This file initializes and
    contains magic tables. It
    uses hashing techniques
    to store each possible board
    at a certain position (key).
    The key can made easily 
    and duplicated easily.

\* ============================== */

#include <iostream>
#include "bitboard.h"
#include "magic.h"
using namespace std;

bb bishop_magic_table[64][512];
bb rook_magic_table[64][4096];

const bb rook_magic[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};

const bb bishop_magic[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL
};

const int bishop_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

/**
 * This generates a magic key to be used in finding magic positions
 * 
 * @param blockers 64 bit unsigned integer board representation including blocked positions
 * @param magic magic number for transformation
 * @param shift number of bits to shift the result to the right. It determines the index range used in the final key calculation
 * @return unique key for magic bitboard
 */
int generate_key(bb blockers, bb magic, int shift) {
    return (blockers * magic) >> (64 - shift);
}

/**
 * Generates a bitboard configuration of blocker pieces based on a permutation and a mask
 *
 * @param permutation index representing a specific permutation of blocker pieces
 * @param mask bitboard mask indicating the squares where blocker pieces can be placed
 * @return bitboard configuration of blocker pieces based on the given permutation and mask
 */
bb get_blockers(int permutation, bb mask) {
    bb blockers = 0ULL;
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

            // adds valid blockers to attack board
            for (int rank = target_rank+1, file = target_file+1; rank < 8 && file < 8; ++rank, ++file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }
            
            for (int rank = target_rank-1, file = target_file+1; rank >= 0 && file < 8; --rank, ++file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }
            
            for (int rank = target_rank+1, file = target_file-1; rank < 8 && file >= 0; ++rank, --file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }
            
            for (int rank = target_rank-1, file = target_file-1; rank >= 0 && file >= 0; --rank, --file) {
                attacks |= (1ULL << (rank * 8 + file));
                if ((1ULL << (rank * 8 + file)) & blockers) break;
            }

            // magic key to store in table
            int key = generate_key(blockers, bishop_magic[square], bishop_bits[square]);

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

            // adds valid blockers to attack board
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
            int key = generate_key(blockers, rook_magic[square], rook_bits[square]);

            rook_magic_table[square][key] = attacks;
        }
    }
}