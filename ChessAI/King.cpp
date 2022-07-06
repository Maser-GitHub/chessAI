#include "King.h";

King::King(bool isWhite) {
	name = isWhite ? "wK" : "bK";
}

int** King::legalmoves() {
	int* r = new int(0);
	return &r;
}