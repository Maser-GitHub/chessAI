#include <iostream>
#include <cstddef>
#include <sstream>
#include <vector>
#include "Board.h"
#include "Util.h"



using namespace std;

int main() {
	Board *board = new Board();
	vector<Board> boards1, boards2, boards3, boards4, boards5, boardstmp;
	string input;
	vector<int> moves;
	int count = 0,i,j;
	bool whiteTurn = true;
	board->initialize();
	//board.print();
	//board.move(6, 4, 4, 4);
	//board.move(1, 4, 3, 4);
	board->print();
	cout << "After the first move" << endl;
	boards1 = board->allPossibleBoards(whiteTurn);
	whiteTurn = !whiteTurn;
	for (i = 0; i < boards1.size(); i++) {
		boardstmp = boards1[i].allPossibleBoards(whiteTurn);
		boards2.insert(boards2.end(), boardstmp.begin(), boardstmp.end());
	}
	whiteTurn = !whiteTurn;
	for (i = 0; i < boards2.size(); i++) {
		boardstmp = boards2[i].allPossibleBoards(whiteTurn);
		boards3.insert(boards3.end(), boardstmp.begin(), boardstmp.end());
	}
	whiteTurn = !whiteTurn;
	for (i = 0; i < boards3.size(); i++) {
		boardstmp = boards3[i].allPossibleBoards(whiteTurn);
		boards4.insert(boards4.end(), boardstmp.begin(), boardstmp.end());
	}
	whiteTurn = !whiteTurn;
	for (i = 0; i < boards4.size(); i++) {
		boardstmp = boards4[i].allPossibleBoards(whiteTurn);
		boards5.insert(boards5.end(), boardstmp.begin(), boardstmp.end());
	}
	/*for (i = 0; i < boards3.size(); i++) {
		for (j = i + 1; j < boards3.size(); j++) {
			if (boards3[i].equals(boards3[j])) count++;
		}
	}*/
	/*for (i = 0; i < boards3.size(); i++) {
		boards3[i].print();
	}*/
	// TODO Implements cheks
	cout << "The number of moves initially are: " << boards5.size() << endl;
	return 0;
}