#include <iostream>
#include <cstddef>
#include <sstream>
#include "Board.h"
#include "Util.h"

using namespace std;


int main() {
	Board board;
	string input;
	int from;
	int to;

	board.initializeFast();
	board.print();
	/*from = Util::ReadNumber("Select starting position[xxx|yyy]: ");
	to = Util::ReadNumber("Select destination position[xxx|yyy]: ");
	cout << "from: " << from << " to: " << to << endl;*/
	board.move(6,4,5,4);
	board.print();
	return 0;
}