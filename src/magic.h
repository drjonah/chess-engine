#ifndef MAGIC_H
#define MAGIC_H

#include <iostream>
#include "bitboard.h"
using namespace std;

// tables
extern bb bishop_magic_table[64][512];
extern bb rook_magic_table[64][4096];

// source for consts: https://github.com/maksimKorzh/chess_programming/blob/master/src/bbc/init_slider_attacks/bbc.c
extern const bb rook_magic[64];
extern const bb bishop_magic[64];

extern const int bishop_bits[64];
extern const int rook_bits[64];

int generate_key(bb blockers, bb magic, int shift);
bb get_blockers(int permutation, bb mask);

void init_bishop_table(bb* bishop_mask);
void init_rook_table(bb* rook_mask);

#endif