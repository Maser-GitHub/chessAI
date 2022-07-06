#include <vector>
#include <map>
#include <iostream>
#include<cstddef>
#include "Piece.h"


using namespace std;

class Board {
private:
	Piece* cells[8][8];
public:
	//Creates board
	Board();
	//Add pieces in the traditional starting position
	void initialize();
	//adds the piece to the board
	void addPiece(Piece* piece, char x, char y);
	//Prints the board on the terminal
	//It is not optimized, because this will be called only by users
	void print();
	/*
	void move(char from, char to);
	//User friendly move
	void move(int fromx, int fromy, int tox, int toy);*/
};