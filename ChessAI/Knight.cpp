#include "Knight.h";

Knight::Knight(bool isWhite) {
	name = isWhite ? "wk" : "bk";
}

int** Knight::legalmoves() {
	int* r = new int(0);
	return &r;
}