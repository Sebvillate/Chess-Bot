#include "computerplayerdiff2.h"
#include "board.h"
#include "player.h"
#include "piece.h"
#include <iostream>
using namespace std;

ComputerPlayerDiff2::ComputerPlayerDiff2(const string &colour) : ComputerPlayerDiff1{colour} {}    

pair<pair<int, int>, pair<int, int>> ComputerPlayerDiff2::chooseMove(shared_ptr<Board> board, string &promotionPiece) {
	pair<pair<int, int>, pair<int, int>> move;
	promotionPiece = "q";
        pair<int, int> start;
	pair<int,int> end;
	int maxTakeVal = 0;
	int foundMove = false;
	int canCheck = 0;
	shared_ptr<Player> myPlayer = board->playerMap[colour];
	
	int numPieces = myPlayer->pieces.size();
	for (int i = 0; i < numPieces; i++) {
		shared_ptr<Piece> currPiece = myPlayer->pieces[i];
		vector<unique_ptr<pair<int,int>>> moves = currPiece->generatePossibleMoves();
		int numMoves = moves.size();
		for (int j = 0; j < numMoves; j++) {
			pair<int,int> * currMove = moves[j].get();
			if ((board->isMoveValid(currPiece, *currMove))) {
				int currCanCheck = false;
				pair<int,int> currStart {currPiece->getX(), currPiece->getY()};
				board->move(currStart, *currMove, colour, "q", false);            
				int currTakeVal = board->lastMoveTakenVal();
				if (currTakeVal > 0) {
					foundMove = true;
				}
				for (auto const &otherPlayer: board->playerMap) {
					if ((otherPlayer.first != colour) && 
							(board->isMoveValid(currPiece, otherPlayer.second->kingPos))) {
						currCanCheck += 1;
						foundMove = true;
					}
				}
				board->undo(false);
				if (currTakeVal > 9) {
					move.first = currStart;
					move.second = *currMove;
					return move;
				}
				if (((currTakeVal >= maxTakeVal) && (currCanCheck >= canCheck)) || 
						(((canCheck == 0) || (maxTakeVal == 0)) && 
						 (currCanCheck != 0) && (currTakeVal != 0))) {
					canCheck = currCanCheck;
					maxTakeVal = currTakeVal;
					start = currStart;
					end = *currMove;
				}
			}	
		}
	}
	if (foundMove) {
		move.first = start;
		move.second = end;
		return move;
	}
	return ComputerPlayerDiff1::chooseMove(board, promotionPiece);
}
