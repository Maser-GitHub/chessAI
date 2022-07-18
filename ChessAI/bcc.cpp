// CHANGED APPROACH USING BITBOARD
// Following methods on the site www.chessprogramming.org
// TODO remove global variables
#include <iostream>
#include <vector>

using namespace std;

#define U64 unsigned long long

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

enum enumSlider {
	rook, bishop
};

enum enumColor {
	white = 0, black = 1, both = 2
};

//DEFINITION BITBOARDS

// piece bitboards
U64 bitboards[12];

// occupancy bitboards
U64 occupancies[3];

// side to move
int side = white;

// enpassant square
int enpassant = no_sq;

// castling rights
int castle;

// ASCII pieces (+1 for the terminator)
char ascii_pieces[12 + 1] = "PNBRQKpnbrqk";

// convert ASCII character pieces to encoded constants
int char_pieces[128];

//Ascii_pieces from index to ascii. char_pieces from ascii to index

// BIT MANIPULATION

//set get pop macros

#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= (~(1ULL << (square))))
#define count_bits(bitboard) __builtin_popcountll(bitboard)

// Definition for moves

#define encode_move(source, target, piece, promoted, capture, double_push, enpassant, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (double_push << 21) |    \
    (enpassant << 22) | \
    (castling << 23)    \

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


// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "

// Copy/Restore

#define copyboard() \
U64 bitboard_copy[12], occupancies_copy[3]; \
int side_copy, enpassant_copy, castle_copy; \
memcpy(bitboard_copy, bitboards, 96); \
memcpy(occupancies_copy, occupancies, 24); \
side_copy = side; enpassant_copy = enpassant; castle_copy = castle;

#define takeback() \
memcpy(bitboards, bitboard_copy, 96); \
memcpy(occupancies, occupancies_copy, 24); \
side = side_copy; enpassant = enpassant_copy; castle = castle_copy;


//Convert the file in .c and use __builtin_popcountll(bitboard) if bottleneck
//Counts bits in a bitboard
static inline int count_bits(U64 bitboard) {
	int count = 0;

	while (bitboard) {
		count++;
		// Magic bit manipulation that removes the LS1B
		bitboard &= bitboard - 1;
	}

	return count;
}

// get LS1B index

static inline int get_ls1b_index(U64 bitboard) {
	//check bitboard != 0
	if (!bitboard) return -1;
	//isolate LS1B, then subract 1 counting the ones created
	return count_bits((bitboard & (0 - bitboard)) - 1);
}


// material scrore


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

// position evaluation
static inline int evaluate()
{
	// evaluation score
	int score = 0, square;

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

			// score positional piece scores
			score += (bb_piece <= 5) ? all_scores[bb_piece][square] : -all_scores[bb_piece - 6][mirror_score[square]];

			// pop ls1b
			pop_bit(bitboard, square);
		}
	}

	// return final evaluation based on side
	return (side == white) ? score : -score;
}




// Print BitBoard


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
	cout << endl << "   a b c d e f g h" << endl << endl;

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
}


//parse FEN

