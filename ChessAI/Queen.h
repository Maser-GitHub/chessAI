#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
	//Constructor
	Queen(bool isWhite);
	virtual int** legalmoves();
};