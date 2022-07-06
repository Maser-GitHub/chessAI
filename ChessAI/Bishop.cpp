#include "Bishop.h";

Bishop::Bishop(bool isWhite) {
	name = isWhite ? "wb" : "bb";
}

int** Bishop::legalmoves() {
	int* r = new int(0);
	return &r;
}