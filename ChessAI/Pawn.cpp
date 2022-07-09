#include "Pawn.h";
#include <vector>

Pawn::Pawn(bool isWhite) {
	name = isWhite ? "wP" : "bP";
	this->isWhite = isWhite;
}

//TODO add Enpassant
vector<int> Pawn::legalmoves(Piece*** cells, int x, int y) {
	vector<int> moves;
	//direction of movement
	int d = isWhite ? -1 : 1;
	//Don't checks for the border because a pawn can never be in row 0 or 7 (It would be promoted)
	if (cells[x + d][y] == nullptr) {
		moves.push_back(x + d);
		moves.push_back(y);
	}
	if ((x == 1 && !isWhite) || (x == 6 && isWhite)) {
		if (cells[x + 2 * d][y] == nullptr && cells[x + d][y] == nullptr) {
			moves.push_back(x + 2 * d);
			moves.push_back(y);
		}
	}
	for (int i = -1; i <= 1; i += 2) {
		if (y + i <= 7 && y + i >= 0 && cells[x + d][y + i] != nullptr && this->isWhite != cells[x + d][y + i]->isWhite) { //Add as eating
			moves.push_back(x + d);
			moves.push_back(y + i);
		}
	}
	return moves;
}