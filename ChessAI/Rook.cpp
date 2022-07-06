#include "Rook.h";

Rook::Rook(bool isWhite) {
	name = isWhite ? "wr" : "br";
}

int** Rook::legalmoves() {
	int* r = new int(0);
	return &r;
}