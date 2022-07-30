#pragma once
#include "Scores.h"


inline void generate_moves(vector<int>& moves);
int parse_move(const char* move_string);
inline void print_moves(vector<int> moves);
void print_move(int move);
extern inline int make_move(int move, int move_flag);
// TODO improve
extern inline void sort_moves(vector<int>& moves);