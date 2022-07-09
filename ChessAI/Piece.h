#pragma once
#include <string>
#include <vector>

using namespace std;

class Piece {
public:
	string name;
	char posx, posy;
	bool isWhite;
	//Returns all the moves it can 
	virtual vector<int> legalmoves(Piece *cells[8][8], int x, int y) = 0;
	//Returns horizontal and vertical moves of Rook and Queen
	vector<int> horizontalmoves(Piece* cells[8][8], int x, int y);
	//Returns diagonal moves of Bishop and Queen
	vector<int> diagonalmoves(Piece* cells[8][8], int x, int y);
	//Cheks and adds eventual move
	bool checkAdd(Piece* cells[8][8], vector<int>& moves, int x, int y);
};