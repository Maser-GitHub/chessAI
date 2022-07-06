#pragma once
#include <string>

using namespace std;

class Piece {
public:
	string name;
	//Returns all the moves it can 
	virtual int** legalmoves() = 0;
};