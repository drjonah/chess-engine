/* ============================== *\

    Chess Engine by DrJonah
            logger.cpp

    This file logs data to ./logs

\* ============================== */

#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include "logger.h"
using namespace std;

string path = "./logs/";

/**
 * This function generates the timestamp
 *      
 * @param log_type this is the label for filename
 * @return timestamp
 */
string generate_timestamp(string log_type) {
    string timestamp;

    // get current time 
    auto current_time = chrono::system_clock::now();
    time_t time_t_now = chrono::system_clock::to_time_t(current_time);
    tm* current_time_info = localtime(&time_t_now);

    timestamp = "[ " + log_type + " @ "
                + to_string(current_time_info->tm_hour) + ':'
                + to_string(current_time_info->tm_min) + ':'
                + to_string(current_time_info->tm_sec) 
                + " ]";

    return timestamp;
}

/**
 * This function generates the file name
 *      
 * @param log_type this is the label for filename
 * @return filename
 */
string generate_file_name(string log_type) {
    string file_name;

    // get current time 
    auto current_time = chrono::system_clock::now();
    time_t time_t_now = chrono::system_clock::to_time_t(current_time);
    tm* current_time_info = localtime(&time_t_now);

    file_name = log_type +
                '_' + to_string(current_time_info->tm_year)  +
                '-' + to_string(current_time_info->tm_mon+1) +
                '-' + to_string(current_time_info->tm_mday)
                + ".txt";

    return file_name;
}

/**
 * This function takes in file stream object and opens a specified file path
 * 
 * @param stream ofstream object for writing to a file
 * @param path location of logs to be stored
 */
bool open_file(ofstream& write_stream, string log_type) {
    // generate file name
    string file_name = path + generate_file_name(log_type);

    write_stream.open(file_name, ios::app);
    if (!write_stream.is_open()) {
        cerr << "file failed to open. [PATH=" << file_name << "]" << endl;
        return false;
    }

    return true;
}

bool log_turn(int piece_type, int start_square, int end_square, 
              int color, int white_score, int black_score,
              bool removed, bool white_king, bool black_king) {
    string log_type = "turn_log";
    // file
    ofstream write_stream;
    if (!open_file(write_stream, log_type)) return false;

    // write
    write_stream << "=========================" << endl;
    write_stream << generate_timestamp(log_type) << endl;
    
    write_stream << "Movement Change" << endl;
    write_stream << "  piece type  : " << piece_type << endl;
    write_stream << "  piece color : " << color << endl;
    write_stream << "  movement    : " << start_square << " -> " << end_square << endl;
    write_stream << "  murderer    : " << ((removed) ? "yes" : "no") << endl;

    write_stream << "Current Score" << endl;
    write_stream << "  white : " << white_score << endl;
    write_stream << "  black : " << white_score << endl;

    write_stream << "Pieces Under Attack" << endl;
    write_stream << "  none" << endl;

    write_stream << "King Safety" << endl;
    write_stream << "  white : " << ((white_king) ? "safe" : "in danger") << endl;
    write_stream << "  black : " << ((black_king) ? "safe" : "in danger") << endl;

    write_stream << "=========================" << endl << endl;

    write_stream.close();

    return true;
}

void log_search() {

}