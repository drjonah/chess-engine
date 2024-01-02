/* ============================== *\

    Chess Engine by DrJonah
         position.cpp

    This file initializes all 
    game masks and pieces. It 
    handles initialization as
    well as manages attacks,
    checks, and checkmates.

\* ============================== */

// #include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "bitboard.h"
#include "magic.h"
#include "position.h"
using namespace std;

// initialize game
void Positions::init(string FEN_args[3]) {
    // setting up game using FEN
    cout << "Generating all games pieces..." << endl;
    this->generate_all_game_pieces(FEN_args[0]);

    cout << "Generating game properties..." << endl;
    this->generate_game_properties(FEN_args[1], FEN_args[2]);

    cout << "Storing surrent state of game..." << endl;
    this->save_position();

    // attack masks including magic 
    cout << "Generating attack masks for each piece..." << endl;
    this->generate_attack_moves();

    cout << "Initializing magic (bishop)..." << endl;
    init_bishop_table(this->bishop_mask);

    cout << "Initializing magic (rook)..." << endl;
    init_rook_table(this->rook_mask);
}

/**
 * Generates all game pieces. The method is passed a FEN based 
 * board and it is translated to generating the pieces based 
 * piece and position.
 * 
 * @param board string FEN based representation of board
 * @return void
 */
void Positions::generate_all_game_pieces(string board) {
    // conversion map
    map<char, int> piece_conversion = {
        {'P', P}, {'p', p},
        {'N', N}, {'n', n},
        {'B', B}, {'b', b},
        {'R', R}, {'r', r},
        {'Q', Q}, {'q', q},
        {'K', K}, {'k', k}
    };

    // default the boards
    for (int i = 0; i < 12; ++i) this->pieces[i] = 0ULL;
    for (int i = 0; i < 3; ++i) this->occupancy[i] = 0ULL;

    // build board
    int square = 0;

    for (char piece : board) {
        // piece
        if (isalpha(piece)) {
            int target_board = piece_conversion[piece];
            int color = (target_board % 2 == 0) ? white : black;

            add_bit(&(this->pieces[target_board]), square);
            add_bit(&(this->occupancy[color]), square);
            add_bit(&(this->occupancy[both]), square);

            ++square;
        }
        // skip digit
        else if (isdigit(piece)) {
            int skip_distance = piece - '0';
            square += skip_distance;
        }
    }
}

/**
 * Generates game properties including starting color,
 * castling rights, and en_passant.
 * 
 * @param starting_color starting color for the game 
 * @param castling_rights starting castling rights
 * @param en_passant rules for en passant
 * @return void
 */
void Positions::generate_game_properties(string castling_rights, string en_passant) {

    // castling rights
    this->castling_rights = 0;
    for (char piece : castling_rights) {
        // white side castling
        if (piece == 'K') this->castling_rights |= wk;
        if (piece == 'Q') this->castling_rights |= wq;

        // black side castling
        if (piece == 'k') this->castling_rights |= bk;
        if (piece == 'q') this->castling_rights |= bq;
    }

    // en_passants
    if (en_passant[0] != '-') {
        int file = en_passant[0] - 'a';
        int column = 8 - (en_passant[1] - '0');
        this->en_passant = (8 * column + file);
    }
    else this->en_passant = 0;
}

/**
 * Generates all possible move for pieces based on positon.
 * 
 * @param None
 * @return void
 */
void Positions::generate_attack_moves() {
    for (int square = 0; square < 64; ++square) {
        // pawn attacks
        this->pawn_mask[white][square] = generate_pawn_mask(white, square);
        this->pawn_mask[black][square] = generate_pawn_mask(black, square);
        // knight attacks
        this->knight_mask[square] = generate_knight_mask(square);
        // bishop attacks
        this->bishop_mask[square] = generate_bishop_mask(square);
        // rook attacks
        this->rook_mask[square] = generate_rook_mask(square);
        // queen attacks
        this->queen_mask[square] = generate_queen_mask(square);
        // king attacks
        this->king_mask[square] = generate_king_mask(square);
    }
}

