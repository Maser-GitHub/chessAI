#pragma once
#include "Hash.h"
//Pawn attacks table [side][square]

extern U64 pawn_attacks[2][64];

//Knight attacks table[square]

extern U64 knight_attacks[64];

//King attacks table[square]

extern U64 king_attacks[64];

//Bishop attack masks table[square]
extern U64 bishop_masks[64];

// Bishop attacks table[square][occupancies]
extern U64 bishop_attacks[64][512];

//Rook attacks mask[square]

extern U64 rook_masks[64];

//Rook attacks table[square][occupancies]
extern U64 rook_attacks[64][4096];

//Those magic numbers are needed in order to filter the rook attack given the current occupancy of the board

// rook magic numbers
extern U64 rook_magic_numbers[64];
// bishop magic numbers
extern U64 bishop_magic_numbers[64];

inline U64 get_bishop_attacks(int square, U64 occupancy);
inline U64 get_rook_attacks(int square, U64 occupancy);
extern inline U64 get_queen_attacks(int square, U64 occupancy);

//generate mask pawn attack
U64 mask_pawn_attacks(int side, int square);

//Generate mask knight attack

U64 mask_knight_attacks(int square);

//Generate mask king attack

U64 mask_king_attacks(int square);
//Generate mask bishop attack
U64 mask_bishop_attacks(int square);

//Generate mask rook attack
U64 mask_rook_attacks(int square);
//Generate bishop attack on the fly
U64 bishop_attacks_on_the_fly(int square, U64 block);

//Generate rook attack on the fly
U64 rook_attacks_on_the_fly(int square, U64 block);
//Sets the various possible combination of mask
// TODO if inefficient try moving in the count of the bits
// The magic starts from here
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

inline bool is_square_attacked(int square, int side);
