#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "position.h"
using namespace std;

extern const string squares[64];

class Game {
    public:
        Game() = default;
        void init(Positions& positions, string FEN_board);
        void run();
        void end();

    private:
        void parse_FEN(string FEN_board, string FEN_args[4]);

    private:
        // piece positions
        Positions positions;

        // clock
        int halfmove_clock;
        int fullmove_number;
};

#endif 