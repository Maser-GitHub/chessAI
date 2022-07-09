#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
	//Constructor
	Queen(bool isWhite);
	virtual vector<int> legalmoves(Piece*** cells, int x, int y);
};