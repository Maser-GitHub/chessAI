#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
	//Constructor
	Rook(bool isWhite);
	virtual int** legalmoves();
};