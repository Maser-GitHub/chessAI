#include "Board.h"

using namespace std;

	Board::Board() {
		initializeFast();
	}


	void Board::initialize() {
		for (char i=0; i < 2; i++)
			for (char j=0; j < 8; j++)
				addPiece(j + i * 8, i, j);
		for (char i = 6; i < 8; i++)
			for (char j = 0; j < 8; j++)
				addPiece(j + (i-4) * 8, i, j);
	}

	void Board::initializeFast() {
		pieces = {64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,112,113,
				114,115,116,117,118,119,120,121,122,123,124,125,126,127};
	}

	void Board::addPiece(char id, char x, char y) {
		if (id< 0 || id >= 32 || x<0 || x>7 || y<0 || y>7) throw; //TODO maybe remove this for future efficency
		char piece = 0b01000000;
		piece += (x << 3);
		piece += y;
		pieces[id] = piece;
	}

	void Board::print() {
		for (char i = 0; i < 8; i++) {
			for (char j = 0; j < 8; j++) {
				string write = "[]";
				for (char k = 0; k < 32; k++) {
					if ((pieces[k] & 0b00000111) == j)
						if ((pieces[k] & 0b00111000) == i << 3)
							if (pieces[k] & 0b01000000) {
								write = types[k];
								break;
							}
				}
				cout << " " << write << " ";
			}
			cout << endl;
		}
	}

	vector<string> Board::getTypes() {
		return types;
	}

	vector<char> Board::getPieces() {
		return pieces;
	}

	void Board::move(char from, char to) {
		for (vector<char>::iterator it = pieces.begin(); it != pieces.end(); it++) {
			if (*it == from) *it = to;
		}
	}

	void Board::move(int fromx, int fromy, int tox, int toy) {
		int from = 64, to = 64, i;

		for (i = 0; i < 3; i++) {
			from += (fromx % 2) << (i+3);
			fromx >>= 1;
		}
		for (i = 0; i < 3; i++) {
			from += (fromy % 2) << i;
			fromy >>= 1;
		}

		for (i = 0; i < 3; i++) {
			to += (tox % 2) << (i+3);
			tox >>= 1;
		}
		for (i = 0; i < 3; i++) {
			to += (toy % 2) << i;
			toy >>= 1;
		}
		
		move(from, to);
		
	}