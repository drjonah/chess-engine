/* ============================== *\

    Chess Engine by DrJonah
            main.cpp

    This file initializes and
    runs the whole engine.

\* ============================== */

#include <iostream>
#include <string>
#include "game.h"
#include "position.h"
using namespace std;

int main() {
    // welcome
    cout << "==========================" << endl;
    cout << "= CHESS ENGINE | drjonah =" << endl;
    cout << "==========================\n" << endl;

    // FEN style boards
    // string starting_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // string starting_board = "rnb1kbnr/ppppp2p/8/8/pp6/2q5/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string starting_board = "k7/8/8/8/8/8/r6r/3K4 b KQkq - 0 1";

    Positions game_pieces;

    Game game;
    game.init(game_pieces, starting_board); // initializes game
    game.run(); // runs the game
    game.end(); // ends the game

    return 0;
}