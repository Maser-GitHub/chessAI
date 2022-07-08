#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
	//Constructor
	Pawn(bool isWhite);
	virtual vector<int> legalmoves(Piece *cells[8][8], int x, int y);
};