#include <vector>
#include <map>
#include <iostream>


using namespace std;

class Board {
private:
	vector<short int> pieces;
	// Dizionario che contiene i numeri e i relativi pezzi
	vector<string> types{"WK","BK","WQ","BQ","Wr","Br","Wk","Bk","Wb","Bb","Wp","Bp"};
public:

	//Add pieces in the traditional starting position
	void initializePieces() {
		addPiece("Br", 0, 0);
		addPiece("Bk", 1, 0);
		addPiece("Bb", 2, 0);
		addPiece("BQ", 3, 0);
		addPiece("BK", 4, 0);
		addPiece("Bb", 5, 0);
		addPiece("Bk", 6, 0);
		addPiece("Br", 7, 0);
		for (int i = 0; i < 8; i++) {
			addPiece("Bp", i, 1);
		}

		addPiece("Wr", 0, 7);
		addPiece("Wk", 1, 7);
		addPiece("Wb", 2, 7);
		addPiece("WQ", 3, 7);
		addPiece("WK", 4, 7);
		addPiece("Wb", 5, 7);
		addPiece("Wk", 6, 7);
		addPiece("Wr", 7, 7);
		for (int i = 0; i < 8; i++) {
			addPiece("Wp", i, 6);
		}
	}

	//Add a piece given the code
	void addPiece(short int piece) {
		pieces.push_back(piece);
	}

	//encode the piece and then it adds it to the pieces vector
	void addPiece(string type, short int x, short int y) {
		short int code = find(types.begin(), types.end(), type) - types.begin();
		if (code >= 12 || x<0 || x>7 || y<0 || y> 7) throw; //TODO maybe remove this for future efficency
		short int piece = 0;
		piece += x;
		piece += (y << 3);
		piece += (code << 6);
		addPiece(piece);
	}

	//Prints the board on the terminal
	//It is not optimized, because this will not be called only by users
	void print() {
		for (short int i = 0; i < 8; i++) {
			for (short int j = 0; j < 8; j++) {
				string write = "[]";
				for (short int piece : pieces) {
					if ((piece & 0b0000000000000111) == j)
						if ((piece & 0b0000000000111000) == i << 3)
							write = types[(piece & 0b0000001111000000) >> 6];
				}
				cout << " " << write << " ";
			}
			cout << endl;
		}
	}
};