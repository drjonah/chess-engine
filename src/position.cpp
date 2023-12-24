/* ============================== *\

    Chess Engine by DrJonah
         position.cpp

    This file initializes all 
    game masks and pieces. It 
    handles initialization as
    well as manages attacks,
    checks, and checkmates.

\* ============================== */

#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "bitboard.h"
#include "magic.h"
#include "position.h"
using namespace std;

// constructor
// Positions::Positions() : starting_color(0),
//           en_passant(true),
//           castling_rights(0ULL),
//           pawn_mask{{0ULL}},
//           knight_mask{0ULL},
//           king_mask{0ULL},
//           bishop_mask{0ULL},
//           rook_mask{0ULL},
//           queen_mask{0ULL},
//           occupancy{0ULL, 0ULL, 0ULL},
//           pieces{0ULL} {}

// initialize game
void Positions::init(string FEN_args[4]) {
    // setting up game using FEN
    cout << "Generating all games pieces..." << endl;
    generate_all_game_pieces(FEN_args[0]);

    cout << "Generating game properties..." << endl;
    generate_game_properties((FEN_args[1].at(0) == 'w') ? white : black, FEN_args[2], FEN_args[3]);

    // attack masks including magic 
    cout << "Generating attack masks for each piece..." << endl;
    generate_attack_moves();

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

    // default the board
    for (int i = 0; i < 12; ++i) this->pieces[i] = 0ULL;
    for (int i = 0; i < 12; ++i) this->pieces_copy[i] = 0ULL;
    for (int i = 0; i < 3; ++i) this->occupancy[i] = 0ULL;
    for (int i = 0; i < 3; ++i) this->occupancy_copy[i] = 0ULL;

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
void Positions::generate_game_properties(int starting_color, string castling_rights, string en_passant) {
    this->starting_color = starting_color;

    // castling rights
    this->castling_rights = 0;
    for (char piece : castling_rights) {
        // white side castling
        if (piece == 'K') { this->castling_rights |= wk; castling_rights_copy |= wk; } 
        if (piece == 'Q') { this->castling_rights |= wq; castling_rights_copy |= wq; }

        // black side castling
        if (piece == 'k') { this->castling_rights |= bk; castling_rights_copy |= bk; }
        if (piece == 'q') { this->castling_rights |= bq; castling_rights_copy |= bq; }
    }

    // en_passants
    this->en_passant = 1;
    this->en_passant_copy = 1; 
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
 * Returns who is the starting color
 * 
 * @return starting color (0 = white, 1 = black)
 */
int Positions::get_starting_color() {
    return this->starting_color;
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
 * Returns the available positions that can be moved to by a piece and color
 * 
 * @param piece_type piece type to check
 * @param square square to check
 * @param color color that the piece is
 * @return returns a vector of all positions on the board where the piece can move to
 */
vector<int> Positions::get_available_moves(int piece_type, int square, int color) {
    vector<int> piece_available_positions;
    bb movement_board = 0ULL, piece_available_bb = 0ULL;
    bb opponent_board = this->occupancy[!color];

    // get moves for piece
    if (piece_type == P || piece_type == p) {
        movement_board = this->pawn_mask[color][square]; // side attacks
        if ((opponent_board & movement_board) == 0ULL) movement_board = 0ULL; // reset if pawn doesnt have attacks

        // single push 
        bool single_push = false;
        int single_push_target = square + ((color == white) ? -8 : 8);
        int double_push_target = square + ((color == white) ? -16 : 16);

        if (!get_bit(opponent_board, single_push_target)) {
            // movement_board |= (1ULL << (square + ((color == white) ? -8 : 8)));
            add_bit(&movement_board, (square + ((color == white) ? -8 : 8)));
            single_push = true;
        }

        // double push when pawn is on row 2 or 7
        // conditions: single push happened and the bit is empty 2 rows up
        if (single_push &&
            !get_bit(opponent_board, double_push_target) &&
            ((piece_type == P && square >= a2 && square <= h2) || (piece_type == p && square >= a7 && square <= h7)))
            // movement_board |= (1ULL << (square + ((color == white) ? -16 : 16)));
            add_bit(&movement_board, (square + ((color == white) ? -16 : 16)));
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
    vector<int> king_moves = get_available_moves(K + color, king_square, color);

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
    // make castle move
    int castled = 0;

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

    // move board
    move_bit(get_piece_board(piece_type), start_square, end_square);
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

    // add to stack
    this->move_stack.push({piece_type, start_square, end_square, color, castled});
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
vector<int> Positions::unmake_move() {
    vector<int> unmade;

    // gets data from stack
    unmade.push_back(this->move_stack.top()[0]); // piece_type
    unmade.push_back(this->move_stack.top()[1]); // start_square
    unmade.push_back(this->move_stack.top()[2]); // end_square
    unmade.push_back(this->move_stack.top()[3]); // color
    unmade.push_back(this->move_stack.top()[4]); // castle_move

    // removes from stack
    this->move_stack.pop();

    // restore position
    this->restore_position();

    // move board
    move_bit(get_piece_board(unmade[0]), unmade[2], unmade[1]);
    // move occupancy (color and both)
    move_bit(&this->occupancy[unmade[3]], unmade[2], unmade[1]);
    move_bit(&this->occupancy[both], unmade[2], unmade[1]);

    // uncastle move
    if (unmade[4] == 1) unmake_move();

    return unmade;
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

    // white pawn moves
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

    // for (int rank = target_rank+1, file = target_file+1; rank < 7 && file < 7; ++rank, ++file) attacks |= (1ULL << (rank * 8 + file)); // NW
    // for (int rank = target_rank-1, file = target_file+1; rank > 0 && file < 7; --rank, ++file) attacks |= (1ULL << (rank * 8 + file)); // NE
    // for (int rank = target_rank+1, file = target_file-1; rank < 7 && file > 0; ++rank, --file) attacks |= (1ULL << (rank * 8 + file)); // SW
    // for (int rank = target_rank-1, file = target_file-1; rank > 0 && file > 0; --rank, --file) attacks |= (1ULL << (rank * 8 + file)); // SE

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
    
    // mask relevant bishop occupancy bits
    // for (int rank = target_rank+1; rank < 7; ++rank) attacks |= (1ULL << (rank * 8 + target_file)); // W
    // for (int rank = target_rank-1; rank > 0; --rank) attacks |= (1ULL << (rank * 8 + target_file)); // E
    // for (int file = target_file+1; file < 7; ++file) attacks |= (1ULL << (target_rank * 8 + file)); // N
    // for (int file = target_file-1; file > 0; --file) attacks |= (1ULL << (target_rank * 8 + file)); // S

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

void Positions::save_position() {
    // Save position into memory
    memcpy(this->pieces_copy, this->pieces, 96);
    memcpy(this->occupancy_copy, this->occupancy, 24);

    this->en_passant_copy = this->en_passant;
    this->castling_rights_copy = this->castling_rights;
}

void Positions::restore_position() {
    // Restore position from memory
    memcpy(this->pieces, this->pieces_copy, 96);
    memcpy(this->occupancy, this->occupancy_copy, 24);

    this->en_passant = this->en_passant_copy;
    this->castling_rights = this->castling_rights_copy;
}
