#pragma once
#include "Piece.h"

class King : public Piece {
public:
	//Constructor
	King(bool isWhite);
	virtual int** legalmoves();
};
