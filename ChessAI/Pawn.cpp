#include "Pawn.h";

Pawn::Pawn(bool isWhite) {
	name = isWhite ? "wp" : "bp";
}

int** Pawn::legalmoves() {
	int* r = new int(0);
	return &r;
}