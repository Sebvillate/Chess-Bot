#include "computerplayerdiff3.h"
#include "player.h"
#include "piece.h"
#include <iostream>
#include <random>
#include <algorithm>
ComputerPlayerDiff3::ComputerPlayerDiff3(const string &colour): ComputerPlayerDiff2{colour} {}

vector<shared_ptr<Piece>> ComputerPlayerDiff3::threatenedPieces(shared_ptr<Board> board) {	
	vector<shared_ptr<Piece>> threatenedPieces;
	for (auto &piece: board->playerMap[colour]->pieces) {
		pair<int, int> piecePos = pair<int, int>(piece->getX(), piece->getY());
		for (auto const &otherPlayer: board->playerMap) {
			if (otherPlayer.first != colour) {
				for (auto &otherPiece: otherPlayer.second->pieces){
					if ((board->isMoveValid(otherPiece, piecePos)) && (count(threatenedPieces.begin(), threatenedPieces.end(), piece) == 0)) {
						int threatenedSize = threatenedPieces.size();
						for (int i = 0; i < (threatenedSize + 1); ++i) {
							if (i == threatenedPieces.size()) {
								threatenedPieces.emplace_back(piece);
							} else if ((piece->getVal()>= threatenedPieces[i]->getVal()) &&
								       	((piece->getX() != threatenedPieces[i]->getX()) 
									 || (piece->getY() != threatenedPieces[i]->getY()))) {
								threatenedPieces.insert(threatenedPieces.begin() + i, piece);
								break;
							}
						}
					}
				}
			}
		}
	}
	return threatenedPieces;
}

bool ComputerPlayerDiff3::newMaxValPiece(vector<shared_ptr<Piece>> & oldPieceVec, vector<shared_ptr<Piece>> & newPieceVec, int pieceMovedVal) {
	int newVecSize = newPieceVec.size();
	for (int i = 0; i < newVecSize; i++) {
		if ((count(oldPieceVec.begin(), oldPieceVec.end(), newPieceVec[i]) == 0) && 
				(newPieceVec[i]->getVal() >= pieceMovedVal)) {
			return true;
		}
	}
	return false;
}

