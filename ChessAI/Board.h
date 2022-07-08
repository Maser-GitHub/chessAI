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
	void addPiece(Piece* piece, int x, int y);
	//Prints the board on the terminal
	void print();
	//Move a piece, can ovveride pieces
	void move(int fromx, int fromy, int tox, int toy);
	vector<int> possiblemoves(int x, int y);
};