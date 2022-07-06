#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
	//Constructor
	Knight(bool isWhite);
	virtual int** legalmoves();
};