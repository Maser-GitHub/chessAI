#pragma once
#include "Move.h"

// leaf nodes (number of positions reached during the test of the move generator at a given depth)
extern long nodes;

extern const int full_depth_moves;
extern const int reduction_limit;


inline void enable_pv_scoring(vector<int>& moves);

//repetition detection
inline bool is_repetition();

inline int quiescence(int alpha, int beta);

inline int negamax(int alpha, int beta, int depth);

void search_position(int depth);