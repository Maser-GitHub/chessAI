#include "Piece.h"


bool Piece::checkAdd(Piece*** cells, vector<int>& moves, int x, int y) {
	if (cells[x][y] == nullptr) {
		moves.push_back(x);
		moves.push_back(y);
	}
	else {
		if (this->isWhite != cells[x][y]->isWhite) { //Add as eating
			moves.push_back(x);
			moves.push_back(y);
		}
		return true;
	}
	return false;
}


vector<int> Piece::orthogonalmoves(Piece*** cells, int x, int y) {
	vector<int> moves;
	int i;
	bool isOver=false;
	for (i = 1; x + i <= 7 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x + i, y);
	}
	isOver = false;
	for (i = 1; y + i <= 7 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x, y + i);
	}
	isOver = false;
	for (i = 1; x - i >= 0 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x - i, y);
	}
	isOver = false;
	for (i = 1; y - i >= 0 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x, y - i);
	}
	return moves;
}

vector<int> Piece::diagonalmoves(Piece*** cells, int x, int y) {
	vector<int> moves;
	int i;
	bool isOver = false;
	for (i = 1; x + i <= 7 && y + i <= 7 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x + i, y + i);
	}
	isOver = false;
	for (i = 1; x + i <= 7 && y - i >= 0 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x + i, y - i);
	}
	isOver = false;
	for (i = 1; x - i >= 0 && y + i <= 7 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x - i, y + i);
	}
	isOver = false;
	for (i = 1; y - i >= 0 && x - i >= 0 && !isOver; i++) {
		isOver = checkAdd(cells, moves, x - i, y - i);
	}
	return moves;
}