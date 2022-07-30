#include "Scores.h"

U64 get_queen_attacks(int, U64);

int pv_length[PLY];
int pv_table[PLY][PLY];

bool follow_pv, score_pv;

int material_score[12] = {
	100,      // white pawn score
	300,      // white knight scrore
	350,      // white bishop score
	500,      // white rook score
   1000,      // white queen score
  10000,      // white king score
   -100,      // black pawn score
   -300,      // black knight scrore
   -350,      // black bishop score
   -500,      // black rook score
  -1000,      // black queen score
 -10000,      // black king score
};


//This will be the core of the AI

// pawn positional score
const int pawn_score[64] =
{
	90,  90,  90,  90,  90,  90,  90,  90,
	30,  30,  30,  40,  40,  30,  30,  30,
	20,  20,  20,  30,  30,  30,  20,  20,
	10,  10,  10,  20,  20,  10,  10,  10,
	 5,   5,  10,  20,  20,   5,   5,   5,
	 0,   0,   0,   5,   5,   0,   0,   0,
	 0,   0,   0, -10, -10,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] =
{
	-5,   0,   0,   0,   0,   0,   0,  -5,
	-5,   0,   0,  10,  10,   0,   0,  -5,
	-5,   5,  20,  20,  20,  20,   5,  -5,
	-5,  10,  20,  30,  30,  20,  10,  -5,
	-5,  10,  20,  30,  30,  20,  10,  -5,
	-5,   5,  20,  10,  10,  20,   5,  -5,
	-5,   0,   0,   0,   0,   0,   0,  -5,
	-5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] =
{
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,  10,  10,   0,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,  10,   0,   0,   0,   0,  10,   0,
	 0,  30,   0,   0,   0,   0,  30,   0,
	 0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rook_score[64] =
{
	50,  50,  50,  50,  50,  50,  50,  50,
	50,  50,  50,  50,  50,  50,  50,  50,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,   0,  20,  20,   0,   0,   0

};

// queen positional score
const int queen_score[64] =
{
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0
};

// king positional score
const int king_score[64] =
{
	 0,   0,   0,   0,   0,   0,   0,   0,
	 0,   0,   5,   5,   5,   5,   0,   0,
	 0,   5,   5,  10,  10,   5,   5,   0,
	 0,   5,  10,  20,  20,  10,   5,   0,
	 0,   5,  10,  20,  20,  10,   5,   0,
	 0,   0,   5,  10,  10,   5,   0,   0,
	 0,   5,   5,  -5,  -5,   0,   5,   0,
	 0,   0,   5,   0, -15,   0,  10,   0
};

// mirror positional score tables for opposite side
const int mirror_score[64] =
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

int all_scores[6][64];

inline int evaluate()
{
	// evaluation score
	int score = 0, square;
	// penalty
	int double_pawns = 0;

	// current pieces bitboard copy
	U64 bitboard;

	//have to modify if you want to weight based on the position
	// loop over piece bitboards
	for (int bb_piece = P; bb_piece <= k; bb_piece++)
	{
		// init piece bitboard copy
		bitboard = bitboards[bb_piece];

		// loop over pieces within a bitboard
		while (bitboard)
		{
			// init square
			square = get_ls1b_index(bitboard);

			// score material weights
			score += material_score[bb_piece];

			score += (bb_piece <= 5) ? all_scores[bb_piece][square] : -all_scores[bb_piece - 6][mirror_score[square]];

			//Extra evaluation with pawns penalty and open/semiopen files
			switch (bb_piece) {
			case P:
				// double pawn penalty
				double_pawns = count_bits(bitboards[P] & file_masks[square]);

				// double pawns
				if (double_pawns > 1)
					score += double_pawns * double_pawn_penalty;

				// isolated pawn
				if ((bitboards[P] & isolated_masks[square]) == 0)
					score += isolated_pawn_penalty;

				// passed pawn
				if ((white_passed_masks[square] & bitboards[p]) == 0)
					score += passed_pawn_bonus[get_rank[square]];
				break;
			case p:
				// double pawn penalty
				double_pawns = count_bits(bitboards[p] & file_masks[square]);

				// double pawns
				if (double_pawns > 1)
					score -= double_pawns * double_pawn_penalty;

				// isolated pawnd
				if ((bitboards[p] & isolated_masks[square]) == 0)
					score -= isolated_pawn_penalty;

				// passed pawn
				if ((black_passed_masks[square] & bitboards[P]) == 0)
					score -= passed_pawn_bonus[get_rank[mirror_score[square]]];
				break;
			case R:
				// semi open file
				if ((bitboards[P] & file_masks[square]) == 0)
					score += semi_open_file_score;

				// semi open file
				if (((bitboards[P] | bitboards[p]) & file_masks[square]) == 0)
					score += open_file_score;
				break;


			case r:
				// semi open file
				if ((bitboards[p] & file_masks[square]) == 0)
					score -= semi_open_file_score;

				// semi open file
				if (((bitboards[P] | bitboards[p]) & file_masks[square]) == 0)
					score -= open_file_score;

				break;
			case K:
				// semi open file
				if ((bitboards[P] & file_masks[square]) == 0)
					score -= semi_open_file_score;

				// semi open file
				if (((bitboards[P] | bitboards[p]) & file_masks[square]) == 0)
					score -= open_file_score;

				// king safety bonus
				score += count_bits(king_attacks[square] & occupancies[white]) * king_shield_bonus;
				break;
			case k:
				// semi open file
				if ((bitboards[p] & file_masks[square]) == 0)
					score += semi_open_file_score;

				// semi open file
				if (((bitboards[P] | bitboards[p]) & file_masks[square]) == 0)
					score += open_file_score;

				// king safety bonus
				score -= count_bits(king_attacks[square] & occupancies[black]) * king_shield_bonus;
				break;
			case B:
				// mobility
				score += count_bits(get_bishop_attacks(square, occupancies[both]));
				break;
			case b:
				// mobility
				score -= count_bits(get_bishop_attacks(square, occupancies[both]));
				break;
			case Q:
				// mobility
				score += count_bits(get_queen_attacks(square, occupancies[both]));
				break;
			case q:
				// mobility
				score -= count_bits(get_queen_attacks(square, occupancies[both]));
				break;
			}
			// pop ls1b
			pop_bit(bitboard, square);
		}
	}

	// return final evaluation based on side
	return (side == white) ? score : -score;
}

inline int score_move(int move) {
	//pv score allowed
	if (score_pv) {
		if (pv_table[0][ply] == move) {
			score_pv = false;
			return 20000;
		}
	}

	//if capture
	if (get_move_capture(move)) {
		int pawn = (side == white) ? (p) : (P);
		int target_piece = pawn; //if the loop doesn't find a piece is because of enpassant, so the target is enemy pawn
		int target_square = get_move_target(move);
		//TODO for for half pieces
		for (int bb_piece = pawn; bb_piece < pawn + 6; bb_piece++) {
			if (get_bit(bitboards[bb_piece], target_square)) {
				target_piece = bb_piece;
			}
		}
		//score MVV LVA
		return mvv_lva[get_move_piece(move)][target_piece] + 10000;
	}
	//quiet move
	//score 1st killer
	if (killer_moves[0][ply] == move) return 9000;
	//score 2nd killer
	if (killer_moves[1][ply] == move) return 8000;
	//score history
	return history_moves[get_move_piece(move)][get_move_target(move)];
}
