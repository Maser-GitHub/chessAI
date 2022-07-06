#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
	//Constructor
	Pawn(bool isWhite);
	virtual int** legalmoves();
};