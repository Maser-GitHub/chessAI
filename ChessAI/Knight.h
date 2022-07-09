#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
	//Constructor
	Knight(bool isWhite);
	virtual vector<int> legalmoves(Piece*** cells, int x, int y);
};