/**
 * Returns a piece board based on a given piece value (0->11)
 * 
 * @param piece_val alue of a piece ex. 0, 1 = white pawn, black pawn
 * @return board pointer to 64 bit unsigned integer board representation 
 */
bb* Positions::get_piece_board(int piece_val) {
    if (piece_val < 12)
        return &this->pieces[piece_val];
    return 0ULL;
}

/**
 * Returns the state of the game
 * 
 * @return returns a struct containing the state of the game
 */
State Positions::get_state() {
    State game_state;

    // game_state.color_state = this->color;
    game_state.en_passant_state = this->en_passant;
    game_state.castling_state = this->castling_rights;
    
    for (int i = 0; i < 12; ++i) {
        if (i < 3) game_state.occupancy_state[i] = this->occupancy[i];
        game_state.pieces_state[i] = this->pieces[i];
    }

    return game_state;
}

/**
 * Returns the available positions that can be moved to by a piece and color
 * 
 * @param piece_type piece type to check
 * @param square square to check
 * @param color color that the piece is
 * @return returns a vector of all positions on the board where the piece can move to
 */
vector<int> Positions::get_piece_moves(int piece_type, int square, int color) {
    vector<int> piece_available_positions;
    bb movement_board = 0ULL, piece_available_bb = 0ULL;
    bb opponent_board = this->occupancy[!color];

    // get moves for piece
    if (piece_type == P || piece_type == p) {
        movement_board = this->pawn_mask[color][square] & opponent_board; // side attacks
        if (movement_board == 0ULL)  movement_board = 0ULL; // reset if pawn doesnt have attacks

        // single push 
        // note: pawns are blocked by pieces in front of them
        bool single_push = false;
        int single_push_target = square + ((color == white) ? -8 : 8);
        int double_push_target = square + ((color == white) ? -16 : 16);

        if (!get_bit(opponent_board, single_push_target) && 
            !(piece_type == P && square >= a8 && square <= h8) && 
            !(piece_type == p && square >= a1 && square <= h1)) {
            add_bit(&movement_board, single_push_target);
            single_push = true;
        }

        // double push when pawn is on row 2 or 7
        // conditions: single push happened and the bit is empty 2 rows up
        if (single_push &&
            !get_bit(opponent_board, double_push_target) &&
            ((piece_type == P && square >= a2 && square <= h2) || (piece_type == p && square >= a7 && square <= h7)))
            add_bit(&movement_board, double_push_target);

        // en passante
        if (this->en_passant != 0) {
            int en_passant_piece = (color == white) ? this->en_passant + 8 : this->en_passant - 8;
            if ((square == (en_passant_piece + 1)) || (square == (en_passant_piece - 1)))
                add_bit(&movement_board, this->en_passant);
        }
    }
    else if (piece_type == N || piece_type == n) movement_board = this->knight_mask[square];
    else if (piece_type == B || piece_type == b) movement_board = this->get_bishop_magic_attack(square, this->occupancy[both]);
    else if (piece_type == R || piece_type == r) movement_board = this->get_rook_magic_attack(square, this->occupancy[both]);
    else if (piece_type == Q || piece_type == q) movement_board = this->get_queen_magic_attack(square, this->occupancy[both]);
    else {
        // default king moves
        movement_board = this->king_mask[square];

        // castling
        // rules:
        //  has the rights
        //  the square for the rook and king are open
        //  the starting, target, and middle squares are not attacked
        if (color == white) {
            if ((this->castling_rights & wk) && !get_bit(this->occupancy[both], f1) && !get_bit(this->occupancy[both], g1) &&
                !is_attacked(e1, !color) && !is_attacked(f1, !color) && !is_attacked(g1, !color)) 
                add_bit(&movement_board, g1);
            if ((this->castling_rights & wq) && !get_bit(this->occupancy[both], b1) && !get_bit(this->occupancy[both], c1) && !get_bit(this->occupancy[both], d1) &&
                !is_attacked(c1, !color) && !is_attacked(d1, !color) && !is_attacked(e1, !color)) 
                add_bit(&movement_board, c1);
        }
        else {
            if ((this->castling_rights & bk) && !get_bit(this->occupancy[both], f8) && !get_bit(this->occupancy[both], g8) &&
                !is_attacked(e8, !color) && !is_attacked(f8, !color) && !is_attacked(g8, !color)) 
                add_bit(&movement_board, g1);
            if ((this->castling_rights & bq) && !get_bit(this->occupancy[both], b8) && !get_bit(this->occupancy[both], c8) && !get_bit(this->occupancy[both], d8) &&
                !is_attacked(c8, !color) && !is_attacked(d8, !color) && !is_attacked(e8, !color)) 
                add_bit(&movement_board, c1);
        }
    }

    // filter blocks on own color
    piece_available_bb = movement_board & ~this->occupancy[color];

    // get moves that are available
    piece_available_positions = get_piece_squares(piece_available_bb);

    return piece_available_positions;
}

