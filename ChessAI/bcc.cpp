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

const char* square_to_coordinate[] = {
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



int main() {
	U64 bitboard = 0ULL;
	//init
	init_leaper_attacks();

	for (int rank = 0; rank < 8; rank++) {
		for (int file = 0; file < 8; file++) {
			int square = rank + file * 8;

			cout << " " << count_bits(mask_rook_attacks(square)) << ",";
		}
		cout << endl;
	}

	return 0;
}