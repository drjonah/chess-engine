#ifndef POSITION_H
#define POSITION_H

#include <array>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "bitboard.h"
using namespace std;

class Positions {
    public:
        // initalization
        Positions() = default;
        void init(string FEN_args[4]);
        void generate_all_game_pieces(string board);
        void generate_game_properties(int starting_color, string castling_rights, string en_passant);
        void generate_attack_moves();

        // basic
        int get_starting_color();
        bb* get_piece_board(int piece_val);

        // board
        vector<int> get_available_moves(int square, int color);
        bool is_attacked(int square, int color);
        bool is_check(int color);
        bool is_checkmate(int color);

        // movements
        void make_move(int piece_type, int start_square, int end_square, int color);
        void unmake_move();
        bool piece_capture(int square, int color);
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
        // properties 
        int starting_color;
        bool en_passant;
        bb castling_rights;

        // masks
        bb pawn_mask[2][64];
        bb knight_mask[64];
        bb king_mask[64];  
        bb bishop_mask[64];
        bb rook_mask[64];
        bb queen_mask[64];
        
        bb occupancy[3];

        // stack
        stack<array<int, 4>> move_stack;

    public:
        // pieces
        bb pieces[12];
};


#endif 