/**
 * Returns whether a specified square is under attack by other pieces
 * 
 * @param check_square square to check attacks
 * @param current_square square of current piece to remove
 * @return color color that is attacking the square
 */
bool Positions::is_attacked(int check_square, int color) {
    // attacked by pawns
    if ((color == white) && (this->pawn_mask[black][check_square] & this->pieces[P])) return true;
    if ((color == black) && (this->pawn_mask[white][check_square] & this->pieces[p])) return true;
    
    // attacked by knights
    if (this->knight_mask[check_square] & ((color == white) ? this->pieces[N] : this->pieces[n])) return true;
    
    // attacked by bishops
    if (get_bishop_magic_attack(check_square, occupancy[both]) & ((color == white) ? this->pieces[B] : this->pieces[b])) return true;

    // attacked by rooks
    if (get_rook_magic_attack(check_square, occupancy[both]) & ((color == white) ? this->pieces[R] : this->pieces[r])) return true;    

    // attacked by queen
    if (get_queen_magic_attack(check_square, occupancy[both]) & ((color == white) ? this->pieces[Q] : this->pieces[q])) return true;
    
    // attacked by kings
    if (this->king_mask[check_square] & ((color == white) ? this->pieces[K] : this->pieces[k])) return true;

    // by default return false
    return 0;
}

/**
 * Returns whether a king is in check
 * 
 * @return color color of the king to check
 * @return returns a whether the king is under attack
 */
bool Positions::is_check(int color) {
    // get king position
    bb king_board = this->pieces[10 + color];
    // king SHOULD always be on board, just precaution
    if (king_board == 0ULL) return false;

    int king_square = get_piece_squares(king_board)[0];
    if (is_attacked(king_square, !color)) 
        return true;
    
    // no checks
    return false;
}

/**
 * Returns whether a king is in checkmate aka game is over
 * 
 * @return color color of the king to check
 * @return returns a whether the king is no longer able to move
 */
bool Positions::is_checkmate(int color) {
    // get king position
    bb king_board = this->pieces[10 + color];
    // king SHOULD always be on board, just precaution
    if (king_board == 0ULL) return false;

    // get king moves based on its position
    int king_square = get_piece_squares(king_board)[0];
    vector<int> king_moves = get_piece_moves(K + color, king_square, color);

    // loop moves and check
    bool checkmate = true;

    for (int square : king_moves) {
        // make temp move for test
        make_move(K + color, king_square, square, color);

        // check to see if there is an attack
        if (!is_attacked(square, !color)) checkmate = false;

        // unmake move
        unmake_move();

        // no checkmate detected
        if (!checkmate) break;
    }
    cout << endl;

    // checkmate
    return checkmate;
}

