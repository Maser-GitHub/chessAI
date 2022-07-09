#include "Rook.h";
#include <vector>

Rook::Rook(bool isWhite) {
	name = isWhite ? "wR" : "bR";
	this->isWhite = isWhite;
}

vector<int> Rook::legalmoves(Piece*** cells, int x, int y) {
	return orthogonalmoves(cells, x, y);
}