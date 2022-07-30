#pragma once
#include "Utility.h"
//DEFINITION BITBOARDS

// piece bitboards
extern U64 bitboards[12];

// occupancy bitboards
extern U64 occupancies[3];

// side to move
extern int side;

// enpassant square
extern int enpassant;

// castling rights
extern int castle;

//hash unique identifier
extern U64 hash_key;

//max ply in entire game TODO make better
extern U64 repetition_table[1000];

extern int repetition_index;

//half moves
extern int ply;

// Print BitBoard

void print_bitboard(U64 bitboard);

//It uses global variables
void print_board();

//parse FEN

void parse_FEN(const char* fen);