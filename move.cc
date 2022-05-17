#include "move.h"

Move::Move(pair<int,int> &startCoords, pair<int, int> &endCoords)
	: startCoords{startCoords}, 
	endCoords{endCoords}, 
	isPromotion{false}, 
	isCastle{false}, 
	wasFirstMove{false}, 
	wasDoubleFirstMove{false}, 
	takenIndex{-1},
	promotionIndex{-1}
{}
