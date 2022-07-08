#include "Rook.h";
#include <vector>

Rook::Rook(bool isWhite) {
	name = isWhite ? "wr" : "br";
	this->isWhite = isWhite;
}

vector<int> Rook::legalmoves(Piece *cells[8][8], int x, int y) {
	vector<int> moves;
	int i;
	for (i = 1; x + i <= 7; i++) {
		if (cells[x + i][y] == nullptr) {
			moves.push_back(x + i);
			moves.push_back(y);
		} else {
			if (this->isWhite != cells[x + i][y]->isWhite) { //Add as eating
				moves.push_back(x + i);
				moves.push_back(y);
			}
			break;
		}
		
	}
	for (i = 1; y + i <= 7; i++) {
		if (cells[x][y + i] == nullptr) {
			moves.push_back(x);
			moves.push_back(y + i);
		}
		else {
			if (this->isWhite != cells[x][y + i]->isWhite) { //Add as eating
				moves.push_back(x);
				moves.push_back(y + i);
			}
			break;
		}
	}
	for (i = 1; x - i >= 0; i++) {
		if (cells[x - i][y] == nullptr) {
			moves.push_back(x - i);
			moves.push_back(y);
		}
		else {
			if (this->isWhite != cells[x - i][y]->isWhite) { //Add as eating
				moves.push_back(x - i);
				moves.push_back(y);
			}
			break;
		}
	}
	for (i = 1; y - i >= 0; i++) {
		if (cells[x][y - i] == nullptr) {
			moves.push_back(x);
			moves.push_back(y - i);
		}
		else {
			if (this->isWhite != cells[x][y - i]->isWhite) { //Add as eating
				moves.push_back(x);
				moves.push_back(y - i);
			}
			break;
		}
	}
	return moves;
}