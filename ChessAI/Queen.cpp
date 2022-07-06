#include "Queen.h";

Queen::Queen(bool isWhite) {
	name = isWhite ? "wQ" : "bQ";
}

int** Queen::legalmoves() {
	int* r = new int(0);
	return &r;
}