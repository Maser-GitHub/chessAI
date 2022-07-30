#include "Perft.h"

int make_move(int, int);

inline void perft_driver(int depth)
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