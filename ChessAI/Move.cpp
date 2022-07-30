#include "Move.h"
U64 get_queen_attacks(int, U64);

// TODO I could make this function longer but with fewer branches for efficency
void generate_moves(vector<int>& moves) {
	//init source-target
	int source_square, target_square, piece;
	//define bb copy
	U64 bitboard, attacks;

	if (side == white) {
		//White Pawns
		bitboard = bitboards[P];

		while (bitboard)
		{
			// init source square
			source_square = get_ls1b_index(bitboard);

			// init target square
			target_square = source_square - 8;

			// generate quite pawn moves
			if (!(target_square < a8) && !get_bit(occupancies[both], target_square))
			{
				// pawn promotion
				if (source_square >= a7 && source_square <= h7)
				{
					moves.push_back(encode_move(source_square, target_square, P, Q, 0, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, P, R, 0, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, P, B, 0, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, P, N, 0, 0, 0, 0));
				}

				else
				{
					// one square ahead pawn move
					moves.push_back(encode_move(source_square, target_square, P, 0, 0, 0, 0, 0));

					// two squares ahead pawn move
					if ((source_square >= a2 && source_square <= h2) && !get_bit(occupancies[both], target_square - 8))
						moves.push_back(encode_move(source_square, (target_square - 8), P, 0, 0, 1, 0, 0));
				}
			}
			// init pawn attacks bitboard
			attacks = pawn_attacks[side][source_square] & occupancies[black];

			// generate pawn captures
			while (attacks)
			{
				// init target square
				target_square = get_ls1b_index(attacks);

				// pawn promotion
				if (source_square >= a7 && source_square <= h7)
				{
					moves.push_back(encode_move(source_square, target_square, P, Q, 1, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, P, R, 1, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, P, B, 1, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, P, N, 1, 0, 0, 0));
				}

				else
					// one square ahead pawn move
					moves.push_back(encode_move(source_square, target_square, P, 0, 1, 0, 0, 0));

				// pop ls1b of the pawn attacks
				pop_bit(attacks, target_square);
			}

			// generate enpassant captures
			if (enpassant != no_sq)
			{
				// lookup pawn attacks and bitwise AND with enpassant square (bit)
				U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

				// make sure enpassant capture available
				if (enpassant_attacks)
				{
					// init enpassant capture target square
					int target_enpassant = get_ls1b_index(enpassant_attacks);
					moves.push_back(encode_move(source_square, target_enpassant, P, 0, 1, 0, 1, 0));
				}
			}

			// pop ls1b from piece bitboard copy
			pop_bit(bitboard, source_square);
		}

		//White king castle
		// king side castling is available
		if (castle & wk)
		{
			// make sure square between king and king's rook are empty
			if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1))
			{
				// make sure king and the f1 squares are not under attacks
				if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
					moves.push_back(encode_move(e1, g1, K, 0, 0, 0, 0, 1));
			}
		}

		// queen side castling is available
		if (castle & wq)
		{
			// make sure square between king and queen's rook are empty
			if (!get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1) && !get_bit(occupancies[both], b1))
			{
				// make sure king and the d1 squares are not under attacks
				if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
					moves.push_back(encode_move(e1, c1, K, 0, 0, 0, 0, 1));
			}
		}
	}
	else {
		//Black pawns
		bitboard = bitboards[p];
		// loop over black pawns within black pawn bitboard
		while (bitboard)
		{
			// init source square
			source_square = get_ls1b_index(bitboard);

			// init target square
			target_square = source_square + 8;

			// generate quite pawn moves
			if (!(target_square > h1) && !get_bit(occupancies[both], target_square))
			{
				// pawn promotion
				if (source_square >= a2 && source_square <= h2)
				{
					moves.push_back(encode_move(source_square, target_square, p, q, 0, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, p, r, 0, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, p, b, 0, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, p, n, 0, 0, 0, 0));
				}

				else
				{
					// one square ahead pawn move
					moves.push_back(encode_move(source_square, target_square, p, 0, 0, 0, 0, 0));

					// two squares ahead pawn move
					if ((source_square >= a7 && source_square <= h7) && !get_bit(occupancies[both], target_square + 8))
						moves.push_back(encode_move(source_square, (target_square + 8), p, 0, 0, 1, 0, 0));
				}
			}
			// init pawn attacks bitboard
			attacks = pawn_attacks[side][source_square] & occupancies[white];

			// generate pawn captures
			while (attacks)
			{
				// init target square
				target_square = get_ls1b_index(attacks);

				// pawn promotion
				if (source_square >= a2 && source_square <= h2)
				{
					moves.push_back(encode_move(source_square, target_square, p, q, 1, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, p, r, 1, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, p, b, 1, 0, 0, 0));
					moves.push_back(encode_move(source_square, target_square, p, n, 1, 0, 0, 0));
				}

				else
					// one square ahead pawn move
					moves.push_back(encode_move(source_square, target_square, p, 0, 1, 0, 0, 0));

				// pop ls1b of the pawn attacks
				pop_bit(attacks, target_square);
			}

			// generate enpassant captures
			if (enpassant != no_sq)
			{
				// lookup pawn attacks and bitwise AND with enpassant square (bit)
				U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

				// make sure enpassant capture available
				if (enpassant_attacks)
				{
					// init enpassant capture target square
					int target_enpassant = get_ls1b_index(enpassant_attacks);
					moves.push_back(encode_move(source_square, target_enpassant, p, 0, 1, 0, 1, 0));
				}
			}

			// pop ls1b from piece bitboard copy
			pop_bit(bitboard, source_square);
		}
		//Black castling
		// king side castling is available
		if (castle & bk)
		{
			// make sure square between king and king's rook are empty
			if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8))
			{
				// make sure king and the f8 squares are not under attacks
				if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
					moves.push_back(encode_move(e8, g8, k, 0, 0, 0, 0, 1));
			}
		}

		// queen side castling is available
		if (castle & bq)
		{
			// make sure square between king and queen's rook are empty
			if (!get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8) && !get_bit(occupancies[both], b8))
			{
				// make sure king and the d8 squares are not under attacks
				if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
					moves.push_back(encode_move(e8, c8, k, 0, 0, 0, 0, 1));
			}
		}
	}

	// I could make the following without branch for efficency:

	//Knight moves
	piece = (side == white) ? N : n;
	bitboard = bitboards[piece];

	// loop over source squares of piece bitboard copy
	while (bitboard)
	{
		// init source square
		source_square = get_ls1b_index(bitboard);

		// init piece attacks in order to get set of target squares
		attacks = knight_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

		// loop over target squares available from generated attacks
		while (attacks)
		{
			// init target square
			target_square = get_ls1b_index(attacks);

			// quite move
			if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
				moves.push_back(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

			else
				// capture move
				moves.push_back(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

			// pop ls1b in current attacks set
			pop_bit(attacks, target_square);
		}


		// pop ls1b of the current piece bitboard copy
		pop_bit(bitboard, source_square);
	}

	//Bishop moves

	piece = (side == white) ? B : b;
	bitboard = bitboards[piece];

	// loop over source squares of piece bitboard copy
	while (bitboard)
	{
		// init source square
		source_square = get_ls1b_index(bitboard);

		// init piece attacks in order to get set of target squares
		attacks = get_bishop_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

		// loop over target squares available from generated attacks
		while (attacks)
		{
			// init target square
			target_square = get_ls1b_index(attacks);

			// quite move
			if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
				moves.push_back(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

			else
				// capture move
				moves.push_back(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

			// pop ls1b in current attacks set
			pop_bit(attacks, target_square);
		}


		// pop ls1b of the current piece bitboard copy
		pop_bit(bitboard, source_square);
	}

	//Rook moves

	piece = (side == white) ? R : r;
	bitboard = bitboards[piece];

	// loop over source squares of piece bitboard copy
	while (bitboard)
	{
		// init source square
		source_square = get_ls1b_index(bitboard);

		// init piece attacks in order to get set of target squares
		attacks = get_rook_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

		// loop over target squares available from generated attacks
		while (attacks)
		{
			// init target square
			target_square = get_ls1b_index(attacks);

			// quite move
			if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
				moves.push_back(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

			else
				// capture move
				moves.push_back(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

			// pop ls1b in current attacks set
			pop_bit(attacks, target_square);
		}


		// pop ls1b of the current piece bitboard copy
		pop_bit(bitboard, source_square);
	}

	//Queen moves

	piece = (side == white) ? Q : q;
	bitboard = bitboards[piece];

	// loop over source squares of piece bitboard copy
	while (bitboard)
	{
		// init source square
		source_square = get_ls1b_index(bitboard);

		// init piece attacks in order to get set of target squares
		attacks = get_queen_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

		// loop over target squares available from generated attacks
		while (attacks)
		{
			// init target square
			target_square = get_ls1b_index(attacks);

			// quite move
			if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
				moves.push_back(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

			else
				// capture move
				moves.push_back(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

			// pop ls1b in current attacks set
			pop_bit(attacks, target_square);
		}


		// pop ls1b of the current piece bitboard copy
		pop_bit(bitboard, source_square);
	}

	piece = (side == white) ? K : k;
	bitboard = bitboards[piece];

	// loop over source squares of piece bitboard copy
	while (bitboard)
	{
		// init source square
		source_square = get_ls1b_index(bitboard);

		// init piece attacks in order to get set of target squares
		attacks = king_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

		// loop over target squares available from generated attacks
		while (attacks)
		{
			// init target square
			target_square = get_ls1b_index(attacks);

			// quite move
			if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
				moves.push_back(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

			else
				// capture move
				moves.push_back(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

			// pop ls1b in current attacks set
			pop_bit(attacks, target_square);
		}

		// pop ls1b of the current piece bitboard copy
		pop_bit(bitboard, source_square);
	}
}

// parse user/GUI move string input (e.g. "e7e8q")
int parse_move(const char* move_string)
{
	// create move list instance
	vector<int> moves;

	// generate moves
	generate_moves(moves);

	// parse source square
	int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;

	// parse target square
	int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

	// loop over the moves within a move list
	for (int move_count = 0; move_count < moves.size(); move_count++)
	{
		// init move
		int move = moves[move_count];

		// make sure source & target squares are available within the generated move
		if (source_square == get_move_source(move) && target_square == get_move_target(move))
		{
			// init promoted piece
			int promoted_piece = get_move_promoted(move);

			// promoted piece is available
			if (promoted_piece)
			{
				// promoted to queen
				if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q')
					// return legal move
					return move;

				// promoted to rook
				else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r')
					// return legal move
					return move;

				// promoted to bishop
				else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b')
					// return legal move
					return move;

				// promoted to knight
				else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n')
					// return legal move
					return move;

				// continue the loop on possible wrong promotions (e.g. "e7e8f")
				continue;
			}

			// return legal move
			return move;
		}
	}

	// return illegal move
	return 0;
}

void print_moves(vector<int> moves)
{
	cout << "move    piece     capture   double    enpass    castling" << endl;

	for (int move : moves) {
		cout << square_to_coordinates[get_move_source(move)];
		cout << square_to_coordinates[get_move_target(move)] << " ";
		cout << (get_move_promoted(move) ? ascii_pieces[get_move_promoted(move)] : ' ') << "    ";
		cout << ascii_pieces[get_move_piece(move)] << "          ";
		cout << (get_move_capture(move) ? 1 : 0) << "         ";
		cout << (get_move_double(move) ? 1 : 0) << "         ";
		cout << (get_move_enpassant(move) ? 1 : 0) << "         ";
		cout << (get_move_castling(move) ? 1 : 0);
		cout << endl;
	}

	cout << endl << "Total moves: " << moves.size() << endl;
}

//IMPLEMENTATION OF ALL MOVES

inline int make_move(int move, int move_flag) {
	//quite moves
	if (move_flag == all_moves) {
		//preserve state
		copyboard();


		int pawn = ((side == black) ? P : p);
		//parse move
		int source_square = get_move_source(move);
		int target_square = get_move_target(move);
		int piece = get_move_piece(move);
		int promoted = get_move_promoted(move);
		int capture = get_move_capture(move);
		int double_push = get_move_double(move);
		int enpass = get_move_enpassant(move);
		int castling = get_move_castling(move);


		//move piece
		pop_bit(bitboards[piece], source_square);
		set_bit(bitboards[piece], target_square);

		//hash piece
		hash_key ^= piece_keys[piece][source_square];
		hash_key ^= piece_keys[piece][target_square];

		//handle capture
		if (capture) {
			for (int bb_piece = pawn; bb_piece < pawn + 6; bb_piece++) {
				if (get_bit(bitboards[bb_piece], target_square)) {
					//remove from board
					pop_bit(bitboards[bb_piece], target_square);

					//remove from hash
					hash_key ^= piece_keys[bb_piece][target_square];
					break;
				}
			}


		}

		//handle promotions
		if (promoted) {//remove the other pawn
			pop_bit(bitboards[6 - pawn], target_square);
			set_bit(bitboards[promoted], target_square);

			hash_key ^= piece_keys[6 - pawn][target_square];
			hash_key ^= piece_keys[promoted][target_square];
		}

		//handle enpassant
		if (enpass) {
			int target_square_enpassant;
			target_square_enpassant = (side == white) ? (target_square + 8) : (target_square - 8);
			pop_bit(bitboards[pawn], target_square_enpassant);
			hash_key ^= piece_keys[pawn][target_square_enpassant];
		}

		//remove enpassant
		if (enpassant != no_sq) hash_key ^= enpassant_keys[enpassant];

		//Reset enpassant
		enpassant = no_sq;

		//handle double push
		if (double_push) {
			int enpassant_square = ((side == white) ? (target_square + 8) : (target_square - 8));
			enpassant = enpassant_square;
			hash_key ^= enpassant_keys[enpassant_square];
		}

		//handle castle
		if (castling) {
			switch (target_square)
			{
				// white castles king side
			case (g1):
				// move H rook
				pop_bit(bitboards[R], h1);
				set_bit(bitboards[R], f1);

				hash_key ^= piece_keys[R][h1];
				hash_key ^= piece_keys[R][f1];
				break;

				// white castles queen side
			case (c1):
				// move A rook
				pop_bit(bitboards[R], a1);
				set_bit(bitboards[R], d1);

				hash_key ^= piece_keys[R][a1];
				hash_key ^= piece_keys[R][d1];
				break;

				// black castles king side
			case (g8):
				// move H rook
				pop_bit(bitboards[r], h8);
				set_bit(bitboards[r], f8);

				hash_key ^= piece_keys[r][h8];
				hash_key ^= piece_keys[r][f8];
				break;

				// black castles queen side
			case (c8):
				// move A rook
				pop_bit(bitboards[r], a8);
				set_bit(bitboards[r], d8);

				hash_key ^= piece_keys[r][a8];
				hash_key ^= piece_keys[r][d8];
				break;
			}
		}

		//remove castle from hash
		hash_key ^= castle_keys[castle];

		//update castle rights
		castle &= castling_rights[source_square];
		castle &= castling_rights[target_square]; //This prevents funny bug

		//insert castle in hash even if not changed
		hash_key ^= castle_keys[castle];

		//update occupancies //Maybe it is faster using pop_bit and set_bit everytime
		occupancies[white] = bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K];
		occupancies[black] = bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k];
		occupancies[both] = occupancies[white] | occupancies[black];

		//change side
		side ^= 1;

		hash_key ^= side_key;

		//Check if king is attacked
		if (is_square_attacked((side == white) ? get_ls1b_index(bitboards[k]) : get_ls1b_index(bitboards[K]), side)) {
			//move is illegal
			takeback();
			return 0;
		}
		else {
			return 1;
		}
	}
	//capture
	else {
		if (get_move_capture(move)) make_move(move, all_moves);
		else return 0;
	}

	return 0;
}

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
	U64 occupancy = 0ULL;

	for (int count = 0; count < bits_in_mask; count++) {
		//gets the LS1B
		int square = get_ls1b_index(attack_mask);

		//remove LS1B
		pop_bit(attack_mask, square);

		//index is the "map" of the ls1b to show
		if (index & (1 << count)) occupancy |= (1ULL << square);
	}

	return occupancy;
}

void print_move(int move) {
	cout << square_to_coordinates[get_move_source(move)];
	cout << square_to_coordinates[get_move_target(move)] << " ";
}

// TODO improve
inline void sort_moves(vector<int>& moves) {
	vector<int> move_score;

	for (int i = 0; i < moves.size(); i++) {
		move_score.push_back(score_move(moves[i]));
	}

	for (int i = 0; i < moves.size(); i++) {
		for (int j = i + 1; j < moves.size(); j++) {
			if (move_score[i] < move_score[j]) {
				move_score[i] ^= move_score[j];
				move_score[j] ^= move_score[i];
				move_score[i] ^= move_score[j];
				moves[i] ^= moves[j];
				moves[j] ^= moves[i];
				moves[i] ^= moves[j];
			}
		}
	}
}