#include "Hash.h"


U64 piece_keys[12][64];
U64 enpassant_keys[64];
U64 castle_keys[16];
U64 side_key;
unordered_map<U64,tuple<U64, int, int, int>> hash_table;

int random_state = 1804289383;

U64 generate_hash_key() {
	//return value
	U64 final_key = 0ULL;

	//loop over the pieces
	U64 bitboard;
	for (int piece = P; piece <= k; piece++) {
		bitboard = bitboards[piece];
		while (bitboard) {
			int square = get_ls1b_index(bitboard);

			final_key ^= piece_keys[piece][square];

			pop_bit(bitboard, square);
		}
	}

	if (enpassant != no_sq) {
		final_key ^= enpassant_keys[enpassant];
	}

	final_key ^= castle_keys[castle];

	if (side == black) final_key ^= side_key;

	return final_key;
}

inline int read_hash_entry(int alpha, int beta, int depth) {
	//treats the std::unordered_map as a hash map
	tuple<U64, int, int, int> hash_entry = hash_table[hash_key % hash_table.max_size()];
	int hash_flag = get<2>(hash_entry);
	int hash_score = get<3>(hash_entry);
	if (get<0>(hash_entry) == hash_key) {
		if (get<1>(hash_entry) >= depth) {

			if (hash_score < -MATE_SCORE) hash_score += ply;
			if (hash_score > MATE_SCORE) hash_score -= ply;

			if (hash_flag == hash_flag_exact) return hash_score;
			if ((hash_flag == hash_flag_alpha) && (hash_score <= alpha)) return alpha;
			if ((hash_flag == hash_flag_beta) && (hash_score >= beta)) return beta;
		}
	}
	return INF + 1; //no hash entry
}

// int depth, int hash_flag, int score
inline void write_hash_entry(int depth, int hash_flag, int score) {
	if (score < -MATE_SCORE) score -= ply;
	if (score > MATE_SCORE) score += ply;
	hash_table[hash_key % hash_table.max_size()] = make_tuple(hash_key, depth, hash_flag, score);
}

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number()
{
	// get current state
	unsigned int number = random_state;

	// XOR shift algorithm
	number ^= number << 13;
	number ^= number >> 17;
	number ^= number << 5;

	// update random number state
	random_state = number;

	// return random number
	return number;
}

// generate 64-bit pseudo legal numbers
U64 get_random_U64_number()
{
	// define 4 random numbers
	U64 n1, n2, n3, n4;

	// init random numbers slicing 16 bits from MS1B side
	n1 = (U64)(get_random_U32_number()) & 0xFFFF;
	n2 = (U64)(get_random_U32_number()) & 0xFFFF;
	n3 = (U64)(get_random_U32_number()) & 0xFFFF;
	n4 = (U64)(get_random_U32_number()) & 0xFFFF;

	// return random number
	return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

void initialize_values(const char* fen) {
	//init hash key
	parse_FEN(fen);
	hash_key = generate_hash_key();
}