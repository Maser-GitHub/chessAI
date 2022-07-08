#include "Queen.h";
#include <vector>

Queen::Queen(bool isWhite) {
	name = isWhite ? "wQ" : "bQ";
	this->isWhite = isWhite;
}

vector<int> Queen::legalmoves(Piece *cells[8][8], int x, int y) {
	vector<int> moves;
	return moves;
}