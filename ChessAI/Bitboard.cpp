#include "Bitboard.h"


U64 bitboards[12];
U64 occupancies[3];
int castle;
U64 hash_key;
U64 repetition_table[1000];
int ply;



int repetition_index = 0;

int side = white;

int enpassant = no_sq;

void print_bitboard(U64 bitboard) {
	//loop ranks
	for (int rank = 0; rank < 8; rank++) {
		//loop files
		for (int file = 0; file < 8; file++) {
			int square = rank * 8 + file;

			//print ranks
			if (!file) {
				cout << 8 - rank << "  ";
			}

			//print 1 if occupied, 0 if empty
			cout << (get_bit(bitboard, square) ? 1 : 0) << " ";
		}
		cout << endl;
	}
	//print files
	cout << endl << "a b c d e f g h" << endl << endl;

	//print bitboard
	cout << "   Bitboard: " << bitboard << endl;
}

//It uses global variables
void print_board() {
	int square, piece;
	int rank, file;
	for (rank = 0; rank < 8; rank++) {
		for (file = 0; file < 8; file++) {
			square = rank * 8 + file;

			piece = -1;

			//print ranks
			if (!file) {
				cout << 8 - rank << "  ";
			}

			for (int bb_piece = P; bb_piece <= k; bb_piece++) {
				if (get_bit(bitboards[bb_piece], square)) piece = bb_piece;
			}

			cout << ((piece == -1) ? '.' : ascii_pieces[piece]) << " ";
		}
		cout << endl;
	}
	//print files
	cout << endl << "   a b c d e f g h" << endl << endl;

	cout << "Side: " << (!side ? "White" : "Black") << endl;
	cout << "Enpassant: " << ((enpassant != no_sq) ? square_to_coordinates[enpassant] : "None") << endl;
	cout << "Castling: " << ((castle & wk) ? 'K' : '-') << ((castle & wq) ? 'Q' : '-') <<
		((castle & bk) ? 'k' : '-') << ((castle & bq) ? 'q' : '-') << endl;
	cout << "Hash key: " << hex << hash_key << endl;
}

//parse FEN

void parse_FEN(const char* fen) {
	//reset board
	memset(bitboards, 0ULL, sizeof(bitboards));
	memset(occupancies, 0ULL, sizeof(occupancies));
	side = white;
	enpassant = no_sq;
	castle = 0;

	hash_key = 0ULL;

	repetition_index = 0;

	memset(repetition_table, 0ULL, sizeof(repetition_table));

	ply = 0;

	for (int rank = 0; rank < 8; rank++) {
		for (int file = 0; file < 8; file++) {
			int square = rank * 8 + file;

			if (*fen >= 'a' && *fen <= 'z' || *fen >= 'A' && *fen <= 'Z') {
				int piece = char_pieces[*fen];

				set_bit(bitboards[piece], square);

				fen++;
			}

			if (*fen >= '0' && *fen <= '9') {
				//atoi for single digit
				int offset = (*fen) - '0';

				int piece = -1;

				//Checks for corner case where where the file starts with no piece
				for (int bb_piece = P; bb_piece <= k; bb_piece++)
				{
					// if there is a piece on current square
					if (get_bit(bitboards[bb_piece], square))
						// get piece code
						piece = bb_piece;
				}

				// on empty current square
				if (piece == -1)
					// decrement file
					file--;

				file += offset;

				fen++;
			}

			if (*fen == '/') fen++;
		}
	}
	// got to parsing side to move (increment pointer to FEN string)
	fen++;

	// parse side to move
	(*fen == 'w') ? (side = white) : (side = black);

	// go to parsing castling rights
	fen += 2;

	// parse castling rights
	while (*fen != ' ')
	{
		switch (*fen)
		{
		case 'K': castle |= wk; break;
		case 'Q': castle |= wq; break;
		case 'k': castle |= bk; break;
		case 'q': castle |= bq; break;
		default: break;
		}

		// increment pointer to FEN string
		fen++;
	}

	// got to parsing enpassant square (increment pointer to FEN string)
	fen++;

	// parse enpassant square
	if (*fen != '-')
	{
		// parse enpassant file & rank
		int file = fen[0] - 'a';
		int rank = 8 - (fen[1] - '0');

		// init enpassant square
		enpassant = rank * 8 + file;
	}
	else {
		enpassant = no_sq;
	}

	// loop over white pieces bitboards
	for (int piece = P; piece <= K; piece++)
		// populate white occupancy bitboard
		occupancies[white] |= bitboards[piece];

	// loop over black pieces bitboards
	for (int piece = p; piece <= k; piece++)
		// populate white occupancy bitboard
		occupancies[black] |= bitboards[piece];

	// init all occupancies
	occupancies[both] |= occupancies[white];
	occupancies[both] |= occupancies[black];
}