void parse_FEN(const char* fen) {
	//reset board
	memset(bitboards, 0ULL, sizeof(bitboards));
	memset(occupancies, 0ULL, sizeof(occupancies));
	side = white;
	enpassant = no_sq;
	castle = 0;

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


// ATTACKS

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

//Those magic numbers are needed in order to filter the rook attack given the current occupancy of the board

// rook magic numbers
U64 rook_magic_numbers[64] = {
	0x8a80104000800020ULL,
	0x140002000100040ULL,
	0x2801880a0017001ULL,
	0x100081001000420ULL,
	0x200020010080420ULL,
	0x3001c0002010008ULL,
	0x8480008002000100ULL,
	0x2080088004402900ULL,
	0x800098204000ULL,
	0x2024401000200040ULL,
	0x100802000801000ULL,
	0x120800800801000ULL,
	0x208808088000400ULL,
	0x2802200800400ULL,
	0x2200800100020080ULL,
	0x801000060821100ULL,
	0x80044006422000ULL,
	0x100808020004000ULL,
	0x12108a0010204200ULL,
	0x140848010000802ULL,
	0x481828014002800ULL,
	0x8094004002004100ULL,
	0x4010040010010802ULL,
	0x20008806104ULL,
	0x100400080208000ULL,
	0x2040002120081000ULL,
	0x21200680100081ULL,
	0x20100080080080ULL,
	0x2000a00200410ULL,
	0x20080800400ULL,
	0x80088400100102ULL,
	0x80004600042881ULL,
	0x4040008040800020ULL,
	0x440003000200801ULL,
	0x4200011004500ULL,
	0x188020010100100ULL,
	0x14800401802800ULL,
	0x2080040080800200ULL,
	0x124080204001001ULL,
	0x200046502000484ULL,
	0x480400080088020ULL,
	0x1000422010034000ULL,
	0x30200100110040ULL,
	0x100021010009ULL,
	0x2002080100110004ULL,
	0x202008004008002ULL,
	0x20020004010100ULL,
	0x2048440040820001ULL,
	0x101002200408200ULL,
	0x40802000401080ULL,
	0x4008142004410100ULL,
	0x2060820c0120200ULL,
	0x1001004080100ULL,
	0x20c020080040080ULL,
	0x2935610830022400ULL,
	0x44440041009200ULL,
	0x280001040802101ULL,
	0x2100190040002085ULL,
	0x80c0084100102001ULL,
	0x4024081001000421ULL,
	0x20030a0244872ULL,
	0x12001008414402ULL,
	0x2006104900a0804ULL,
	0x1004081002402ULL
};

// bishop magic numbers
U64 bishop_magic_numbers[64] = {
	0x40040844404084ULL,
	0x2004208a004208ULL,
	0x10190041080202ULL,
	0x108060845042010ULL,
	0x581104180800210ULL,
	0x2112080446200010ULL,
	0x1080820820060210ULL,
	0x3c0808410220200ULL,
	0x4050404440404ULL,
	0x21001420088ULL,
	0x24d0080801082102ULL,
	0x1020a0a020400ULL,
	0x40308200402ULL,
	0x4011002100800ULL,
	0x401484104104005ULL,
	0x801010402020200ULL,
	0x400210c3880100ULL,
	0x404022024108200ULL,
	0x810018200204102ULL,
	0x4002801a02003ULL,
	0x85040820080400ULL,
	0x810102c808880400ULL,
	0xe900410884800ULL,
	0x8002020480840102ULL,
	0x220200865090201ULL,
	0x2010100a02021202ULL,
	0x152048408022401ULL,
	0x20080002081110ULL,
	0x4001001021004000ULL,
	0x800040400a011002ULL,
	0xe4004081011002ULL,
	0x1c004001012080ULL,
	0x8004200962a00220ULL,
	0x8422100208500202ULL,
	0x2000402200300c08ULL,
	0x8646020080080080ULL,
	0x80020a0200100808ULL,
	0x2010004880111000ULL,
	0x623000a080011400ULL,
	0x42008c0340209202ULL,
	0x209188240001000ULL,
	0x400408a884001800ULL,
	0x110400a6080400ULL,
	0x1840060a44020800ULL,
	0x90080104000041ULL,
	0x201011000808101ULL,
	0x1a2208080504f080ULL,
	0x8012020600211212ULL,
	0x500861011240000ULL,
	0x180806108200800ULL,
	0x4000020e01040044ULL,
	0x300000261044000aULL,
	0x802241102020002ULL,
	0x20906061210001ULL,
	0x5a84841004010310ULL,
	0x4010801011c04ULL,
	0xa010109502200ULL,
	0x4a02012000ULL,
	0x500201010098b028ULL,
	0x8040002811040900ULL,
	0x28000010020204ULL,
	0x6000020202d0240ULL,
	0x8918844842082200ULL,
	0x4010011029020020ULL
};


//Pawn attacks table [side][square]

U64 pawn_attacks[2][64];

//Knight attacks table[square]

U64 knight_attacks[64];

//King attacks table[square]

U64 king_attacks[64];

//Bishop attack masks table[square]
U64 bishop_masks[64];

// Bishop attacks table[square][occupancies]
U64 bishop_attacks[64][512];

//Rook attacks mask[square]

U64 rook_masks[64];

//Rook attacks table[square][occupancies]
U64 rook_attacks[64][4096];

//generate mask pawn attack
U64 mask_pawn_attacks(int side, int square) {
	U64 attacks = 0ULL;

	U64 bitboard = 0ULL;
	//set piece
	set_bit(bitboard, square);

	//not_a_file and not_h_file in order to prevent row overflow
	if (!side) { //white
		attacks |= (bitboard & not_h_file) >> 7;
		attacks |= (bitboard & not_a_file) >> 9;
	}
	else { //black
		attacks |= (bitboard & not_a_file) << 7;
		attacks |= (bitboard & not_h_file) << 9;
	}

	return attacks;
}

//Generate mask knight attack

U64 mask_knight_attacks(int square) {
	U64 attacks = 0ULL;

	U64 bitboard = 0ULL;
	//set piece
	set_bit(bitboard, square);

	//not_ab_file and not_hg_file in order to prevent row overflow
	attacks |= (bitboard & not_h_file) >> 15;
	attacks |= (bitboard & not_a_file) >> 17;
	attacks |= (bitboard & not_hg_file) >> 6;
	attacks |= (bitboard & not_ab_file) >> 10;
	attacks |= (bitboard & not_a_file) << 15;
	attacks |= (bitboard & not_h_file) << 17;
	attacks |= (bitboard & not_ab_file) << 6;
	attacks |= (bitboard & not_hg_file) << 10;

	return attacks;
}

//Generate mask king attack

U64 mask_king_attacks(int square) {
	U64 attacks = 0ULL;

	U64 bitboard = 0ULL;
	//set piece
	set_bit(bitboard, square);

	//not_a_file and not_h_file in order to prevent row overflow
	attacks |= (bitboard & not_h_file) << 9;
	attacks |= (bitboard & not_h_file) << 1;
	attacks |= (bitboard & not_h_file) >> 7;
	attacks |= (bitboard & not_a_file) >> 9;
	attacks |= (bitboard & not_a_file) >> 1;
	attacks |= (bitboard & not_a_file) << 7;
	attacks |= bitboard << 8;
	attacks |= bitboard >> 8;

	return attacks;
}

//Generate mask bishop attack
U64 mask_bishop_attacks(int square) {
	U64 attacks = 0ULL;

	//ranks & files
	int r, f;
	//target r&f
	int tr = square / 8;
	int tf = square % 8;

	//Slider without the edge
	for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
	for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
	for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
	for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));


	return attacks;
}

