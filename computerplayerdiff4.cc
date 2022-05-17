#include "computerplayerdiff4.h"
#include "player.h"
#include <algorithm>
#include <random>

ComputerPlayerDiff4::ComputerPlayerDiff4(const string &colour): ComputerPlayerDiff3{colour} { 
	// Negative coordinates are used to indicate that a player made any
	// 	 move except that move (when transformed to positive coordinates)
	// {{100, 100}, {100, 100}} is used to indicate where it doesn't matter what move a player made
	if (colour == "black") {
		setups = {
	    		{{{3,6}, {3,4}}, {{7,6}, {5,5}}, {{2,7}, {5,4}}}, //London setup
	    		{{{6,7}, {5,5}}, {{6,6}, {6,5}}, {{5,7}, {6,6}}, {{4,7}, {6,7}}}, //Barcza system	
		};
		openings = {
	    		{{{4,6}, {4,4}},{{6,0}, {5,2}}}, //Alekhine Defense
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, {{2,4}, {3,3}}, 
				{{3,0}, {3,3}}, {{1,7}, {2,5}}, {{3,3}, {3,4}}, {{3,7}, {3,4}}, {{4,3}, {3,4}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, {{2,4}, {3,3}}, 
				{{3,0}, {3,3}}, {{4,1}, {4,2}}, {{4,3},{3,4}}, {{3,7}, {3,4}}, {{3,3}, {3,4}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, {{2,4}, {3,3}}, 
				{{3,0}, {3,3}}, {{6,7}, {5,5}}, {{4,3},{3,4}}, {{3,7}, {3,4}}, {{3,3}, {3,4}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, {{1,7}, {2,5}}, 
				{{3,3}, {2,4}}, {{3,4}, {4,3}}, {{3,0},{3,7}}, {{100,100},{100,100}} ,{{1,0}, {2,2}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{4,6}, {4,5}}, {{4,3}, {3,4}}, {{4,5}, {3,4}}, {{6,0}, {5,2}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{6,7}, {5,5}}, {{4,3}, {4,4}}, {{-5,-5}, {100,100}}, {{4,4}, {5,5}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{6,7}, {5,5}}, {{4,3}, {4,4}}, {{5,5}, {4,3}}, {{5,1}, {5,2}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{6,7}, {5,5}}, {{4,3}, {4,4}}, {{5,5}, {6,3}}, {{5,1}, {5,2}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{6,7}, {5,5}}, {{4,3}, {4,4}}, {{5,5}, {6,7}}, {{6,0}, {5,2}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{6,7}, {5,5}}, {{4,3}, {4,4}}, {{5,5}, {3,6}}, {{6,0}, {5,2}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{3,4}, {4,3}}, {{3,3}, {3,4}}, {{3,7}, {3,4}}, {{3,0}, {3,4}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, 
				{{3,4}, {4,3}}, {{3,3}, {3,4}}, {{6,7}, {5,5}}, {{1,0}, {2,2}}, {{6,6}, {6,5}}, 
				{{2,0}, {4,2}}, {{5,7}, {7,5}}, {{3,0}, {3,1}}, {{1,7}, {3,6}}, {{4,0}, {2,0}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, {{3,4}, {4,3}}, 
				{{3,3}, {3,4}}, {{6,7}, {5,5}}, {{1,0}, {2,2}}, {{6,6}, {6,5}}, 
				{{2,0}, {4,2}}, {{1,7}, {3,6}}, {{3,0}, {3,1}}, {{5,7}, {7,5}}, {{4,0},{2,0}}}, //Albin counter gambit
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,3}}, {{3,4}, {4,3}}, 
				{{3,3}, {3,4}}, {{4,6}, {4,5}}, {{5,0}, {1,4}}, {{2,7}, {3,6}}, {{3,4}, {4,5}}, 
				{{3,1}, {1,4}}, {{4,5}, {5,6}}, {{4,7}, {4,6}}, {{5,6}, {6,7}}}, //Albin counter gambit (FIND OUT PROMO)
		};
	}
	else if (colour == "white") {
		setups = {		
	    		{{{3,6}, {3,4}}, {{7,6}, {5,5}}, {{2,7}, {5,4}}}, //London setup
	    		{{{6,7}, {5,5}}, {{6,6}, {6,5}}, {{5,7}, {6,6}}, {{4,7}, {6,7}}}, //Barcza system
	    		{{{4,6}, {4,4}}, {{3,6}, {3,5}}, {{1,7}, {3,6}}, {{6,7}, {5,5}}, 
				{{6,6}, {6,5}}, {{5,7}, {6,6}}, {{4,7}, {6,7}}}, //King Indian Attack
		};
		openings = {			
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{3,3}, {2,4}}, 
				{{4,6}, {4,5}}, {{1,1}, {1,3}}, {{0,6}, {0,4}}, {{1,3}, {0,4}}, {{0,7}, {0,4}}}, //Queens Gambit Accepted
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{3,3}, {2,4}}, {{4,6}, {4,5}}, 
				{{1,1}, {1,3}}, {{0,6}, {0,4}}, {{2,1}, {2,2}}, {{0,4}, {1,3}}, {{2,2}, {1,3}}, {{3,7},{5,5}}}, //Queens Gambit Accepted
	    		{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{3,3}, {2,4}}, 
				{{4,6}, {4,5}}, {{1,1}, {1,3}}, {{0,6}, {0,4}}, {{2,1}, {2,2}}, 
				{{0,4}, {1,3}}, {{-2,-2},{100,100}}, {{1,3}, {2,2}}}, //Queens Gambit Accepted
			{{{3,6}, {3,4}}, {{3,1}, {3,3}}, {{2,6}, {2,4}}, {{4,1}, {4,2}}, {{1,7}, {2,5}}, 
				{{6,0}, {5,2}}, {{2,7}, {6,3}}, {{1,0}, {2,2}}, {{6,7}, {5,5}}   }, //Queens Gambit Declined
		};	
	}
}

