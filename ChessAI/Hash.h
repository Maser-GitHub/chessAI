#pragma once
#include "Bitboard.h"
// HASH

//random piece keys
extern U64 piece_keys[12][64];
//random enpassant keys
extern U64 enpassant_keys[64];
//random castle keys
extern U64 castle_keys[16];
//random side key;
extern U64 side_key;

extern int random_state;

//In the tuple the U64 is the hash_key, the int are: depth, flag, score
//get<0> returns hash_key, get<1> depth, get<2> flag, get<3> score
extern unordered_map<U64, tuple<U64, int, int, int>> hash_table;

// transposition table hash flags
enum { hash_flag_exact, hash_flag_alpha, hash_flag_beta };


//generate the hash key of the current board state
U64 generate_hash_key();

extern inline int read_hash_entry(int alpha, int beta, int depth);

// int depth, int hash_flag, int score
extern inline void write_hash_entry(int depth, int hash_flag, int score);

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number();
// generate 64-bit pseudo legal numbers
U64 get_random_U64_number();

void initialize_values(const char* fen);