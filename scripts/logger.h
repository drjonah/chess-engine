#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern string path;

// helpers
string generate_timestamp(string log_type);
string generate_file_name(string log_type);
bool open_file(ofstream& stream, string log_type);

// loggers
bool log_turn(int piece_type, string start_square, 
              string end_square, string color, 
              int white_score, int black_score,
              int halfmove_clock, int fullmove_number,
              bool removed, 
              bool white_king_check, bool white_king_checkmate,
              bool black_king_check, bool black_king_checkmate);
void log_search();

#endif