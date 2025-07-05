#include <string>

#ifndef MATRIX_H
#define MATRIX_H

extern char matrix[3][3];

extern char matrix_orderned[3][3];


std::string get_key();
bool check_victory(char matrix[3][3], char matrix_orderned[3][3]);
void text_to_speech(const std::string &text);
void display_matrix(int moves);
void shuffle_matrix();
void start();

#endif