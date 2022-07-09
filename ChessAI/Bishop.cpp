#include "Bishop.h";
#include <vector>

Bishop::Bishop(bool isWhite) {
	name = isWhite ? "wB" : "bB";
	this->isWhite = isWhite;
}

vector<int> Bishop::legalmoves(Piece*** cells, int x, int y) {
	return diagonalmoves(cells, x, y);
}