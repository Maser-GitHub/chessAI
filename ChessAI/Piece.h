#pragma once
#include <string>
#include <vector>

using namespace std;

class Piece {
public:
	string name;
	bool isWhite;
	//Returns all the moves it can 
	virtual vector<int> legalmoves(Piece ***, int x, int y) = 0;
	//Returns horizontal and vertical moves of Rook and Queen
	vector<int> orthogonalmoves(Piece***, int x, int y);
	//Returns diagonal moves of Bishop and Queen
	vector<int> diagonalmoves(Piece***, int x, int y);
	//Cheks and adds eventual move
	bool checkAdd(Piece***, vector<int>& moves, int x, int y);
};