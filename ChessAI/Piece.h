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
};