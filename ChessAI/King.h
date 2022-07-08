#pragma once
#include "Piece.h"

class King : public Piece {
public:
	//Constructor
	King(bool isWhite);
	virtual vector<int> legalmoves(Piece *cells[8][8], int x, int y);
};
