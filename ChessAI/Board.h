#include <vector>
#include <map>
#include <iostream>
#include<cstddef>


using namespace std;

class Board {
private:
	//Pieces are 1 unusued bit, 1 bit if alive, 6 bit for position
	vector<char> pieces;
	vector<string> types{ "Br","Bk","Bb","BQ","BK","Bb","Bk","Br","Bp","Bp","Bp","Bp","Bp","Bp","Bp","Bp",
						"Wp","Wp","Wp","Wp","Wp","Wp","Wp","Wp","Wr","Wk","Wb","WQ","WK","Wb","Wk","Wr" };
public:
	//Creates board
	Board();
	//Add pieces in the traditional starting position
	void initialize();
	//Add pieces faster
	void initializeFast();
	//encode the piece and then it adds it to the pieces vector
	void addPiece(char id, char x, char y);
	//Prints the board on the terminal
	//It is not optimized, because this will be called only by users
	void print();
	//Returns a copy of the vector types
	vector<string> getTypes();
	//Returns a copy of the vector pieces
	vector<char> getPieces();
	void move(char from, char to);
	//User friendly move
	void move(int fromx, int fromy, int tox, int toy);
};