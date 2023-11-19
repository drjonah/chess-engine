#include <iostream>
#include "bitboard.h"
using namespace std;

const bb not_a = 18374403900871474942ULL;
const bb not_h = 9187201950435737471ULL;
const bb not_ab = 4557430888798830399ULL;
const bb not_hg = 18229723555195321596ULL;

void printBoard(bb board) {
    for (int rank = 0; rank < 8; ++rank) {
        cout << 8 - rank << " | ";
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            cout << ((board & (1ULL << square)) != 0) << " ";
        }
        cout << endl;
    }
    cout << "  -----------------" << endl;
    cout << "    a b c d e f g h" << endl;
    cout << "Position: " << board << endl;
    cout << endl;
}
