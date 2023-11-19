#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include "bitboard.h"

class Positions {
    public:
        Positions() = default;
        void init();
        void generate_attack_moves();
        void generate_all_game_pieces();
        void attacks_to(int position, int color, bb queen_board, bb pawn_board);
    private:
        bb generate_pawn_mask(int color, int square);
        bb generate_knight_mask(int square);
        bb generate_king_mask(int square);

        // requires magic mwahah
        bb generate_bishop_mask(int square);
        bb generate_rook_mask(int square);
        bb generate_queen_mask(int square);
    public:
        // masks
        bb pawn_mask[2][64];
        bb knight_mask[64];
        bb king_mask[64];  

        // requires magic mwahah
        bb bishop_mask[64];
        bb rook_mask[64];
        bb queen_mask[64];

        bb bishop_table[64][512];
        bb rook_table[64][4096];

        // pieces
        bb pawns[2];
        bb knights[2];
        bb bishops[2];
        bb rooks[2];
        bb queens[2];
        bb kings[2];
};


#endif 