pair<pair<int, int>, pair<int, int>> ComputerPlayerDiff4::chooseMove(shared_ptr<Board> board, string &promotionPiece) {
        pair<pair<int,int>,pair<int,int>> move {{-1, -1}, {-1, -1}};
	if (!board->isFourPlayer) {
		// STANDARD OPENINGS	
		try {
			pair<pair<int, int>, pair<int, int>> lastMove = board->lastMadeMove();
			int openingCount = openings.size();
			for (int i = 0; i < openingCount; i++) {
				if ((openings[i].size() <= board->movesCount()) ||
					       	!(openings[i][board->movesCount() - 1] == lastMove)){
					openings.erase(openings.begin() + i);
					openingCount -= 1;
					i -= 1;
				}
			}
			if (openingCount >= 1) {
				move = openings[0][board->movesCount()]; 
				return move;
			}
		} catch (InvalidMove &e) {
			move = openings[0][0];
		}
	}

	if (goToDiff3(board, promotionPiece)) {
		return ComputerPlayerDiff3::chooseMove(board, promotionPiece);
	}

	if (move == pair<pair<int,int>,pair<int,int>>(pair<int,int>(-1,-1),pair<int,int>(-1,-1))) {
		vector<pair<pair<pair<int,int>,pair<int,int>>,int>> movesToControlledSquares;
		for (int i = 0; i < board->playerMap[colour]->pieces.size(); i++) {
			shared_ptr<Piece> piece = board->playerMap[colour]->pieces[i];
			pair<int,int> start = pair<int,int>(piece->getX(),piece->getY());
			for (auto &m: piece->generatePossibleMoves()) {
				if (board->isMoveValid(piece, *m)) {
					if (board->isTargeted(*m, colour) < 2) {
						try {
							board->move(start, *m, colour, "q", false);
						} catch (InvalidMove &e) {
							continue;
						}
						vector<pair<int,int>> currentPlayerControlledSquares = board->controlledSquares(colour);
						vector<pair<int,int>> reachableEnemyControlledSquares;
						for (auto &player: board->playerMap) {
							if (player.first != colour) {
								for (auto &enemyControlled: board->controlledSquares(player.first)) {
									if ((!count(reachableEnemyControlledSquares.begin(), 
													reachableEnemyControlledSquares.end(), 
													enemyControlled)) && 
											count(currentPlayerControlledSquares.begin(), 
												currentPlayerControlledSquares.end(), 
												enemyControlled)) {
										reachableEnemyControlledSquares.emplace_back(enemyControlled.first, enemyControlled.second);
									}
								}
							}
						}
						int controlled = reachableEnemyControlledSquares.size();
						board->undo(false);
						pair<int,int> end = pair<int,int>(m->first,m->second);
						pair<pair<int,int>,pair<int,int>> movePair = pair<pair<int,int>,pair<int,int>>(start, *m);
						movesToControlledSquares.emplace_back(movePair, controlled);
					}
				}
			}
		}
		int numMoves = movesToControlledSquares.size();
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> pieceIndexGenerator(0, numMoves);
		int startOffset = pieceIndexGenerator(mt);
		int maxIndex = 0;
		int max = 0;
		for (int i = 0; i < numMoves; i++) {
			if (movesToControlledSquares[(i + startOffset) % numMoves].second > max) {
				maxIndex = (i + startOffset) % numMoves;
			}
		}
		move = movesToControlledSquares[maxIndex].first;
	}
	return move;
} 

bool ComputerPlayerDiff4::goToDiff3(shared_ptr<Board> board, string &promotionPiece) {
	// check if any of your pieces are under attack
        for (auto &piece: board->playerMap[colour]->pieces) {
                pair<int, int> piecePos = pair<int, int>(piece->getX(), piece->getY());
                for (auto const &otherPlayer: board->playerMap) {
                        if (otherPlayer.first != colour) {
                                for (auto &otherPiece: otherPlayer.second->pieces) {
                                        if (board->isMoveValid(otherPiece, piecePos)) {
                                        	return true;
					}
                                }
                        }
                }
        }
	// check if you can attack any piece
        for (auto &piece: board->playerMap[colour]->pieces) {
                vector<unique_ptr<pair<int,int>>> possibleMoveList = piece->generatePossibleMoves();
                pair<int,int> currStartPos {piece->getX(), piece->getY()};

                // loop through all possible moves for the piece
                for (int i = 0; i < possibleMoveList.size(); ++i) {
                        pair<int, int> possibleMove = *possibleMoveList[i].get();

                        // check if the move is valid
                        if (board->isMoveValid(piece, possibleMove)) {
                                board->move(currStartPos, possibleMove, colour, promotionPiece, false);
                                int currTakeVal = board->lastMoveTakenVal();
				if (currTakeVal > 0) {
					board->undo(false);
					return true;
				}
				for (auto const &player: board->playerMap) {
					if (player.first != colour) {
						if ((board->isCheckmate(player.first)) && (!player.second->pieces.empty())) {
							board->undo(false);
							return true;
						} 
						if ((board->isInCheck(player.first)) && (!player.second->pieces.empty())) {
						        board->undo(false);
							return true;
						}
					}
				}
				board->undo(false);
			}
		}
	}
	return false;	
}

