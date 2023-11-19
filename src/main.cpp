#include <iostream>
#include "bitboard.h"
#include "magic.h"
#include "position.h"
using namespace std;

int main() {
    // welcome
    cout << "==========================" << endl;
    cout << "= CHESS ENGINE | drjonah =" << endl;
    cout << "==========================\n" << endl;

    // bb queen_board = 0ULL;
    // bb pawn_board = 0ULL;

    // king on a3

    Positions game_pieces;
    game_pieces.init();

    // game_pieces.attacks_to(a3, white, queen_board, pawn_board);
    bb occupancy = 0ULL;
    occupancy |= (1ULL << d6);
    occupancy |= (1ULL << b4);
    occupancy |= (1ULL << c5);
    occupancy |= (1ULL << f4);

    printBoard(occupancy);

    // bb result = get_bishop_magic_attack(d4, occupancy, game_pieces.bishop_mask, game_pieces.bishop_table);
    // printBoard(result);

    // bb result = get_rook_magic_attack(d4, occupancy, game_pieces.rook_mask, game_pieces.rook_table);
    // printBoard(result);

    bb result = get_queen_magic_attack(d4, occupancy, game_pieces.queen_mask, game_pieces.bishop_mask, game_pieces.rook_mask, game_pieces.bishop_table, game_pieces.rook_table);
    printBoard(result);

    return 0;
}