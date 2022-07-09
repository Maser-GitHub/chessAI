#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
	//Constructor
	Rook(bool isWhite);
	virtual vector<int> legalmoves(Piece*** cells, int x, int y);
};