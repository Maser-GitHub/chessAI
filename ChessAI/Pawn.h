#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
	//Constructor
	Pawn(bool isWhite);
	virtual vector<int> legalmoves(Piece*** cells, int x, int y);
};