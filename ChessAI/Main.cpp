// CHANGED APPROACH USING BITBOARD
// Following methods on the site www.chessprogramming.org
// TODO remove global variables
#include "Init.h"

using namespace std;

int main() {
	U64 bitboard = 0ULL;
	//init
	init_all();
	parse_FEN(start_position);
	char input[10];
	while (1) {
		print_board();
		search_position(8);
		make_move(pv_table[0][0], all_moves);
		print_board();
		cin >> input;
		make_move(parse_move(input), all_moves);

	}
	cout << dec << nodes << endl;
	return 0;
}
