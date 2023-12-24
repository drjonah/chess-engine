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
        vector<int> get_available_moves(int piece_type, int square, int color);
        bool is_attacked(int check_square, int color);
        bool is_check(int color);
        bool is_checkmate(int color);

        // movements
        void make_move(int piece_type, int start_square, int end_square, int color, int castle_move=0);
        vector<int> unmake_move();
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

        // store / fetch positions
        void save_position();
        void restore_position();

    private:
        // properties 
        int starting_color;
        int en_passant;

        // 0 (1) 0 (2) 0 (4) 0 (8) .... first 4 bits are used to see if available
        // u_int16_t castling_rights;

        // masks
        bb pawn_mask[2][64];
        bb knight_mask[64];
        bb king_mask[64];  
        bb bishop_mask[64];
        bb rook_mask[64];
        bb queen_mask[64];
        
        bb occupancy[3];

        // stack
        stack<array<int, 5>> move_stack;

        // store / fetch members
        bb pieces_copy[12], occupancy_copy[3];
        int en_passant_copy, castling_rights_copy;

    public:
        // pieces
        bb pieces[12];

        u_int16_t castling_rights;
};


#endif 