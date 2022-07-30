#include "Attack.h"

//Extern variables
U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_masks[64];
U64 bishop_attacks[64][512];
U64 rook_masks[64];
U64 rook_attacks[64][4096];


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
//Checks if square is attacked
inline bool is_square_attacked(int square, int side)
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

//get bishop attacks
inline U64 get_bishop_attacks(int square, U64 occupancy) {
	// getting bishop attack given the current occupancy
	occupancy &= bishop_masks[square];
	occupancy *= bishop_magic_numbers[square];
	occupancy >>= 64 - (U64)bishop_relevant_bits[square];

	return bishop_attacks[square][occupancy];
}

//get rook attacks
inline U64 get_rook_attacks(int square, U64 occupancy) {
	// getting bishop attack given the current occupancy
	occupancy &= rook_masks[square];
	occupancy *= rook_magic_numbers[square];
	occupancy >>= 64 - rook_relevant_bits[square];

	return rook_attacks[square][occupancy];
}

//get queen attacks
inline U64 get_queen_attacks(int square, U64 occupancy) {
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