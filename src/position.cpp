#include <iostream>
#include "bitboard.h"
#include "magic.h"
#include "position.h"
using namespace std;

void Positions::init() {
    cout << "Initializing board..." << endl;

    cout << "Generating attack moves for each piece..." << endl;
    generate_attack_moves();

    cout << "Generating all games pieces..." << endl;
    generate_all_game_pieces();

    cout << "Initializing magic (bishop)..." << endl;
    init_bishop_table(this->bishop_mask);

    cout << "Initializing magic (rook)..." << endl;
    init_rook_table(this->rook_mask);
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
 * Generates all game pieces.
 * 
 * @param None
 * @return void
 */
void Positions::generate_all_game_pieces() {
    // pawns;
    this->pawns[white] = 0ULL;
    this->pawns[black] = 0ULL;
    for (int i = 0; i < 8; ++i) {
        this->pawns[white] |= (1ULL << (a2+i));
        this->pawns[black] |= (1ULL << (a7+i));
    }
    // knights
    this->knights[white] = 0ULL;
    this->knights[black] = 0ULL;
    this->knights[white] |= (1ULL << b1);
    this->knights[white] |= (1ULL << g1);
    this->knights[black] |= (1ULL << b8);
    this->knights[black] |= (1ULL << g8);
    // bishops
    this->bishops[white] = 0ULL;
    this->bishops[black] = 0ULL;
    this->bishops[white] |= (1ULL << c1);
    this->bishops[white] |= (1ULL << f1);
    this->bishops[black] |= (1ULL << c8);
    this->bishops[black] |= (1ULL << f8);
    // rooks
    this->rooks[white] = 0ULL;
    this->rooks[black] = 0ULL;
    this->rooks[white] |= (1ULL << a1);
    this->rooks[white] |= (1ULL << h1);
    this->rooks[black] |= (1ULL << a8);
    this->rooks[black] |= (1ULL << h8);
    // queens
    this->queens[white] = 0ULL;
    this->queens[black] = 0ULL;
    this->queens[white] |= (1ULL << d1);
    this->queens[black] |= (1ULL << d8);
    // kings
    this->kings[white] = 0ULL;
    this->kings[black] = 0ULL;
    this->kings[white] |= (1ULL << e1);
    this->kings[black] |= (1ULL << e8);
}

/**
 * Generates all possible move for pawns based on positon.
 * 
 * @param color : 0 = white, 1 = black
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
bb Positions::generate_pawn_mask(int color, int square) {
    bb attacks = 0ULL;
    bb board = 0ULL;

    board |= (1ULL << square);

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
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
bb Positions::generate_knight_mask(int square) {
    bb attacks = 0ULL;
    bb board = 0ULL;

    board |= (1ULL << square);

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
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
bb Positions::generate_bishop_mask(int square) {
    bb attacks = 0ULL;

    // init target rank and files
    int target_rank = square / 8; // 4
    int target_file = square % 8; // 4

    for (int rank = target_rank+1, file = target_file+1; rank < 7 && file < 7; ++rank, ++file) attacks |= (1ULL << (rank * 8 + file)); // NW
    for (int rank = target_rank-1, file = target_file+1; rank > 0 && file < 7; --rank, ++file) attacks |= (1ULL << (rank * 8 + file)); // NE
    for (int rank = target_rank+1, file = target_file-1; rank < 7 && file > 0; ++rank, --file) attacks |= (1ULL << (rank * 8 + file)); // SW
    for (int rank = target_rank-1, file = target_file-1; rank > 0 && file > 0; --rank, --file) attacks |= (1ULL << (rank * 8 + file)); // SE

    return attacks;
}

/**
 * Generates all possible move for rooks based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
bb Positions::generate_rook_mask(int square) {
    bb attacks = 0ULL;
    
    // init target rank and files
    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (int rank = target_rank+1; rank < 7; ++rank) attacks |= (1ULL << (rank * 8 + target_file)); // W
    for (int rank = target_rank-1; rank > 0; --rank) attacks |= (1ULL << (rank * 8 + target_file)); // E
    for (int file = target_rank+1; file < 7; ++file) attacks |= (1ULL << (target_rank * 8 + file)); // N
    for (int file = target_rank-1; file > 0; --file) attacks |= (1ULL << (target_rank * 8 + file)); // S

    return attacks;
}

/**
 * Generates all possible move for queens based on positon.
 * 
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
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
 * @param square : position on board
 * @return board : 64 bit unsigned integer board representation 
 */
bb Positions::generate_king_mask(int square) {
    bb attacks = 0ULL;
    bb board = 0ULL;

    board |= (1ULL << square);

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


void Positions::attacks_to(int position, int color, bb queen_board, bb pawn_board) {

    // king on a3

    // check queen
    // int queen_pos = __builtin_ctzll(queen_board);
    // if (this->queen_mask[queen_pos] & temp_king != 0) cout << true << endl;
    // printBoard(this->pawn_mask[!color][position]);
    // printBoard(this->pawns[!color]);

    // check pawn
    // if (this->pawn_mask[!color][position] & this->pawns[!color]) {
    //     cout << "attack" << endl;
    // }
    cout << "queen" << endl;
    queen_board |= (1ULL << a8);
    queen_board &= this->queen_mask[position];
    printBoard(queen_board);

    cout << "pawn" << endl;
    pawn_board |= (1ULL << b4);
    pawn_board &= this->pawn_mask[color][position];
    printBoard(pawn_board);

    printBoard(queen_board | pawn_board);

    // Logic: get queens attack board from kings perspective and see
    //        if the queen attack board sees the queens position.

    // cout << ((this->queen_mask[h8] & (1ULL << position)) != 0) << endl;

}