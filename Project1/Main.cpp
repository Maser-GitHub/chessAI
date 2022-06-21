#include<iostream>
#include "Board.cpp"

using namespace std;


int main() {
	Board board;

	//board.addPiece(0b0000000101000000);
	board.initializePieces();
	board.print();
	return 0;
}

