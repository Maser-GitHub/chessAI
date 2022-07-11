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

enum enumColor {
	white, black
};
//set get pop macros

#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (bitboard &= (~(1ULL << square)))

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


//Pawn attacks table [side][square]

U64 pawn_attack[2][64];

//generate mask pawn 
U64 mask_pawn_attack(int side, int square) {
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
void init_leaper_attacks() {
	for (int square = 0; square < 64; square++) {
		pawn_attack[white][square] = mask_pawn_attack(white, square);
		pawn_attack[black][square] = mask_pawn_attack(black, square);
	}
}











int main() {
	U64 bitboard = 0ULL;
	//init
	init_leaper_attacks();

	return 0;
}