#include <vector>
#include <map>
#include <iostream>


using namespace std;

class Board {
private:
	vector<short int> pieces;
	// Dizionario che contiene i numeri e i relativi pezzi
	vector<string> types{"Br","Bk","Bb","BQ","BK","Bp","Wr","Wk","Wb","WQ","WK","Wp"};
public:

	//Add pieces in the traditional starting position
	void initialize() {
		short int i;
		for (i = 0; i < 3; i++) {
			addPiece(types[i], i, 0);
			addPiece(types[i], 7-i, 0);
		}
		for (i = 0; i < 8; i++) {
			addPiece("Bp", i, 1);
		}
		for (i = 0; i < 3; i++) {
			addPiece(types[6+i], i, 7);
			addPiece(types[6+i], 7 - i, 7);
		}
		for (i = 0; i < 8; i++) {
			addPiece("Wp", i, 6);
		}
		addPiece("BQ", 3, 0);
		addPiece("BK", 4, 0);
		addPiece("WQ", 3, 7);
		addPiece("WK", 4, 7);
	}

	//Add pieces faster
	void initializeFast() {
		pieces = {0,7,65,70,130,133,328,329,330,331,332,333,334,335,440,447,505,
				510,570,573,752,753,754,755,756,757,758,759,195,260,635,700};
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