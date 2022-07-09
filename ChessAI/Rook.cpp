#include "Rook.h";
#include <vector>

Rook::Rook(bool isWhite) {
	name = isWhite ? "wr" : "br";
	this->isWhite = isWhite;
}

vector<int> Rook::legalmoves(Piece *cells[8][8], int x, int y) {
	return horizontalmoves(cells, x, y);
}