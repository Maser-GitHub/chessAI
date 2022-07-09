#include "Knight.h";
#include <vector>

Knight::Knight(bool isWhite) {
	name = isWhite ? "wN" : "bN";
	this->isWhite = isWhite;
}

vector<int> Knight::legalmoves(Piece*** cells, int x, int y) {
	vector<int> moves;
	if (x > 1 && y < 7) checkAdd(cells, moves, x - 2, y + 1);
	if (x > 1 && y > 0) checkAdd(cells, moves, x - 2, y - 1);
	if (x > 0 && y < 6) checkAdd(cells, moves, x - 1, y + 2);
	if (x > 0 && y > 1) checkAdd(cells, moves, x - 1, y - 2);
	if (x < 7 && y < 6) checkAdd(cells, moves, x + 1, y + 2);
	if (x < 7 && y > 1) checkAdd(cells, moves, x + 1, y - 2);
	if (x < 6 && y < 7) checkAdd(cells, moves, x + 2, y + 1);
	if (x < 6 && y > 0) checkAdd(cells, moves, x + 2, y - 1);
	return moves;
}