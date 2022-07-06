#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
	//Constructor
	Bishop(bool isWhite);
	virtual int** legalmoves();
};