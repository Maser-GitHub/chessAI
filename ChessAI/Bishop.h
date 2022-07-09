#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
	//Constructor
	Bishop(bool isWhite);
	virtual vector<int> legalmoves(Piece*** cells, int x, int y);
};