//Generate mask rook attack
U64 mask_rook_attacks(int square) {
	U64 attacks = 0ULL;

	//ranks & files
	int r, f;
	//target r&f
	int tr = square / 8;
	int tf = square % 8;

	//Slider without the edge
	for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
	for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
	for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
	for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

	return attacks;
}

//Generate bishop attack on the fly
U64 bishop_attacks_on_the_fly(int square, U64 block) {
	U64 attacks = 0ULL;

	//ranks & files
	int r, f;
	//target r&f
	int tr = square / 8;
	int tf = square % 8;

	//Sliders that stops when blocked
	//I use the break because I would have to make a new variable flag since it has to exit the loop after the action is done when condition is met
	for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & block) break;
	}
	for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & block) break;
	}
	for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & block) break;
	}
	for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & block) break;
	}


	return attacks;
}

//Generate rook attack on the fly
U64 rook_attacks_on_the_fly(int square, U64 block) {
	U64 attacks = 0ULL;

	//ranks & files
	int r, f;
	//target r&f
	int tr = square / 8;
	int tf = square % 8;

	//Slider without the edge
	for (r = tr + 1; r <= 7; r++) {
		attacks |= (1ULL << (r * 8 + tf));
		if ((1ULL << (r * 8 + tf)) & block) break;
	}
	for (r = tr - 1; r >= 0; r--) {
		attacks |= (1ULL << (r * 8 + tf));
		if ((1ULL << (r * 8 + tf)) & block) break;
	}
	for (f = tf + 1; f <= 7; f++) {
		attacks |= (1ULL << (tr * 8 + f));
		if ((1ULL << (tr * 8 + f)) & block) break;
	}
	for (f = tf - 1; f >= 0; f--) {
		attacks |= (1ULL << (tr * 8 + f));
		if ((1ULL << (tr * 8 + f)) & block) break;
	}

	return attacks;
}


