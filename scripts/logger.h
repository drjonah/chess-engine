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
bool log_turn(int piece_type, int start_square, int end_square, 
              int color, int white_score, int black_score,
              bool removed, bool white_king, bool black_king);
void log_search();

#endif