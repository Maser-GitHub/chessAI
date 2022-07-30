#pragma once
#define U64 unsigned long long
#define PLY 64
// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
//Change this toghether
#define INF 50000
#define MATE 49000
#define MATE_SCORE 48000
#include <vector>
#include <unordered_map>
#include <tuple>
#include <iostream>

using namespace std;
// enums

//to mask the flag of castling
enum {
	wk = 1, wq = 2, bk = 4, bq = 8
};

// encode pieces, upper for white
enum { P, N, B, R, Q, K, p, n, b, r, q, k };

// move types
enum {
	all_moves, only_capture
};

enum enumSquare {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};


enum enumSlider {
	rook, bishop
};

enum enumColor {
	white = 0, black = 1, both = 2
};

extern const char* square_to_coordinates[];

// ASCII pieces (+1 for the terminator)
extern char ascii_pieces[12 + 1];

// convert ASCII character pieces to encoded constants
extern int char_pieces[128];


// Mask all that is not a-file
extern const U64 not_a_file;
// Mask all that is not h-file
extern const U64 not_h_file;
// Mask all that is not a-file or b-file
extern const U64 not_ab_file;
// Mask all that is not h-file or h-file
extern const U64 not_hg_file;

// number of squares attacked from bishop
extern const int bishop_relevant_bits[64];

// number of squares attacked from rook
extern const int rook_relevant_bits[64];

// castling rights update constants
extern const int castling_rights[64] ;


// MVV LVA [attacker][victim]
extern int mvv_lva[12][12];

extern const int get_rank[64];
// killer moves [id][ply]
extern int killer_moves[2][PLY];

// history moves [piece][square]
extern int history_moves[12][64];

//file masks
extern U64 file_masks[64];
//rank masks
extern U64 rank_masks[64];

extern U64 isolated_masks[64];

extern U64 white_passed_masks[64];

extern U64 black_passed_masks[64];

extern const int double_pawn_penalty;

extern const int isolated_pawn_penalty;

extern const int passed_pawn_bonus[8];

extern const int semi_open_file_score;

extern const int open_file_score;

extern const int king_shield_bonus;




// BIT MANIPULATION

//set get pop macros

#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= (~(1ULL << (square))))
//#define count_bits(bitboard) __builtin_popcountll(bitboard)

// Definition for moves

#define encode_move(source, target, piece, promoted, capture, double_push, enpassant, castling) \
    ((source) |           \
    (target << 6) |      \
    (piece << 12) |      \
    (promoted << 16) |   \
    (capture << 20) |    \
    (double_push << 21) |\
    (enpassant << 22) |  \
    (castling << 23))     \

// extract source square
#define get_move_source(move) (move & 0x3f)

// extract target square
#define get_move_target(move) ((move & 0xfc0) >> 6)

// extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)

// extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)

// extract capture flag
#define get_move_capture(move) (move & 0x100000)

// extract double pawn push flag
#define get_move_double(move) (move & 0x200000)

// extract enpassant flag
#define get_move_enpassant(move) (move & 0x400000)

// extract castling flag
#define get_move_castling(move) (move & 0x800000)

#define copyboard() \
U64 bitboard_copy[12], occupancies_copy[3]; \
int side_copy, enpassant_copy, castle_copy; \
memcpy(bitboard_copy, bitboards, 96); \
memcpy(occupancies_copy, occupancies, 24); \
side_copy = side; enpassant_copy = enpassant; castle_copy = castle; \
U64 hash_key_copy = hash_key;

#define takeback() \
memcpy(bitboards, bitboard_copy, 96); \
memcpy(occupancies, occupancies_copy, 24); \
side = side_copy; enpassant = enpassant_copy; castle = castle_copy; \
hash_key = hash_key_copy;

//Counts bits in a bitboard
inline int count_bits(U64 bitboard);

// get LS1B index

extern inline int get_ls1b_index(U64 bitboard);

U64 set_file_rank_masks(int file, int rank);
