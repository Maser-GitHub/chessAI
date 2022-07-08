#include "Pawn.h";
#include <vector>

Pawn::Pawn(bool isWhite) {
	name = isWhite ? "wp" : "bp";
	this->isWhite = isWhite;
}

vector<int> Pawn::legalmoves(Piece *cells[8][8], int x, int y) {
	vector<int> moves;
	return moves;
}