/**
 * Gets the piece, moves the piece, adds to move stack
 *
 * @param piece_type indicates the board ex P is 0 = white pawn board
 * @param start_square square to start the move
 * @param end_square square to end the move
 * @param color color being moved (occupancy reasons)
 * @return void
 */
void Positions::make_move(int piece_type, int start_square, int end_square, int color, int castle_move) {
    int castled = 0;
    bool promoted = false;

    if (piece_type == K) {
        // white king side
        if (end_square == g1) {
            make_move(R, h1, f1, white, 1);
            castled = 1;
        }
        // white queen side
        else if (end_square == c1) {
            make_move(R, a1, d1, white, 1);
            castled = 1;
        }
    }
    else if (piece_type == k) {
        // white king side
        if (end_square == g8) {
            make_move(r, h8, f8, black, 1);
            castled = 1;
        }
        // white queen side
        else if (end_square == c8) {
            make_move(r, a8, d8, black, 1);
            castled = 1;
        }
    }

    // save positon
    this->save_position();

    // pawn actions
    if ((piece_type == P) || (piece_type == p)) {
        // en passant
        if ((abs(end_square - start_square) == 16)) {
            int en_passant_square = (color == white) ? end_square + 8 : end_square - 8;
            this->en_passant = en_passant_square;
        }
        else this->en_passant = 0;

        // promotions 
        if ((end_square >= a1 && end_square <= h1) || (end_square >= a8 && end_square <= h8)) {
            this->pawn_promote(piece_type, ((color == white) ? Q : q), start_square, end_square);
            promoted = true;
        }
    }

    // move board
    if (!promoted) move_bit(get_piece_board(piece_type), start_square, end_square);
    // move occupancy (color and both)
    move_bit(&this->occupancy[color], start_square, end_square);
    move_bit(&this->occupancy[both], start_square, end_square);

    // edit castling rules
    if (castled && piece_type == K) { this->castling_rights &= ~wk; this->castling_rights &= ~wq; }
    if (castled && piece_type == k) { this->castling_rights &= ~bk; this->castling_rights &= ~bq; }
    if (!castle_move && piece_type == R && start_square == h1) this->castling_rights &= ~wk; // white king side
    if (!castle_move && piece_type == R && start_square == a1) this->castling_rights &= ~wq; // white queen side
    if (!castle_move && piece_type == r && start_square == h8) this->castling_rights &= ~bk; // black king side
    if (!castle_move && piece_type == r && start_square == a8) this->castling_rights &= ~bq; // black queen side

    // create move && add to stack
    Move move;
    move.piece_type = piece_type;
    move.start_square = start_square;
    move.end_square = end_square;
    move.color = color;
    move.castle_move = castled;

    this->move_stack.push(move);
}

/**
 * Reverts board by unmaking the top move done
 *
 * @param piece_type indicates the board ex P is 0 = white pawn board
 * @param start_square square to start the move
 * @param end_square square to end the move
 * @param color color being moved (occupancy reasons)
 * @return void
 */
Move Positions::unmake_move() {
    // gets data from stack
    Move unmade_move = this->move_stack.top();

    // removes from stack
    this->move_stack.pop();

    // restore position
    this->restore_position();

    // move board
    move_bit(get_piece_board(unmade_move.piece_type), unmade_move.end_square, unmade_move.start_square);
    // move occupancy (color and both)
    move_bit(&this->occupancy[unmade_move.color], unmade_move.end_square, unmade_move.start_square);
    move_bit(&this->occupancy[both], unmade_move.end_square, unmade_move.start_square);

    // uncastle move
    if (unmade_move.castle_move == 1) unmake_move();

    return unmade_move;
}

/**
 * Gets the piece to remove and removes it 
 *
 * @param square current square the piece is occupying
 * @param color color to delete
 * @return boolean if removed
 */
bool Positions::piece_capture(int square, int color) {
    // loop to find piece to capture
    for (int piece = color; piece < 12; piece+=2) {
        bb* board = get_piece_board(piece);
        // board found that is to be taken
        if (get_bit(*board, square)) {
            remove_bit(board, square);
            return true;
        }
    }

    return false;
}

