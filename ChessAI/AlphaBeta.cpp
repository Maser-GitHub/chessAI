#include "AlphaBeta.h"

//Extern variables
int evaluate();
int make_move(int,int);
void sort_moves(vector<int>&);

long nodes;
const int full_depth_moves = 4;
const int reduction_limit = 3;

inline void enable_pv_scoring(vector<int>& moves) {
	follow_pv = false;

	for (int i = 0; i < moves.size(); i++) {
		if (pv_table[0][ply] == moves[i]) {
			score_pv = true;
			follow_pv = true;
		}
	}
}


//repetition detection
inline bool is_repetition() {
	for (int i = 0; i < repetition_index; i++) {
		if (hash_key == repetition_table[i]) {
			return true;
		}
	}

	return false;
}



inline int quiescence(int alpha, int beta) {

	nodes++;

	//We are overflowing the arrays
	if (ply >= PLY - 2) return evaluate();

	//evaluation
	int eval = evaluate();

	//found better move (PV node move)
	if (eval > alpha) {
		if (eval >= beta) return beta;
		alpha = eval;
	}
	//create moves:
	vector<int> moves;

	generate_moves(moves);

	sort_moves(moves);

	for (int i = 0; i < moves.size(); i++) {
		//preserve boards
		copyboard();
		ply++;

		repetition_index++;
		repetition_table[repetition_index] = hash_key;


		//legal moves
		if (make_move(moves[i], only_capture) == 0) {
			ply--;

			repetition_index--;
			continue;
		}

		//score move
		int score = -quiescence(-beta, -alpha);

		//if it was legal
		ply--;
		repetition_index--;
		takeback();

		//fail-hard cutoff (fail high)
		if (score > alpha) {
			alpha = score;
			if (score >= beta) return beta;
		}
	}
	return alpha;
}

inline int negamax(int alpha, int beta, int depth) {

	int score;

	int hash_flag = hash_flag_alpha;

	if (is_repetition()) return 0;

	bool pv_node = (beta - alpha) > 1;

	//read hash entry
	if (ply && (score = read_hash_entry(alpha, beta, depth)) <= INF && !pv_node) {
		return score;
	}

	pv_length[ply] = ply;

	if (depth == 0) return quiescence(alpha, beta);

	//We are overflowing the arrays
	if (ply >= PLY - 2) return evaluate();
	//increments nodes
	nodes++;

	bool in_check = is_square_attacked((side == white) ? (get_ls1b_index(bitboards[K])) : (get_ls1b_index(bitboards[k])), side ^ 1);
	if (in_check) depth++;
	int legal_moves = 0;

	//null move pruning
	if (depth >= 3 && in_check == 0 && ply) {
		copyboard();
		ply++;

		repetition_index++;
		repetition_table[repetition_index] = hash_key;


		if (enpassant != no_sq) hash_key ^= enpassant_keys[enpassant];

		//reset enpassant
		enpassant = no_sq;

		//extra move to opponenet
		side ^= 1;

		hash_key ^= side_key;
		score = -negamax(-beta, -beta + 1, depth - 1 - 2);

		ply--;
		repetition_index--;
		takeback();

		if (score >= beta) return beta;
	}

	//create moves:
	vector<int> moves;

	generate_moves(moves);

	if (follow_pv) enable_pv_scoring(moves);

	sort_moves(moves);

	int moves_searched = 0;

	for (int i = 0; i < moves.size(); i++) {
		//preserve boards
		copyboard();
		ply++;

		repetition_index++;
		repetition_table[repetition_index] = hash_key;

		//legal moves
		if (make_move(moves[i], all_moves) == 0) {
			ply--;
			repetition_index--;
			continue;
		}

		legal_moves++;
		//score move

		score;

		// full depth search
		if (moves_searched == 0) {
			// do normal alpha beta search
			score = -negamax(-beta, -alpha, depth - 1);
		}
		else {
			// condition to consider LMR
			if (
				moves_searched >= full_depth_moves &&
				depth >= reduction_limit &&
				in_check == 0 &&
				get_move_capture(moves[i]) == 0 &&
				get_move_promoted(moves[i]) == 0
				) {
				// search current move with reduced depth:
				score = -negamax(-alpha - 1, -alpha, depth - 2);
			}
			else {
				score = alpha + 1;
			};
			// PVS
			if (score > alpha)
			{
				// re-search at full depth but with narrowed score bandwith
				score = -negamax(-alpha - 1, -alpha, depth - 1);

				// if LMR fails re-search at full depth and full score bandwith
				if ((score > alpha) && (score < beta))
					score = -negamax(-beta, -alpha, depth - 1);
			}
		}

		//if it was legal
		ply--;
		repetition_index--;
		takeback();
		moves_searched++;

		//found better move (PV node move)
		if (score > alpha) {
			hash_flag = hash_flag_exact;

			if (get_move_capture(moves[i]) == 0) {
				history_moves[get_move_piece(moves[i])][get_move_target(moves[i])] += depth;
			}
			alpha = score;
			pv_table[ply][ply] = moves[i];
			//copy next ply
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++)
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			//adjust pv length
			pv_length[ply] = pv_length[ply + 1];

			//fail-hard cutoff (fail high)
			if (score >= beta) {
				//store hash entry
				write_hash_entry(depth, hash_flag_beta, score);

				if (get_move_capture(moves[i]) == 0) {
					killer_moves[1][ply] = killer_moves[0][ply];
					killer_moves[0][ply] = moves[i];
				}
				return beta;
			}
		}
	}
	if (legal_moves == 0) {
		//king in check //+ply to find "closest mate"
		if (in_check) return -MATE + ply;
		return 0;

	}
	//store hash with fail low
	write_hash_entry(depth, hash_flag, alpha);
	//nodes fails low
	return alpha;
}


void search_position(int depth) {

	int score = 0;
	nodes = 0;
	follow_pv = false;
	score_pv = false;

	memset(killer_moves, 0, sizeof(killer_moves));
	memset(history_moves, 0, sizeof(history_moves));
	memset(pv_table, 0, sizeof(pv_table));
	memset(pv_length, 0, sizeof(pv_length));

	int alpha = -INF;
	int beta = INF;

	for (int d = 1; d <= depth; d++) {
		nodes = 0;
		follow_pv = true;

		score = negamax(alpha, beta, d);


		if ((score <= alpha) || (score >= beta)) {
			alpha = -INF;
			beta = INF;
			continue;
		}
		// set up the window for the next iteration
		alpha = score - 50;
		beta = score + 50;

		//return best move
		cout << "Best moves: ";
		for (int i = 0; i < pv_length[0]; i++) {
			print_move(pv_table[0][i]);
		}
		cout << endl;
	}
}