#include <iostream>

#include "bitboard.h"
#include "game.h"
#include "position.h"

using namespace std;

int main() {
    // welcome
    cout << "==========================" << endl;
    cout << "= CHESS ENGINE | drjonah =" << endl;
    cout << "==========================\n" << endl;

    Positions* game_pieces = new Positions();
    game_pieces->init();

    Game game;
    game.init(game_pieces); 
    game.run();

    // game_pieces.attacks_to(a3, white, queen_board, pawn_board);
    // bb occupancy = 0ULL;
    // occupancy |= (1ULL << d6);
    // occupancy |= (1ULL << b4);
    // occupancy |= (1ULL << c5);
    // occupancy |= (1ULL << f4);

    // print_bb(occupancy);

    // bb result = get_bishop_magic_attack(d4, occupancy, game_pieces.bishop_mask);
    // print_bb(result);

    // bb result = get_rook_magic_attack(d4, occupancy, game_pieces.rook_mask);
    // print_bb(result);

    // bb result = get_queen_magic_attack(d4, occupancy, game_pieces.queen_mask, game_pieces.bishop_mask, game_pieces.rook_mask);
    // print_bb(result);

    return 0;
}