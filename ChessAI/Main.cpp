#include <iostream>
#include <cstddef>
#include <sstream>
#include <vector>
#include "Board.h"
#include "Util.h"



using namespace std;

int main() {
	Board board;
	string input;
	vector<int> moves;
	board.initialize();
	//board.print();
	//board.move(6, 4, 4, 4);
	//board.move(1, 4, 3, 4);
	board.move(0, 3, 4, 4);
	board.print();
	moves = board.possiblemoves(4, 4);
	//moves.push_back(10);
	for (int i = 0; i < moves.size(); i+=2) {
		cout << moves[i] << " " << moves[i+1] << endl;
	}
	return 0;
}