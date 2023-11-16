#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include "bitboard.h"

namespace Chess {

class Pieces {
    public:
        Pieces();
        void generate_attack_moves_();
        void generate_all_game_pieces_();
        void attacks_to(int position);
    private:
        uint64_t generate_pawn_attacks_(int color, int square);
        uint64_t generate_knight_attacks_(int square);
        uint64_t generate_bishop_attacks_(int square);
        uint64_t generate_rook_attacks_(int square);
        uint64_t generate_queen_attacks_(int square);
        uint64_t generate_king_attacks_(int square);
    private:
        uint64_t attack_pawns[2][64];
        uint64_t attack_knights[64];
        uint64_t attack_bishops[64];
        uint64_t attack_rooks[64];
        uint64_t attack_queens[64];
        uint64_t attack_kings[64];

        uint64_t pawns[2];
        uint64_t knights[2];
        uint64_t bishops[2];
        uint64_t rooks[2];
        uint64_t queens[2];
        uint64_t kings[2];
};

}

#endif 