//Sets the various possible combination of mask
// TODO if inefficient try moving in the count of the bits
// The magic starts from here
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

				int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

				bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
			}
			else {//rook
				U64 occupancy = set_occupancy(index, relevant_bits, attack_mask);

				int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);

				rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
			}
		}
	}
}

//get bishop attacks
static inline U64 get_bishop_attacks(int square, U64 occupancy) {
	// getting bishop attack given the current occupancy
	occupancy &= bishop_masks[square];
	occupancy *= bishop_magic_numbers[square];
	occupancy >>= 64 - bishop_relevant_bits[square];

	return bishop_attacks[square][occupancy];
}

//get rook attacks
static inline U64 get_rook_attacks(int square, U64 occupancy) {
	// getting bishop attack given the current occupancy
	occupancy &= rook_masks[square];
	occupancy *= rook_magic_numbers[square];
	occupancy >>= 64 - rook_relevant_bits[square];

	return rook_attacks[square][occupancy];
}

//get queen attacks
static inline U64 get_queen_attacks(int square, U64 occupancy) {
	U64 b_occupancy = occupancy;
	U64 r_occupancy = occupancy;

	// getting bishop attack given the current occupancy
	b_occupancy &= bishop_masks[square];
	b_occupancy *= bishop_magic_numbers[square];
	b_occupancy >>= 64 - bishop_relevant_bits[square];

	// getting rook attack given the current occupancy
	r_occupancy &= rook_masks[square];
	r_occupancy *= rook_magic_numbers[square];
	r_occupancy >>= 64 - rook_relevant_bits[square];

	return bishop_attacks[square][b_occupancy] | rook_attacks[square][r_occupancy];
}

// TODO could be optimized
//Checks if square is attacked
static inline bool is_square_attacked(int square, int side)
{
	// attacked by white pawns
	if ((side == white) && (pawn_attacks[black][square] & bitboards[P])) return true;

	// attacked by black pawns
	if ((side == black) && (pawn_attacks[white][square] & bitboards[p])) return true;

	// attacked by knights
	if (knight_attacks[square] & ((side == white) ? bitboards[N] : bitboards[n])) return true;

	// attacked diagonally
	if (get_bishop_attacks(square, occupancies[both]) & ((side == white) ? (bitboards[B] | bitboards[Q]) : (bitboards[b] | bitboards[q]))) return true;

	// attacked orthogonally
	if (get_rook_attacks(square, occupancies[both]) & ((side == white) ? (bitboards[R] | bitboards[Q]) : (bitboards[r] | bitboards[q]))) return true;

	// attacked by kings
	if (king_attacks[square] & ((side == white) ? bitboards[K] : bitboards[k])) return true;

	// by default return false
	return false;
}

void print_attacked_squares(int side)
{
	printf("\n");

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;

			if (!file)
				cout << " " << 8 - rank << " ";

			// check whether current square is attacked or not
			cout << " " << (is_square_attacked(square, side) ? 1 : 0);
		}
		cout << endl;
	}
	cout << "    a b c d e f g h" << endl;;
}


//IMPLEMENTATION OF ALL MOVES

