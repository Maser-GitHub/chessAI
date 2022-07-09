#include "King.h";
#include <vector>

King::King(bool isWhite) {
	name = isWhite ? "wK" : "bK";
	this->isWhite = isWhite;
}

// TODO actually checking if it is legal or not
// TODO add castling
vector<int> King::legalmoves(Piece*** cells, int x, int y) {
	vector<int> moves;
	if (x > 0 && y > 0) checkAdd(cells, moves, x - 1, y - 1);
	if (x > 0 && y < 7) checkAdd(cells, moves, x - 1, y + 1);
	if (x < 7 && y > 0) checkAdd(cells, moves, x + 1, y - 1);
	if (x < 7 && y < 7) checkAdd(cells, moves, x + 1, y + 1);
	if (x > 0) checkAdd(cells, moves, x - 1, y);
	if (y > 0) checkAdd(cells, moves, x, y - 1);
	if (y < 7) checkAdd(cells, moves, x, y + 1);
	if (x < 7) checkAdd(cells, moves, x + 1, y);
	return moves;
}