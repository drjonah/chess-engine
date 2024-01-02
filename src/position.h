#ifndef POSITION_H
#define POSITION_H

#include <array>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "bitboard.h"
using namespace std;

struct Move {
    int piece_type=0;
    int start_square=0;
    int end_square=0;
    int color=0;
    int castle_move=0;
    int score=0;
};

struct State {
    int color_state;
    int en_passant_state;
    int castling_state;

    bb pieces_state[12];
    bb occupancy_state[3];
};

enum {
    p_val=100,
    n_val=300,
    b_val=350,
    r_val=500,
    q_val=900,
    k_val=10000
};

const int opening_score[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  10, 10, 10, 10, 0,  0,
    0,  0,  10, 20, 20, 10, 0,  0,
    0,  0,  10, 20, 20, 10, 0,  0,
    0,  0,  10, 10, 10, 10, 0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int pawn_score[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  10, 10, 0,  0,  0,
    0,  0,  0,  5,  5,  0,  0,  0,
    0,  0,  0, -5, -5,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0
};

class Positions {
    public:
        // initalization
        Positions() = default;
        void init(string FEN_args[3]);
        void generate_all_game_pieces(string board);
        void generate_game_properties(string castling_rights, string en_passant);
        void generate_attack_moves();

        // basic
        bb* get_piece_board(int piece_val);
        State get_state();

        // board
        vector<int> get_piece_moves(int piece_type, int square, int color);
        bool is_attacked(int check_square, int color);
        bool is_check(int color);
        bool is_checkmate(int color);

        // movements
        void make_move(int piece_type, int start_square, int end_square, int color, int castle_move=0);
        Move unmake_move();
        bool piece_capture(int square, int color);
        Move get_previous_move();
        void pawn_promote(int start_type, int end_type, int starting_square, int ending_square);

        bb get_bishop_magic_attack(int square, bb curr_occupancy);
        bb get_rook_magic_attack(int square, bb curr_occupancy);
        bb get_queen_magic_attack(int square, bb curr_occupancy);

        // ai
        Move get_best_move(int color);
        int evaluate();
        Move alpha_beta_max(int alpha, int beta, int depth, int color);
        Move alpha_beta_min(int alpha, int beta, int depth, int color);
        vector<Move> get_all_moves(int color);

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
        int color;
        int en_passant;
        int castling_rights; // 0 (1) 0 (2) 0 (4) 0 (8)

        // masks
        bb pawn_mask[2][64];
        bb knight_mask[64];
        bb king_mask[64];  
        bb bishop_mask[64];
        bb rook_mask[64];
        bb queen_mask[64];
    
        // pieces
        bb pieces[12];
        bb occupancy[3];

        // storage
        stack<Move> move_stack;
        stack<State> game_stack;
};

#endif 