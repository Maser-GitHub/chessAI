#include "Board.h"
#include "Piece.h"
#include "Rook.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#define W true
#define B false

using namespace std;

	Board::Board() {
		initialize();
	}


	void Board::initialize() {
		addPiece(new Rook(B), 0, 0);
		addPiece(new Knight(B), 0, 1);
		addPiece(new Bishop(B), 0, 2);
		addPiece(new Queen(B), 0, 3);
		addPiece(new King(B), 0, 4);
		addPiece(new Bishop(B), 0, 5);
		addPiece(new Knight(B), 0, 6);
		addPiece(new Rook(B), 0, 7);

		addPiece(new Rook(W), 7, 0);
		addPiece(new Knight(W), 7, 1);
		addPiece(new Bishop(W), 7, 2);
		addPiece(new Queen(W), 7, 3);
		addPiece(new King(W), 7, 4);
		addPiece(new Bishop(W), 7, 5);
		addPiece(new Knight(W), 7, 6);
		addPiece(new Rook(W), 7, 7);
		
		for (int i = 0; i < 8; i++) {
			addPiece(new Pawn(B), 1, i);
			addPiece(new Pawn(W), 6, i);
			addPiece(nullptr, 2, i);
			addPiece(nullptr, 3, i);
			addPiece(nullptr, 4, i);
			addPiece(nullptr, 5, i);
		}
	}

	void Board::addPiece(Piece* piece, int x, int y) {
		cells[x][y] = piece;
	}

	void Board::print() {
		string write;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				write = cells[i][j] != nullptr ? cells[i][j]->name : "[]";
				cout << " " << write << " ";
			}
			cout << endl;
		}
	}

	void Board::move(int fromx, int fromy, int tox, int toy) {
		cells[tox][toy] = cells[fromx][fromy];
		cells[fromx][fromy] = nullptr;

	}

	vector<int> Board::possiblemoves(int x, int y) {
		Piece *piece = cells[x][y];
		return piece == nullptr ? vector<int>() : piece->legalmoves(cells, x, y);
	}