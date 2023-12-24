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
    // string starting_board = "k7/8/8/8/8/8/r6r/3K4 b KQkq - 0 1"; // checkmate in 1 board
    // string starting_board = "k7/8/8/8/5p2/5p2/r4P1r/3K4 b KQkq - 0 1";

    // castling tests
    string starting_board = "k7/8/8/8/8/8/r6r/R3K2R w KQkq - 0 1";
    // string starting_board = "k7/8/8/8/8/8/r6r/R1B1KB1R w KQkq - 0 1";
    // string starting_board = "k3r3/8/8/8/8/8/r6r/R3K2R w KQkq - 0 1";

    // objects
    Positions game_pieces;
    Game game;

    // run
    game.init(game_pieces, starting_board); // initializes game
    game.run(); // runs the game
    game.end(); // ends the game

    return 0;
}