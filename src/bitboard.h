#ifndef BITBOARD_H
#define BITBOARD_H

#include <iostream>
#include <vector>

using namespace std;

// colors for printing readable board
#define RESET   "\033[0m" // RESET
#define GREEN   "\033[32m" // TRADITIONAL BLACK
#define YELLOW  "\033[33m" // TRADITIONAL WHITE
#define CYAN    "\033[36m" // BOARD
#define WHITE   "\033[37m" // LABELS

enum { white, black, both };
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};
enum {
    P, p,
    N, n,
    B, b,
    R, r, 
    Q, q, 
    K, k
};

typedef uint64_t bb;

extern const bb not_a;
extern const bb not_h;
extern const bb not_ab;
extern const bb not_hg;

vector<int> get_piece_squares(bb board);

int count_bits(bb board);
int get_lsb(bb board);

void remove_bit(bb* board, int square);
void move_bit(bb* board, int starting_square, int ending_square);
bool get_bit(bb bitboard, int square);

void print_bb(bb board);
void print_readable(bb* pieces);

#endif 