pair<pair<int, int>, pair<int, int>> ComputerPlayerDiff3::chooseMove(shared_ptr<Board> board, string &promotionPiece) {
	int maxTakeVal = 0;
	promotionPiece = "q";
	int maxCheck = 0;
	bool foundMove = false;
	pair<pair<int,int>,pair<int,int>> move;
	bool found = false;
	vector<shared_ptr<Piece>> avoidCapture = threatenedPieces(board);
	random_device rd;
	mt19937 mt(rd());
	for (auto &piece: avoidCapture) {
		vector<unique_ptr<pair<int, int>>> possibleMoveList = piece->generatePossibleMoves();
		pair<int,int> currStartPos {piece->getX(), piece->getY()};
		bool isKing = false;
		if (piece->getPieceType() == 'k' || piece->getPieceType() == 'K') {
			isKing = true;
		}
		vector<shared_ptr<Piece>> piecesTargeting = board->piecesTargeting(currStartPos, colour, isKing);
		// if it's only targetted by one piece, try to take it if it minimizes
		// 	the loss and does not expose a new, higher value piece
		if (piecesTargeting.size() == 1) {
			pair<int,int> enemyCell {piecesTargeting[0]->getX(), piecesTargeting[0]->getY()};
			vector<shared_ptr<Piece>> defendingPieces = board->piecesCanReach(enemyCell, colour); 
			for (auto &defendingPiece: defendingPieces) {
				char defPieceType = defendingPiece->getPieceType();
				pair<int,int> defendingPieceCoords {defendingPiece->getX(), defendingPiece->getY()};	
				board->move(defendingPieceCoords, enemyCell, colour, "q", false);	
				vector<shared_ptr<Piece>> newExposed = threatenedPieces(board);
				if ((!newMaxValPiece(avoidCapture, newExposed, piece->getVal())) && 
						((!board->isTargeted(enemyCell, colour)) || ((defendingPiece->getVal() < piece->getVal()) 
							|| (piecesTargeting[0]->getVal() >= defendingPiece->getVal())))){
							move.first = defendingPieceCoords;
							move.second = enemyCell;
							found = true;
							board->undo(false);
							break;
				}
				board->undo(false);
			}	
		}
		foundMove = found;
		// see if the piece can move itself out of the way
		uniform_int_distribution<int> offsetGenerator(0, possibleMoveList.size());
		int randomOffset = offsetGenerator(mt);
		for (int i = 0; i < possibleMoveList.size(); ++i) {
			pair<int, int> possibleMove = *possibleMoveList[(i + randomOffset) % possibleMoveList.size()].get();
			bool isCapturable = false;
			int currTakeVal = 0;
			int currCheck = 0;
			if (board->isMoveValid(piece, possibleMove)) {
				board->move(currStartPos, possibleMove, colour, "q", false);
				int currTakeVal = board->lastMoveTakenVal();
				for (auto const &otherPlayer: board->playerMap) {
					if (board->isMoveValid(piece,otherPlayer.second->kingPos)) {
						currCheck += 1;
					}
					for (auto &otherPiece: otherPlayer.second->pieces) {
						if (board->isMoveValid(otherPiece,possibleMove)) {
							isCapturable = true;
							break;	
						}	
					}
					if (isCapturable) {
						break;
					}
				}
				if (currTakeVal > 9) {
					board->undo(false);
					move.first = currStartPos;
					move.second = possibleMove;
					return move;
				}
				if (!isCapturable) {
					if (!foundMove && ((currTakeVal >= maxTakeVal) && (currCheck >= maxCheck)) || 
							((maxCheck == 0|| maxTakeVal == 0) && (currTakeVal != 0) && (currCheck != 0))) {
						vector<shared_ptr<Piece>> newExposed = threatenedPieces(board);
						if (!newMaxValPiece(avoidCapture, newExposed, piece->getVal())){
							maxTakeVal = currTakeVal;
							maxCheck = currCheck;	
							move.first = currStartPos;
							move.second = possibleMove;
							found = true;
						}
					}			
				}
				board->undo(false);
			}
		}
		foundMove = found;
	}
	if (foundMove) {
		return move;
	}
	found = false;
	uniform_int_distribution<int> offsetGenerator2(0, board->playerMap[colour]->pieces.size());
	int randomOffset = offsetGenerator2(mt);
	maxTakeVal = 0;
	maxCheck = 0;
	for (int i = 0; i < board->playerMap[colour]->pieces.size(); i++) {
		shared_ptr<Piece> piece = board->playerMap[colour]->pieces[(i + randomOffset) % 
			board->playerMap[colour]->pieces.size()];
		pair<int,int> currCoords {piece->getX(), piece->getY()};
		if (count(avoidCapture.begin(), avoidCapture.end(), piece) == 0) {
			vector<unique_ptr<pair<int, int>>> possibleMoveList = piece->generatePossibleMoves();
			for (auto & currMove : possibleMoveList) {
				if (board->isMoveValid(piece, *currMove)) {
					board->move(currCoords, *currMove, colour, "q", false);
					int currTakeVal = board->lastMoveTakenVal();
					int currCheck = 0;
					for (auto const &otherPlayer: board->playerMap) {
						if (board->isMoveValid(piece,otherPlayer.second->kingPos)) {
							currCheck += 1;
						}
					}	
					if (currTakeVal > 9) {
						board->undo(false);
						move.first = currCoords;
						move.second = *currMove;
						return move;
					}
					if (!foundMove && ((currTakeVal >= maxTakeVal) && (currCheck >= maxCheck)) || 
							((maxCheck == 0|| maxTakeVal == 0) && 
								(currTakeVal != 0) && (currCheck != 0))) {
						vector<shared_ptr<Piece>> newExposed = threatenedPieces(board);
						if (!newMaxValPiece(avoidCapture, newExposed, piece->getVal())) {
							maxTakeVal = currTakeVal;
							maxCheck = currCheck;		
							move.first = currCoords;
							move.second = *currMove;
							found = true;
						}
					}
					board->undo(false);			
				}	
			}
		}
	}
	if (found) {
		return move;
	}
	return ComputerPlayerDiff2::chooseMove(board, promotionPiece);
}  
