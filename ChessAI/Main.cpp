#include <iostream>
#include <cstddef>
#include <sstream>
#include "Board.h"
#include "Util.h"



using namespace std;


int main() {
	Board board;
	string input;

	board.initialize();
	board.print();
	//board.move(6, 4, 5, 3);
	//board.move(1, 4, 3, 4);
	//board.print();
	return 0;
}