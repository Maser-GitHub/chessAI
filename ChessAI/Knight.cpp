#include "Knight.h";
#include <vector>

Knight::Knight(bool isWhite) {
	name = isWhite ? "wk" : "bk";
	this->isWhite = isWhite;
}

vector<int> Knight::legalmoves(Piece *cells[8][8], int x, int y) {
	vector<int> moves;
	return moves;
}