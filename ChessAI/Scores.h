#pragma once
#include "Attack.h"
// material score
extern int pv_length[PLY];
extern int pv_table[PLY][PLY];

// flags
extern bool follow_pv, score_pv;


extern int material_score[12];


//This will be the core of the AI

// pawn positional score
extern const int pawn_score[64];
// knight positional score
extern const int knight_score[64];

// bishop positional score
extern const int bishop_score[64];

// rook positional score
extern const int rook_score[64];

// queen positional score
extern const int queen_score[64];

// king positional score
extern const int king_score[64];

// mirror positional score tables for opposite side
extern const int mirror_score[64];

extern int all_scores[6][64];

// position evaluation
extern inline int evaluate();

extern inline int score_move(int move);