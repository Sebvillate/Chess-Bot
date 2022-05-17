#include "computerplayerdiff1.h"
#include <random>
#include "board.h"
#include "player.h"
#include "piece.h"
#include <iostream>
using namespace std;

ComputerPlayerDiff1::ComputerPlayerDiff1(const string &colour): ComputerPlayer{colour} {
}

pair<pair<int, int>, pair<int, int>> ComputerPlayerDiff1::chooseMove(shared_ptr<Board> board, string &promotionPiece) {
	// level 1 choose a move
	promotionPiece = "q";
	pair<pair<int, int>, pair<int, int>> move;
	pair<int,int> start;
	pair<int,int> end;
	Player * myPlayer = board->playerMap[colour].get();
	int numPieces = myPlayer->pieces.size();
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> pieceIndexGenerator(0, numPieces);
	int startOffset = pieceIndexGenerator(mt);
	for (int i = 0; i < numPieces; i++) {
		shared_ptr<Piece> currPiece = myPlayer->pieces[(i + startOffset) % numPieces];
		vector<unique_ptr<pair<int,int>>> moves = currPiece->generatePossibleMoves();
		uniform_int_distribution<int> moveIndexGenerator(0, moves.size());
		int moveStartOffset = moveIndexGenerator(mt);
		for (int j = 0; j < moves.size(); j++) {
			pair<int,int> * currMove = moves[(j + moveStartOffset) % moves.size()].get();
			if (board->isMoveValid(currPiece,*currMove)) {
				start.first = currPiece->getX();
				start.second = currPiece->getY();
				end.first = currMove->first;
				end.second = currMove->second;
				move.first = start;
				move.second = end;
				return move;
			}
		} 
	}
	return move;
}
