#include "king.h"
#include <iostream>

King::King(int x, int y, string colour, char pieceType, shared_ptr<Board> board, char direction, bool hasFirstMove)
	: Piece::Piece(x, y, colour, pieceType, board, hasFirstMove), 
	direction{direction}
{
	if ((direction != 'u') && (direction != 'd') && 
			(direction != 'l') && (direction != 'r')) {
		throw InvalidDirection{};
	}
}

vector<unique_ptr<pair<int,int>>> King::generatePossibleMoves() const {
	vector<unique_ptr<pair<int,int>>> moves;
	if (shared_ptr<Board> tmpBoard = board.lock()) {
		if (x + 1 <= tmpBoard->maxWidth()) {
			moves.push_back(make_unique<pair<int,int>>(x + 1, y));
			if (y + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y + 1));
			}
			if (y - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y - 1));
			}
		}
		if (x - 1 >= 0) {
			moves.push_back(make_unique<pair<int,int>>(x - 1, y));
			if (y + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y + 1));
			}
			if (y - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y - 1));
			}	
		}
		if (y + 1 <= tmpBoard->maxHeight()) {
			moves.push_back(make_unique<pair<int,int>>(x, y + 1));
		}
		if (y - 1 >= 0) {
			moves.push_back(make_unique<pair<int,int>>(x, y - 1));
		}
	}
	return moves;
}

bool King::isPathValid(pair<int,int> &start, pair<int,int> &end) const {
	if ((start.first == end.first) && (start.second == end.second)) { // cannot move to itself
		return false;
	}
	return ((abs(start.first - end.first) <= 1) && 
			(abs(start.second - end.second) <= 1));
}

vector<unique_ptr<pair<int,int>>> King::generatePath(pair<int,int> &endCoords) const {
	pair<int,int> startCoords {x,y};
	if (!isPathValid(startCoords, endCoords)) {
		throw Unreachable{};
	}
	vector<unique_ptr<pair<int,int>>> path;
	path.push_back(make_unique<pair<int,int>>(endCoords.first,endCoords.second));
	return path;
}

bool King::hasMoved() const {
	return !isFirstMove;
}

void King::move(int X, int Y) {
	x = X;
	y = Y;
	isFirstMove = false;
}


bool King::isCastling(pair<int,int> &newPos, pair<int,int> &rookPos, pair<int,int> &finalRookPos) const {
	rookPos.first = x;
	rookPos.second = y;
	finalRookPos.first = x;
	finalRookPos.second = y;
	if (isFirstMove == true) {
		if (((direction == 'u') || (direction == 'd')) && 
				(!((newPos.second == y) && (abs(newPos.first - x) == 2)))) {
			return false;
		}
		if (((direction == 'l') || (direction == 'r')) && 
				(!((newPos.first == x) && (abs(newPos.second - y) == 2)))) {
			return false;
		}
		if (direction == 'u') {
			if (newPos.first == x - 2) {
				rookPos.first = x - 4;
				finalRookPos.first = x  - 1;
			}	
			if (newPos.first == x + 2) {
				rookPos.first = x + 3;
				finalRookPos.first = x + 1;
			}	
		}
		else if (direction == 'd') {
			if (newPos.first == x - 2) {
				if (boardType == 4) {
					rookPos.first = x - 3;
				}
				else {
					rookPos.first = x - 4;

				}
				finalRookPos.first = x - 1;
			}	
			if (newPos.first == x + 2) {
				if (boardType == 4) {
					rookPos.first = x + 4;
				}
				else {
					rookPos.first = x + 3;
				}
				finalRookPos.first = x + 1;
			}	
		}
		else if (direction == 'l') {
			if (newPos.second == y - 2) {
				rookPos.second = y - 4;
				finalRookPos.second = y - 1;
			}
			if (newPos.second == y + 2) {
				rookPos.second = y + 3;
				finalRookPos.second = y + 1;
			}
		}
		else if (direction == 'r') {
			if (newPos.second == y - 2) {
				rookPos.second = y - 3;
				finalRookPos.second = y - 1;
			}
			if (newPos.second == y + 2) {
				rookPos.second = y +  4;
				finalRookPos.second = y + 1;
			}
		}
		return true;
	}
	return false;
}

char King::getKingDir() const {
	return direction;
}

void King::draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const {
	w.drawKing(x, y, colour, zoomFactor);
}
