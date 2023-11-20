#include <iostream>
#include <string>

#include "bitboard.h"
#include "game.h"
#include "position.h"

using namespace std;

void Game::init(Positions* positions) {
    cout << "Initializing game..." << endl;
    this->positions = positions;
}

void Game::run() {
    string input;
    char start_piece, end_piece;
    int turn = 0, start_square, end_square;

    while (true) {
        // input
        cout << "Enter move: ";
        cin >> input;

        // clean input
        start_piece = input[0];
        start_square = stoi(input.substr(1,1));
        end_piece = input[2];
        end_square = stoi(input.substr(3,3));
        
    }
}