/**
 * Gets the previous move data
 *
 * @return list of previous move data
 */
Move Positions::get_previous_move() {
    Move top_stack;
    Move previous_stack;

    // perseve the top 
    top_stack = this->move_stack.top();
    this->move_stack.pop();
    previous_stack = this->move_stack.top();

    // adds back to the stack
    this->move_stack.push(top_stack);

    return previous_stack;    
}

/**
 * Promotes a pawn to a specified piece
 *
 * @param start_type piece type of starting piece
 * @param end_type piece type of ending piece
 * @return list of previous move data
 */
void Positions::pawn_promote(int start_type, int end_type, int starting_square, int ending_square) {
    remove_bit(this->get_piece_board(start_type), starting_square); // alter starting board
    add_bit(this->get_piece_board(end_type), ending_square); // alter ending board
}

/**
 * Gets the bishops available moves based on current positioning and occupancy
 *
 * @param square current square the bishop is occupying
 * @param curr_occupancy bitboard of all occupied squares
 * @return bitboard of available spaces the bishop can reach
 */
bb Positions::get_bishop_magic_attack(int square, bb curr_occupancy) {
    // get bishop attacks assuming current board occupancy
    curr_occupancy &= this->bishop_mask[square];
    // gets key for corresponding position
    int key = generate_key(curr_occupancy, bishop_magic[square], bishop_bits[square]);
    
    // return bishop attacks
    return bishop_magic_table[square][key];
}

/**
 * Gets the rook available moves based on current positioning and occupancy
 *
 * @param square current square the rook is occupying
 * @param curr_occupancy bitboard of all occupied squares
 * @return bitboard of available spaces the rook can reach
 */
bb Positions::get_rook_magic_attack(int square, bb curr_occupancy) {
    // get rook attacks assuming current board occupancy
    // print_bb(this->rook_mask[square]);
    curr_occupancy &= this->rook_mask[square];
    // gets key for corresponding position
    int key = generate_key(curr_occupancy, rook_magic[square], count_bits(this->rook_mask[square]));
    
    // return rook attacks
    return rook_magic_table[square][key];
}

/**
 * Gets the queen available moves based on current positioning and occupancy
 *
 * @param square current square the bishop is occupying
 * @param curr_occupancy bitboard of all occupied squares
 * @return bitboard of available spaces the queen can reach
 */
bb Positions::get_queen_magic_attack(int square, bb curr_occupancy) {
    // get bishops magic
    bb temp_bishop = get_bishop_magic_attack(square, curr_occupancy);
    // get rooks magic 
    bb temp_rook = get_rook_magic_attack(square, curr_occupancy);

    return temp_bishop | temp_rook;

}

/**
 * Generates all possible move for pawns based on positon.
 * 
 * @param color 0 = white, 1 = black
 * @param square position on board
 * @return board 64 bit unsigned integer board representation 
 */
bb Positions::generate_pawn_mask(int color, int square) {
    bb attacks = 0ULL;
    bb board = 0ULL;

    // board |= (1ULL << square);
    add_bit(&board, square);

    // // white pawn moves
    if (!color) {
        if ((board >> 7) & not_a) attacks |= (board >> 7);
        if ((board >> 9) & not_h) attacks |= (board >> 9);
    }
    // black pawn moves
    else {
        if ((board << 7) & not_h) attacks |= (board << 7);
        if ((board << 9) & not_a) attacks |= (board << 9);
    }

    return attacks;
}

/**
 * Generates all possible move for knights based on positon.
 * 
 * @param square position on board
 * @return board 64 bit unsigned integer board representation 
 */
