/* ============================== *\

    Chess Engine by DrJonah
            game.cpp

    This file contains and runs
    the contents and logic for
    the chess engine. This
    includes eval and search.

\* ============================== */

#include <iostream>
#include <string>
#include "bitboard.h"
#include "game.h"
#include "../scripts/logger.h"
#include "position.h"
using namespace std;

void Game::init(Positions positions) {
    cout << "Initializing game..." << endl;
    this->positions = positions;
}

void Game::run() {
    bb* board = 0ULL;
    string input;
    int turn = this->positions.get_starting_color();
    int start_square, end_square, color, piece_type;

    while (true) {
        // add castling and en pasante
        print_readable(this->positions.pieces);

        // white or black
        color = (turn%2==0) ? white : black;

        // input
        cout << "[" << ((color == white) ? "white" : "black") << "] Enter move: ";
        cin >> input;

        if (input == "q") break;

        // clean input
        start_square = ((8 - stoi(input.substr(1,1))) * 8) + (input[0] - 'a');
        end_square = ((8 - stoi(input.substr(3))) * 8) + (input[2] - 'a');

        // find board to edit
        for (int piece = color; piece < 12; piece+=2) {
            board = this->positions.get_piece_board(piece);
            if (get_bit(*board, start_square)) {
                piece_type = piece;
                break;
            }
        }

        if (board != 0ULL) {
            this->positions.make_move(piece_type, start_square, end_square, color);
            ++turn;
            board = 0ULL;
        }

        // logger
        cout << this->positions.is_check(white) << endl;
        bool resp_log = log_turn(piece_type, start_square, end_square, color, 0, 0, 
                                 this->positions.piece_capture(end_square, !color), 
                                 this->positions.is_check(white), this->positions.is_check(black));

        if (!resp_log) cerr << "Logging error!" << endl;
        else cout << "Logging successful!" << endl;

        // check

        // checkmate
    }
}

void Game::end() {
    cout << "Game over." << endl;
}