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

const string squares[] = {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
                          "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
                          "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
                          "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
                          "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
                          "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
                          "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
                          "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};


void Game::init(Positions& positions, string FEN_board) {
    cout << "Initializing game..." << endl;
    this->positions = positions;

    // deal with FEN
    string FEN_args[4];
    this->parse_FEN(FEN_board, FEN_args);
    this->positions.init(FEN_args);
}

void Game::run() {
    bb* board = 0ULL;
    string input, color_str;
    int start_square, end_square, color, piece_type;
    bool resp_log, captured, checkmate;

    color = this->positions.get_starting_color();
    checkmate = false;

    while (true) {
        // print
        print_readable(this->positions.pieces);

        // white or black
        color = (color%2 == white) ? white : black;

        // input
        color_str = (color == white) ? "white" : "black";
        cout << "[" << color_str << "] Enter move: ";
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

        // make move
        if (board != 0ULL) {
            this->positions.make_move(piece_type, start_square, end_square, color);
            captured = this->positions.piece_capture(end_square, !color);

            // update clock
            // rules: halfmove = pawn or captured
            //        fullmove = after black moves
            if (piece_type == P || piece_type == p || captured) this->halfmove_clock = 0;
            else ++this->halfmove_clock;

            if (color%2 == black) ++this->fullmove_number; 

            ++color;
            board = 0ULL;
        }

        // check / checkmate for colors
        for (int check_color = white; check_color <= black; ++check_color) {
            color_str = (check_color == white) ? "white" : "black";
            if (this->positions.is_check(white)) {
                cout << color_str << " is in check." << endl;
                if (this->positions.is_checkmate(white)) {
                    cout << color_str << " is in checkmate." << endl; 
                    checkmate = true;
                    break;
                }
            }
        }

        // logger
        resp_log = log_turn(piece_type, squares[start_square], 
                            squares[end_square], color_str, 
                            0, 0, 
                            this->halfmove_clock, this->fullmove_number,
                            captured, 
                            this->positions.is_check(white), this->positions.is_checkmate(white), 
                            this->positions.is_check(black), this->positions.is_checkmate(black));

        if (!resp_log) cerr << "Logging error!" << endl;
        else cout << "Logging successful!" << endl;

        // checkmate
        if (checkmate) break;
    }
}

void Game::end() {
    cout << "\nGame over." << endl;
    print_readable(this->positions.pieces);
}

void Game::parse_FEN(string FEN_board, string FEN_args[4]) {
    // FEN ex. rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    cout << "Initializing board (FEN)..." << endl;

    int clock_args[2];
    int counter = 0;
    while (counter < 6) {
        int position = FEN_board.find(" ");

        // differentiate where args go 
        if (counter < 4)
            FEN_args[counter] = FEN_board.substr(0, position);
        else
            clock_args[counter - 4] = stoi(FEN_board.substr(0, position));

        FEN_board.erase(0, position+1);
        ++counter;
    }

    // init clock
    this->halfmove_clock = clock_args[0];
    this->fullmove_number = clock_args[1];
}