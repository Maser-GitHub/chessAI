// CHANGED APPROACH USING BITBOARD
// Following methods on the site www.chessprogramming.org

#include <iostream>

using namespace std;

#define U64 unsigned long long	

// enums 

enum enumSquare {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1
};

const char *square_to_coordinate[] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};
 
enum enumColor {
	white, black
};


// BIT MANIPULATION

//set get pop macros

#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (bitboard &= (~(1ULL << square)))
#define count_bits(bitboard) __builtin_popcountll(bitboard)


//Convert the file in .c and use __builtin_popcountll(bitboard) if bottleneck
//Counts bits in a bitboard
static inline int count_bits(U64 bitboard) {
	int count = 0;

	while (bitboard) {
		count++;
		// Magic bit manipulation that removes the LSB
		bitboard &= bitboard - 1;
	}

	return count;
}

// get LS1B index

static inline int get_lsb_index(U64 bitboard) {
	//check bitboard != 0
	if (!bitboard) return -1;
	//isolate lsb, then subract 1 counting the ones created
	return count_bits((bitboard & (0-bitboard)) - 1);
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



// ATTACKS 

// Mask all that is not a-file
const U64 not_a_file = 0xfefefefefefefefeULL;
// Mask all that is not h-file
const U64 not_h_file = 0x7f7f7f7f7f7f7f7fULL;
// Mask all that is not a-file or b-file
const U64 not_ab_file = 0xfcfcfcfcfcfcfcfcULL;
// Mask all that is not h-file or h-file
const U64 not_hg_file = 0x3f3f3f3f3f3f3f3fULL;




//Pawn attacks table [side][square]

U64 pawn_attacks[2][64];

//Knight attacks table[square]

U64 knight_attacks[64];

//King attacks table[square]

U64 king_attacks[64];

//Bishop attacks table[square]

U64 bishop_attacks[64];

//Rook attacks table[square]

U64 rook_attacks[64];


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
	for (int square = 0; square < 64; square++) {
		bishop_attacks[square] = mask_bishop_attacks(square);
	}
	for (int square = 0; square < 64; square++) {
		rook_attacks[square] = mask_rook_attacks(square);
	}
}







int main() {
	U64 bitboard = 0ULL;
	//init
	init_leaper_attacks();

	//init occupance
	U64 block = 0ULL;
	set_bit(block, d7);
	set_bit(block, d1);
	set_bit(block, h8);
	set_bit(block, g4);

	print_bitboard(block);
	cout << square_to_coordinate[get_lsb_index(block)] << endl;

	return 0;
}