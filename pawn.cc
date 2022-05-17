#include "pawn.h"

Pawn::Pawn(int x, int y, string colour, char pieceType, shared_ptr<Board> board, char direction, bool hasFirstMove)
	: Piece{x, y, colour, pieceType, board, hasFirstMove},
       	direction{direction}, 
	doubleFirstMove{false} 
{
	if ((direction != 'u') && (direction != 'd') && 
			(direction != 'l') && (direction != 'r')) {
		throw InvalidDirection{};
	}
}


vector<unique_ptr<pair<int,int>>> Pawn::generatePossibleMoves() const {
	vector<unique_ptr<pair<int,int>>> moves;
	if (shared_ptr<Board> tmpBoard = board.lock()) {
		if ((direction ==  'u') && (y - 1 >= 0)) {
			moves.push_back(make_unique<pair<int,int>>(x, y - 1));
			if (isFirstMove) {
				moves.push_back(make_unique<pair<int,int>>(x, y - 2));
			}
			if (x + 1 <= tmpBoard->maxWidth()) {	
				moves.push_back(make_unique<pair<int,int>>(x + 1, y - 1));
			}
			if (x - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y - 1));
			}
		}
		else if ((direction ==  'd') && (y + 1 <= tmpBoard->maxHeight())) {
			moves.push_back(make_unique<pair<int,int>>(x, y + 1));
			if (isFirstMove) {
				moves.push_back(make_unique<pair<int,int>>(x, y + 2));
			}
			if (x + 1 <= tmpBoard->maxWidth()) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y + 1));
			}
			if (x - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y + 1));
			}
		}	
		else if ((direction ==  'l') && (x - 1 >= 0)) {
			moves.push_back(make_unique<pair<int,int>>(x - 1, y ));
			if (isFirstMove) {
				moves.push_back(make_unique<pair<int,int>>(x - 2, y));
			}
			if (y + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y + 1));
			}
			if (y - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y - 1));
			}
		}
		else if ((direction ==  'r') && (x + 1 <= tmpBoard->maxWidth())) {
			moves.push_back(make_unique<pair<int,int>>(x + 1, y));
			if (isFirstMove) {
				moves.push_back(make_unique<pair<int,int>>(x + 2, y));
			}
			if (y - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y - 1));	
			}
			if (y + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y + 1));
			}
		}
	}
	return moves;	
}


bool Pawn::isPathValid(pair<int,int> &start, pair<int,int> &end) const {
	if ((start.first == end.first) && (start.second == end.second)) { //cannot move to itself
		return false;
	}
	if (direction == 'u') {
		return ((((end.second - start.second) == -1) && (abs(end.first - start.first) <= 1)) || 
				(isFirstMove && (end.second - start.second == -2) && (end.first - start.first == 0)));
	}
	if (direction == 'd') {
		return ((((end.second - start.second) == 1) && (abs(end.first - start.first) <= 1)) || 
				(isFirstMove && ((end.second - start.second == 2) && (end.first - start.first == 0))));
	}
	if (direction == 'l') {
		return ((((end.first - start.first) == -1) && (abs(end.second - start.second) <= 1)) || 
				(isFirstMove && ((end.first - start.first == -2) && (end.second - start.second == 0))));
	}
	if (direction == 'r') {
		return ((((end.first - start.first) == 1) && (abs(end.second - start.second) <= 1)) || 
				(isFirstMove &&  ((end.first - start.first == 2) && (end.second - start.second == 0))));
	}
	return false;
}


