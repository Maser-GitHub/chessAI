#include "Bishop.h";
#include <vector>

Bishop::Bishop(bool isWhite) {
	name = isWhite ? "wb" : "bb";
	this->isWhite = isWhite;
}

vector<int> Bishop::legalmoves(Piece* cells[8][8], int x, int y) {
	return diagonalmoves(cells, x, y);
}