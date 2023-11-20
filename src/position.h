#ifndef POSITION_H
#define POSITION_H

#include <iostream>

#include "bitboard.h"

using namespace std;

class Positions {
    public:
        // initalization
        Positions() = default;
        void init();
        void generate_attack_moves();
        void generate_all_game_pieces();

        // movements
        bb get_bishop_magic_attack(int square, bb curr_occupancy);
        bb get_rook_magic_attack(int square, bb curr_occupancy);
        bb get_queen_magic_attack(int square, bb curr_occupancy);

    private:
        // generate masks
        bb generate_pawn_mask(int color, int square);
        bb generate_knight_mask(int square);
        bb generate_king_mask(int square);
        bb generate_bishop_mask(int square);
        bb generate_rook_mask(int square);
        bb generate_queen_mask(int square);

    private:
        // masks
        bb pawn_mask[2][64];
        bb knight_mask[64];
        bb king_mask[64];  
        bb bishop_mask[64];
        bb rook_mask[64];
        bb queen_mask[64];

        // pieces
        bb pawns[2];
        bb knights[2];
        bb bishops[2];
        bb rooks[2];
        bb queens[2];
        bb kings[2];
};


#endif 