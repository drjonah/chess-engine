#include <iostream>
#include "bits.h"
using namespace std;

typedef uint64_t Bitboard;

/**
 * Sets a bit on board in specified positon
 * 
 * @param board : references to 64 bit unsigned integer board representation 
 * @param square : position on board
 * @return void
 */
void set_bit(Bitboard &board, int square) {
    board |= (1ULL << square); 
}

/**
 * Gets bit on board in specific position
 * 
 * @param board : references to 64 bit unsigned integer board representation 
 * @param square : position on board
 * @return void
 */
bool get_bit(Bitboard board, int square) {
    return (board & (1ULL << square)) != 0;
}

/**
 * Removes bit on board in specific position
 * 
 * @param board : references to 64 bit unsigned integer board representation 
 * @param square : position on board
 * @return void
 */
void pop_bit(Bitboard &board, int square) {
    if (get_bit(board, square)) {
        board ^= (1ULL << square);
    }
}