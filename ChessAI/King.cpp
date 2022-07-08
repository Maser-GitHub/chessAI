#include "King.h";
#include <vector>

King::King(bool isWhite) {
	name = isWhite ? "wK" : "bK";
	this->isWhite = isWhite;
}

vector<int> King::legalmoves(Piece *cells[8][8], int x, int y) {
	vector<int> moves;
	return moves;
}