vector<unique_ptr<pair<int,int>>> Pawn::generatePath(pair<int,int> &endCoords) const {
	pair<int,int> startCoords {x,y};
	if (!isPathValid(startCoords, endCoords)) {
		throw Unreachable{};
	}
	vector<unique_ptr<pair<int,int>>> path;
	if (isFirstMove) {
		if (direction == 'u') {
			if (endCoords.first == x && endCoords.second - y == -2) {
				path.push_back(make_unique<pair<int,int>>(endCoords.first, endCoords.second + 1));
			}
		} else if (direction == 'd') {
			if (endCoords.first == x && endCoords.second - y == 2) {
				path.push_back(make_unique<pair<int,int>>(endCoords.first, endCoords.second - 1));
			}
		} else if (direction == 'l') {
			if (endCoords.first - x == -2 && endCoords.second == y) {
				path.push_back(make_unique<pair<int,int>>(endCoords.first + 1, endCoords.second));
			}
		} else if (direction == 'r') {
			if (endCoords.first - x == 2 && endCoords.second == y) {
				path.push_back(make_unique<pair<int,int>>(endCoords.first - 1, endCoords.second));
			}
		}
	}
	path.push_back(make_unique<pair<int,int>>(endCoords.first,endCoords.second));
	return path;
}



bool Pawn::didDoubleFirstMove() {
	return doubleFirstMove;
}

bool Pawn::hasMoved() const {
	return !isFirstMove;
}

void Pawn::move(int X, int Y) {
	if (abs(X - x) == 2 || abs(Y - y) == 2) {
		doubleFirstMove = true;
	}
	else {
		doubleFirstMove = false;
	}
	x = X;
	y = Y;
	isFirstMove = false; 
}

pair<int,int> Pawn::forward() const {
	pair<int,int> forward {x,y};
	if (direction == 'u') {
		forward.second -= 1;
	}
	if (direction == 'd') {
		forward.second += 1;
	}
	if (direction == 'l') {
		forward.first -= 1;
	}
	if (direction == 'r') {
		forward.first += 1;
	}
	return forward;
}

pair<int,int> Pawn::doubleForward() const {
	pair<int,int> forward {x,y};
	if (direction == 'u') {
		forward.second -= 2;
	}
	if (direction == 'd') {
		forward.second += 2;
	}
	if (direction == 'l') {
		forward.first -= 2;
	}
	if (direction == 'r') {
		forward.first += 2;
	}
	return forward;
}

pair<int,int> Pawn::leftDiag() const {
	pair<int,int> leftDiag {x,y};
	if (direction == 'u') {
		leftDiag.first -= 1;
		leftDiag.second -= 1;
	}
	if (direction == 'd') {
		leftDiag.first += 1;
		leftDiag.second += 1;
	}
	if (direction == 'l') {
		leftDiag.second += 1;
		leftDiag.first -= 1;
	}
	if (direction == 'r') {
		leftDiag.second -= 1;
		leftDiag.first += 1;
	}
	return leftDiag;	
}

pair<int,int> Pawn::left() const {
	pair<int,int> left {x,y};
	if (direction == 'u') {
		left.first -= 1;
	}
	if (direction == 'd') {
		left.first += 1;
	}
	if (direction == 'l') {
		left.second += 1;
	}
	if (direction == 'r') {
		left.second -= 1;
	}
	return left;
}


pair<int,int> Pawn::right() const {
	pair<int,int> right {x,y};
	if (direction == 'u') {
		right.first += 1;
	}
	if (direction == 'd') {
		right.first -= 1;
	}
	if (direction == 'l') {
		right.second -= 1;
	}
	if (direction == 'r') {
		right.second += 1;
	}
	return right;	
}


pair<int,int> Pawn::rightDiag() const {
	pair<int,int> rightDiag {x,y};
	if (direction == 'u') {
		rightDiag.first += 1;
		rightDiag.second -= 1;
	}
	if (direction == 'd') {
		rightDiag.first -= 1;
		rightDiag.second += 1;
	}
	if (direction == 'l') {
		rightDiag.second -= 1;
		rightDiag.first -= 1;
	}
	if (direction == 'r') {
		rightDiag.second += 1;
		rightDiag.first += 1;
	}
	return rightDiag;	
}

char Pawn::getDirection() const {
	return direction;
}

void Pawn::draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const {
	w.drawPawn(x, y, colour, zoomFactor);
}
