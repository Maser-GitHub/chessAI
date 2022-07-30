#include "Utility.h"


const char* square_to_coordinates[] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

// ASCII pieces (+1 for the terminator)
char ascii_pieces[12 + 1] = "PNBRQKpnbrqk";

// Mask all that is not a-file
const U64 not_a_file = 0xfefefefefefefefeULL;
// Mask all that is not h-file
const U64 not_h_file = 0x7f7f7f7f7f7f7f7fULL;
// Mask all that is not a-file or b-file
const U64 not_ab_file = 0xfcfcfcfcfcfcfcfcULL;
// Mask all that is not h-file or h-file
const U64 not_hg_file = 0x3f3f3f3f3f3f3f3fULL;

// number of squares attacked from bishop
const int bishop_relevant_bits[64]{
	 6, 5, 5, 5, 5, 5, 5, 6,
	 5, 5, 5, 5, 5, 5, 5, 5,
	 5, 5, 7, 7, 7, 7, 5, 5,
	 5, 5, 7, 9, 9, 7, 5, 5,
	 5, 5, 7, 9, 9, 7, 5, 5,
	 5, 5, 7, 7, 7, 7, 5, 5,
	 5, 5, 5, 5, 5, 5, 5, 5,
	 6, 5, 5, 5, 5, 5, 5, 6
};

// number of squares attacked from rook
const int rook_relevant_bits[64]{
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};

// castling rights update constants
const int castling_rights[64] = {
	 7, 15, 15, 15,  3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14
};


// MVV LVA [attacker][victim]
int mvv_lva[12][12] = {
	105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

	105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

const int get_rank[64] =
{
	7, 7, 7, 7, 7, 7, 7, 7,
	6, 6, 6, 6, 6, 6, 6, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	4, 4, 4, 4, 4, 4, 4, 4,
	3, 3, 3, 3, 3, 3, 3, 3,
	2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0
};

int killer_moves[2][PLY];

int history_moves[12][64];

U64 file_masks[64];
U64 rank_masks[64];
U64 isolated_masks[64];
U64 white_passed_masks[64];
U64 black_passed_masks[64];

const int double_pawn_penalty = -10;

const int isolated_pawn_penalty = -10;

const int passed_pawn_bonus[8] = { 0, 10, 30, 50, 75, 100, 150, 200 };

const int semi_open_file_score = 10;

const int open_file_score = 15;

const int king_shield_bonus = 5;

//Counts bits in a bitboard
inline int count_bits(U64 bitboard) {
	int count = 0;

	while (bitboard) {
		count++;
		// Magic bit manipulation that removes the LS1B
		bitboard &= bitboard - 1;
	}

	return count;
}

// get LS1B index

inline int get_ls1b_index(U64 bitboard) {
	//check bitboard != 0
	if (!bitboard) return -1;
	//isolate LS1B, then subract 1 counting the ones created
	return count_bits((bitboard & (0 - bitboard)) - 1);
}

U64 set_file_rank_masks(int file, int rank) {
	U64 mask = 0ULL;

	for (int r = 0; r < 8; r++) {
		for (int f = 0; f < 8; f++) {
			int square = r * 8 + f;
			if (f == file || r == rank) {
				set_bit(mask, square);
			}
		}
	}

	return mask;
}
