#include <vector>
#include <map>
#include <iostream>
#include<cstddef>
#include "Piece.h"


using namespace std;

class Board {
private:
	//There exist only 32 pointers to pieces, shared among all the boards
	Piece*** cells;
	// from right to left, the first 16 bits represent if the pawn can receive EnPassant and then CastleRight,CastleLeft both for white and black 
	int canCastleAndEnPassant=0;
public:
	//Creates board
	Board();
	//Create board with set cells
	Board(Piece*** cells);
	//Add pieces in the traditional starting position
	void initialize();
	//adds the piece to the board
	void addPiece(Piece* piece, int x, int y);
	//Prints the board on the terminal
	void print();
	//Move a piece, can ovveride pieces
	void move(int fromx, int fromy, int tox, int toy);
	//Returns in a vector the possible moves from the cell xy.
	//The vector contains the x-axis and the y-axis
	vector<int> possibleMoves(bool whiteTurn, int x, int y);
	//Returns a vector with all the possible board state
	vector<Board> allPossibleBoards(bool whiteTurn);
	//Returns true if the boards are equal
	bool equals(Board board);
	//Checks if the cell is attacked
	bool isAttacked(bool isWhite, int x, int y);
	//Checks if the board is in a legal state for the current color (the king is not in check)
	bool isLegalState(bool whiteTurn);
};