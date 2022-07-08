#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
	//Constructor
	Bishop(bool isWhite);
	virtual vector<int> legalmoves(Piece *cells[8][8], int x, int y);
};