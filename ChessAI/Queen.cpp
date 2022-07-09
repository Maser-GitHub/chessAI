#include "Queen.h";
#include <vector>

Queen::Queen(bool isWhite) {
	name = isWhite ? "wQ" : "bQ";
	this->isWhite = isWhite;
}

vector<int> Queen::legalmoves(Piece*** cells, int x, int y) {
	vector<int> moves, movesD;
	moves = orthogonalmoves(cells, x, y);
	movesD = diagonalmoves(cells, x, y);
	moves.insert(moves.end(), movesD.begin(), movesD.end());
	return moves;
}