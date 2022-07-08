#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
	//Constructor
	Queen(bool isWhite);
	virtual vector<int> legalmoves(Piece *cells[8][8], int x, int y);
};