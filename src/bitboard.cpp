#include <iostream>
#include "bitboard.h"

namespace Chess {

const uint64_t not_a = 18374403900871474942ULL;
const uint64_t not_h = 9187201950435737471ULL;
const uint64_t not_ab = 4557430888798830399ULL;
const uint64_t not_hg = 18229723555195321596ULL;

void printBoard(uint64_t board) {
    for (int rank = 0; rank < 8; ++rank) {
        std::cout << 8 - rank << " | ";
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << ((board & (1ULL << square)) != 0) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "  -----------------" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "Position: " << board << std::endl;
    std::cout << std::endl;
}

}