bb Positions::generate_knight_mask(int square) {
    bb attacks = 0ULL;
    bb board = 0ULL;

    // board |= (1ULL << square);
    add_bit(&board, square);

    // knight moves
    if ((board >> 17) & not_h) attacks |= (board >> 17);
    if ((board >> 15) & not_a) attacks |= (board >> 15);
    if ((board >> 10) & not_hg) attacks |= (board >> 10);
    if ((board >> 6) & not_ab) attacks |= (board >> 6);
    if ((board << 17) & not_a) attacks |= (board << 17);
    if ((board << 15) & not_h) attacks |= (board << 15);
    if ((board << 10) & not_ab) attacks |= (board << 10);
    if ((board << 6) & not_hg) attacks |= (board << 6);

    return attacks;
}

/**
 * Generates all possible move for bishops based on positon.
 * 
 * @param square position on board
 * @return board 64 bit unsigned integer board representation 
 */
bb Positions::generate_bishop_mask(int square) {
    bb attacks = 0ULL;

    // init target rank and files
    int target_rank = square / 8; // 4
    int target_file = square % 8; // 4

    for (int rank = target_rank+1, file = target_file+1; rank < 7 && file < 7; ++rank, ++file) add_bit(&attacks, (rank * 8 + file)); // NW
    for (int rank = target_rank-1, file = target_file+1; rank > 0 && file < 7; --rank, ++file) add_bit(&attacks, (rank * 8 + file)); // NE
    for (int rank = target_rank+1, file = target_file-1; rank < 7 && file > 0; ++rank, --file) add_bit(&attacks, (rank * 8 + file)); // SW
    for (int rank = target_rank-1, file = target_file-1; rank > 0 && file > 0; --rank, --file) add_bit(&attacks, (rank * 8 + file)); // SE

    return attacks;
}

/**
 * Generates all possible move for rooks based on positon.
 * 
 * @param square position on board
 * @return board 64 bit unsigned integer board representation 
 */
bb Positions::generate_rook_mask(int square) {
    bb attacks = 0ULL;
    
    // init target rank and files
    int target_rank = square / 8;
    int target_file = square % 8;
    
    for (int rank = target_rank+1; rank < 7; ++rank) add_bit(&attacks, (rank * 8 + target_file)); // W
    for (int rank = target_rank-1; rank > 0; --rank) add_bit(&attacks, (rank * 8 + target_file)); // E
    for (int file = target_file+1; file < 7; ++file) add_bit(&attacks, (target_rank * 8 + file)); // N
    for (int file = target_file-1; file > 0; --file) add_bit(&attacks, (target_rank * 8 + file)); // S

    return attacks;
}

/**
 * Generates all possible move for queens based on positon.
 * 
 * @param square position on board
 * @return board 64 bit unsigned integer board representation 
 */
bb Positions::generate_queen_mask(int square) {
    bb attacks = 0ULL;

    bb bishop_attacks = 0ULL;
    bb rook_attacks = 0ULL;
    
    bishop_attacks = generate_bishop_mask(square);
    attacks |= bishop_attacks;

    rook_attacks = generate_rook_mask(square);
    attacks |= rook_attacks;

    return attacks;
}

/**
 * Generates all possible move for kings based on positon.
 * 
 * @param square position on board
 * @return board 64 bit unsigned integer board representation 
 */
bb Positions::generate_king_mask(int square) {
    bb attacks = 0ULL;
    bb board = 0ULL;

    // board |= (1ULL << square);
    add_bit(&board, square);

    // king moves
    if (board >> 8) attacks |= (board >> 8);
    if ((board >> 9) & not_h) attacks |= (board >> 9);
    if ((board >> 7) & not_a) attacks |= (board >> 7);
    if ((board >> 1) & not_h) attacks |= (board >> 1);
    if (board << 8) attacks |= (board << 8);
    if ((board << 9) & not_a) attacks |= (board << 9);
    if ((board << 7) & not_h) attacks |= (board << 7);
    if ((board << 1) & not_a) attacks |= (board << 1);

    return attacks;
}

/**
 * Stores the game's state into the stack.
 * 
 * @return void
 */
