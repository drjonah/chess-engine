#include <iostream>
#include "bitboard.h"
#include "position.h"

int main() {
    // welcome
    std::cout << "==========================" << std::endl;
    std::cout << "= CHESS ENGINE | drjonah =" << std::endl;
    std::cout << "==========================\n" << std::endl;

    Chess::Pieces pieces;
    pieces.attacks_to(Chess::a1);

    return 0;
}