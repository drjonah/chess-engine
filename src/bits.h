#ifndef BITS_H
#define BITS_H

#include <iostream>
using namespace std;

typedef uint64_t Bitboard;

void set_bit(Bitboard &board, int square);
bool get_bit(Bitboard board, int square);
bool pop_bit(Bitboard board, int square);

#endif