void Positions::save_position() {
    // Save position into memory
    State game_state = this->get_state();
    game_stack.push(game_state);
}

/**
 * Stores the game's state into the stack.
 * 
 * @return void
 */
void Positions::restore_position() {
    // Restore position from memory
    State game_state = this->game_stack.top();

    for (int i = 0; i < 12; ++i) {
        if (i < 3) this->occupancy[i] = game_state.occupancy_state[i];
        this->pieces[i] = game_state.pieces_state[i];
    }
    this->en_passant = game_state.en_passant_state;
    this->castling_rights = game_state.castling_state;

    this->game_stack.pop();
}

Move Positions::get_best_move(int color) {
    
    cout << "Finding move..." << endl;

    Move best_move = this->alpha_beta_max(-INFINITY, +INFINITY, 1, color);

    cout << "Move found..." << endl;

    return best_move;
}

int Positions::evaluate() {

    int eval = 0;

    for (int piece_type = 0; piece_type < 12; ++piece_type) {
        switch (piece_type) {
            case P:
                // evaluate pawn position
                for (int square: get_piece_squares(this->pieces[piece_type])) {
                    eval += pawn_score[square];
                }

                break;
            case p:
                break;
            case N:
                break;
            case n:
                break;
            case B:
                break;
            case b:
                break;
            case Q:
                break;
            case q:
                break;
            case K:
                break;
            case k:
                break;
        }
    }
    return eval;
}

Move Positions::alpha_beta_max(int alpha, int beta, int depth, int color) {
    if (depth == 0) {
        Move temp;
        temp.score = evaluate();
        return temp;
    }

    Move best_move;
    best_move.score = -INFINITY;

    for (Move curr_move : this->get_all_moves(color)) {
        // make move
        make_move(curr_move.piece_type, curr_move.start_square, curr_move.end_square, curr_move.color);

        // goto min 
        Move child_move = this->alpha_beta_min(alpha, beta, depth - 1, !curr_move.color);
        child_move.score = -child_move.score;  // min node

        // unmake move
        unmake_move();

        // swap if the move was better than the best move
        if (child_move.score > best_move.score) {
            best_move = curr_move;
            best_move.score = child_move.score;
        }

        alpha = max(alpha, best_move.score);
        if (beta <= alpha) 
            break;
    }

    return best_move;
}

Move Positions::alpha_beta_min(int alpha, int beta, int depth, int color) {
    if (depth == 0) {
        Move temp;
        temp.score = -evaluate();  // min node
        return temp;
    }

    Move best_move;
    best_move.score = INFINITY;

    for (Move curr_move : this->get_all_moves(color)) {
        // make move
        make_move(curr_move.piece_type, curr_move.start_square, curr_move.end_square, curr_move.color);

        // goto max
        Move child_move = this->alpha_beta_max(alpha, beta, depth - 1, !curr_move.color);

        // unmake move
        unmake_move();

        // swap if the move was "worse" than the best move
        if (child_move.score < best_move.score) {
            best_move = curr_move;
            best_move.score = child_move.score;
        }

        beta = min(beta, best_move.score);
        if (beta <= alpha) 
            break;
    }

    return best_move;
}

vector<Move> Positions::get_all_moves(int color) {
    vector<Move> all_moves;

    // loop for checking moves
    for (int piece_type = color; piece_type < 12; piece_type+=2) {
        // get each move for each square
        for (int square: get_piece_squares(this->pieces[piece_type])) {
            // iterate through each move
            for (int move: get_piece_moves(piece_type, square, color)) {
                Move temp;
                temp.piece_type = piece_type;
                temp.start_square = square;
                temp.end_square = move;
                temp.color = color;
                temp.score = 0;

                cout << temp.piece_type << " ";
                cout << temp.start_square << " ";
                cout << temp.end_square << " ";
                cout << temp.color;
                cout << endl;

                all_moves.push_back(temp);
            }
        }
    }

    return all_moves;
}