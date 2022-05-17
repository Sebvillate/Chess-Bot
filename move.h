#ifndef MOVE_H
#define MOVE_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "piece.h"
using namespace std;

class Move {
	public:
		Move(pair<int,int> &startCoords, pair<int, int> &endCoords);	
		friend Board;
	private:
		pair<int, int> startCoords;
		pair<int, int> endCoords;
		shared_ptr<Piece> takenPiece;
		shared_ptr<Piece> promotedPawn;
		vector<shared_ptr<Piece>> checkmatePieces;
		bool isPromotion;
		bool isCastle;
		bool wasFirstMove;
		bool wasDoubleFirstMove;
		int takenIndex;
		int promotionIndex;
};
#endif
