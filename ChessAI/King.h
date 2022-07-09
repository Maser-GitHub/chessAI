#pragma once
#include "Piece.h"

class King : public Piece {
public:
	//Constructor
	King(bool isWhite);
	virtual vector<int> legalmoves(Piece*** cells, int x, int y);
};
