#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "position.h"
using namespace std;

class Game {
    public:
        Game() = default;
        void init(Positions positions);
        void run();
        void end();

    private:
        // piece positions
        Positions positions;
};

#endif 