static inline int make_move(int move, int move_flag) {
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

		pop_bit(bitboards[piece], source_square);
		set_bit(bitboards[piece], target_square);


		//handle capture
		if (capture) {

			// Maybe it is faster with a break condition
			for (int bb_piece = pawn; bb_piece < pawn + 6; bb_piece++) {
				pop_bit(bitboards[bb_piece], target_square);
			}
		}

		//handle promotions
		if (promoted) {
			pop_bit(bitboards[pawn], target_square);
			set_bit(bitboards[promoted], target_square);
		}

		//handle enpassant
		if (enpass) {
			(side == white) ? pop_bit(bitboards[p], target_square + 8) : pop_bit(bitboards[P], target_square - 8);
		}

		//Reset enpassant
		enpassant = no_sq;

		//handle double push
		if (double_push) {
			enpassant = ((side == white) ? (target_square + 8) : (target_square - 8));
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
				break;

				// white castles queen side
			case (c1):
				// move A rook
				pop_bit(bitboards[R], a1);
				set_bit(bitboards[R], d1);
				break;

				// black castles king side
			case (g8):
				// move H rook
				pop_bit(bitboards[r], h8);
				set_bit(bitboards[r], f8);
				break;

				// black castles queen side
			case (c8):
				// move A rook
				pop_bit(bitboards[r], a8);
				set_bit(bitboards[r], d8);
				break;
			}
		}

		//update castle rights
		castle &= castling_rights[source_square];
		castle &= castling_rights[target_square]; //This prevents funny bug

		//update occupancies //Maybe it is faster using pop_bit and set_bit everytime
		occupancies[white] = bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K];
		occupancies[black] = bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k];
		occupancies[both] = occupancies[white] | occupancies[black];

		//change side
		side ^= 1;

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
}

// TODO I could make this function longer but with fewer branches for efficency
static inline void generate_moves(vector<int>& moves) {
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
						moves.push_back(encode_move(source_square, target_square - 8, P, 0, 0, 1, 0, 0));
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
						moves.push_back(encode_move(source_square, target_square + 8, p, 0, 0, 1, 0, 0));
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


// leaf nodes (number of positions reached during the test of the move generator at a given depth)
long nodes;

// perft driver
static inline void perft_driver(int depth)
{
	// reccursion escape condition
	if (depth == 0)
	{
		// increment nodes count (count reached positions)
		nodes++;
		return;
	}

	// generate moves
	vector<int> moves;
	generate_moves(moves);

	// loop over generated moves
	for (int i = 0; i < moves.size(); i++)
	{
		// preserve board state
		copyboard();

		// make move
		if (!make_move(moves[i], all_moves))
			// skip to the next move
			continue;

		// call perft driver recursively
		perft_driver(depth - 1);

		// take back
		takeback();
	}
}

// perft test
void perft_test(int depth)
{
	printf("\n     Performance test\n\n");


	// generate moves
	vector<int> moves;
	generate_moves(moves);


	// loop over generated moves
	for (int i = 0; i < moves.size(); i++)
	{
		// preserve board state
		copyboard();

		// make move
		if (!make_move(moves[i], all_moves))
			// skip to the next move
			continue;

		// cummulative nodes
		long cummulative_nodes = nodes;

		// call perft driver recursively
		perft_driver(depth - 1);

		// old nodes
		long old_nodes = nodes - cummulative_nodes;

		// take back
		takeback();

		// print move
		printf("     move: %s%s%c  nodes: %ld\n", square_to_coordinates[get_move_source(moves[i])],
			square_to_coordinates[get_move_target(moves[i])],
			get_move_promoted(moves[i]) ? ascii_pieces[get_move_promoted(moves[i])] : ' ',
			old_nodes);
	}

	// print results
	printf("\n    Depth: %d\n", depth);
	printf("    Nodes: %ld\n", nodes);
}

//USER INTERACTION

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

void init_all() {
	init_char();
	init_leaper_attacks();
	init_slider_attacks(bishop);
	init_slider_attacks(rook);
	init_score();
	//init_bitboards(); LATER Add this for performance when you will need to implement multiple boards
}

int main() {
	U64 bitboard = 0ULL;
	//init
	init_all();
	parse_FEN("rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1 ");

	print_board();

	cout << "Score: " << evaluate() << endl;
	return 0;
}
