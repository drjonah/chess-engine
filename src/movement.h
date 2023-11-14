#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <iostream>
#include "bits.h"
using namespace std;

// const
const Bitboard not_a = 18374403900871474942ULL;
const Bitboard not_h = 9187201950435737471ULL;
const Bitboard not_ab = 4557430888798830399ULL;
const Bitboard not_hg = 18229723555195321596ULL;

// generation of moves
Bitboard generate_pawn_attacks(int color, int square);
Bitboard generate_knight_attacks(int square);
Bitboard generate_bishop_attacks(int square);
Bitboard generate_rook_attacks(int square);
Bitboard generate_queen_attacks(int square);
Bitboard generate_king_attacks(int square);

#endif