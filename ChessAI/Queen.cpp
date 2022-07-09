#include "Queen.h";
#include <vector>

Queen::Queen(bool isWhite) {
	name = isWhite ? "wQ" : "bQ";
	this->isWhite = isWhite;
}

vector<int> Queen::legalmoves(Piece *cells[8][8], int x, int y) {
	vector<int> moves, movesD;
	moves = horizontalmoves(cells, x, y);
	movesD = diagonalmoves(cells, x, y);
	moves.insert(moves.end(), movesD.begin(), movesD.end());
	return moves;
}