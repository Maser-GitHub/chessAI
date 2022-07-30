#include "Init.h"

// convert ASCII character pieces to encoded constants
int char_pieces[128];


void init_all() {
	init_char();
	init_leaper_attacks();
	init_slider_attacks(bishop);
	init_slider_attacks(rook);
	init_score();
	init_random_keys();
	hash_table.clear();
	init_evaluation_masks();
	//init_bitboards(); LATER Add this for performance when you will need to implement multiple boards
}

void init_random_keys()
{
	// update pseudo random number state
	int random_state = 1804289383;

	// loop over piece codes
	for (int piece = P; piece <= k; piece++)
	{
		// loop over board squares
		for (int square = 0; square < 64; square++) {
			// init random piece keys
			piece_keys[piece][square] = get_random_U64_number();
		}
	}

	// loop over board squares
	for (int square = 0; square < 64; square++)
		// init random enpassant keys
		enpassant_keys[square] = get_random_U64_number();

	// loop over castling keys
	for (int i = 0; i < 16; i++)
		// init castling keys
		castle_keys[i] = get_random_U64_number();

	// init random side key
	side_key = get_random_U64_number();
}


void init_char() {
	char_pieces['P'] = P;
	char_pieces['N'] = N;
	char_pieces['B'] = B;
	char_pieces['R'] = R;
	char_pieces['Q'] = Q;
	char_pieces['K'] = K;
	char_pieces['p'] = p;
	char_pieces['n'] = n;
	char_pieces['b'] = b;
	char_pieces['r'] = r;
	char_pieces['q'] = q;
	char_pieces['k'] = k;
}

void init_score() {
	for (int i = 0; i < 64; i++) {
		all_scores[0][i] = pawn_score[i];
		all_scores[1][i] = knight_score[i];
		all_scores[2][i] = bishop_score[i];
		all_scores[3][i] = rook_score[i];
		all_scores[4][i] = queen_score[i];
		all_scores[5][i] = king_score[i];
	}
}


void init_evaluation_masks() {
	for (int r = 0; r < 8; r++) {
		for (int f = 0; f < 8; f++) {
			int square = r + f * 8;
			//Files
			file_masks[square] = set_file_rank_masks(f, -1);
			//Ranks
			rank_masks[square] = set_file_rank_masks(-1, r);
			//Isolated pawns
			isolated_masks[square] = set_file_rank_masks(f - 1, -1);
			isolated_masks[square] |= set_file_rank_masks(f + 1, -1);
			//White passed pawns
			white_passed_masks[square] = set_file_rank_masks(f - 1, -1);
			white_passed_masks[square] |= set_file_rank_masks(f, -1);
			white_passed_masks[square] |= set_file_rank_masks(f + 1, -1);
			for (int i = 0; i < (7 - r); i++)
				white_passed_masks[square] &= ~rank_masks[(7 - i) * 8 + f];
			//Black passed pawns
			black_passed_masks[square] = set_file_rank_masks(f - 1, -1);
			black_passed_masks[square] |= set_file_rank_masks(f, -1);
			black_passed_masks[square] |= set_file_rank_masks(f + 1, -1);
			for (int i = 0; i < r + 1; i++)
				black_passed_masks[square] &= ~rank_masks[i * 8 + f];
		}
	}
}

void init_leaper_attacks() {
	for (int square = 0; square < 64; square++) {
		pawn_attacks[white][square] = mask_pawn_attacks(white, square);
		pawn_attacks[black][square] = mask_pawn_attacks(black, square);
	}
	for (int square = 0; square < 64; square++) {
		knight_attacks[square] = mask_knight_attacks(square);
	}
	for (int square = 0; square < 64; square++) {
		king_attacks[square] = mask_king_attacks(square);
	}
}

void init_slider_attacks(int bishop) {
	for (int square = 0; square < 64; square++) {
		bishop_masks[square] = mask_bishop_attacks(square);
		rook_masks[square] = mask_rook_attacks(square);
		//Init current mask
		U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

		//init occupancy bit count
		int relevant_bits = count_bits(attack_mask);

		int occupancy_indices = (1 << relevant_bits);

		//loop occupancy_indices
		for (int index = 0; index < occupancy_indices; index++) {
			if (bishop) {//bishop
				U64 occupancy = set_occupancy(index, relevant_bits, attack_mask);

				int magic_index = (int)((occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]));

				bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
			}
			else {//rook
				U64 occupancy = set_occupancy(index, relevant_bits, attack_mask);

				int magic_index = (int)((occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]));

				rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
			}
		}
	}
}