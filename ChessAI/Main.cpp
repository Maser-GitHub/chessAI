#include <iostream>
#include <cstddef>
#include <sstream>
#include <vector>
#include "Board.h"
#include "Util.h"


void moves(bool, vector<Board>&, vector<Board>&);

using namespace std;

int ex_main() {
	Board *board = new Board();
	vector<Board> boards1, boards2, boards3, boards4, boards5;
	string input;
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
	moves(whiteTurn, boards1, boards2);
	whiteTurn = !whiteTurn;
	moves(whiteTurn, boards2, boards3);
	whiteTurn = !whiteTurn;
	moves(whiteTurn, boards3, boards4);
	whiteTurn = !whiteTurn;
	
	/*for (i = 0; i < boards3.size(); i++) {
		for (j = i + 1; j < boards3.size(); j++) {
			if (boards3[i].equals(boards3[j])) count++;
		}
	}*/
	/*for (i = 0; i < boards3.size(); i++) {
		boards3[i].print();
	}*/
	// TODO Implements cheks
	cout << "The number of moves initially are: " << boards4.size() << endl;
	return 0;
}

void moves(bool whiteTurn, vector<Board>& in, vector<Board>& out) {
	vector<Board> boardstmp;
	for (int i = 0; i < in.size(); i++) {
		boardstmp = in[i].allPossibleBoards(whiteTurn);
		out.insert(out.end(), boardstmp.begin(), boardstmp.end());
	}
}