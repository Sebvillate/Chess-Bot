#include "knight.h"
using namespace std;

bool Knight::isPathValid(pair<int,int> & start, pair<int,int> & end) const {
	if ((start.first == end.first) && (start.second == end.second)) { //cannot move to itself
		return false;
	}
	return (((start.first - end.first) != 0) && ((start.second - end.second) != 0) && 
			((abs(end.first - start.first) + abs(end.second - start.second)) == 3));  
}

vector<unique_ptr<pair<int,int>>> Knight::generatePossibleMoves() const {
	vector<unique_ptr<pair<int,int>>> moves;
	if (shared_ptr<Board> tmpBoard = board.lock()) {	
		if (x + 2 <= tmpBoard->maxWidth()) {
			if (y + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x + 2, y + 1));
			}
			if (y - 1 >= 0) {	
				moves.push_back(make_unique<pair<int,int>>(x + 2, y - 1));
			}
		}
		if (x - 2 >= 0) {
			if (y + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x - 2, y + 1));
			}
			if (y - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 2, y - 1));
			}
		}
		if (y + 2 <= tmpBoard->maxHeight()) {
			if (x + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y + 2));
			}
			if (x - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y + 2));
			}
		}
		if (y - 2 >= 0) {
			if (x + 1 <= tmpBoard->maxHeight()) {
				moves.push_back(make_unique<pair<int,int>>(x + 1, y - 2));
			}
			if (x - 1 >= 0) {
				moves.push_back(make_unique<pair<int,int>>(x - 1, y - 2));
			}	
		}
	}
	return moves;
}

vector<unique_ptr<pair<int,int>>> Knight::generatePath(pair<int,int> & endCoords) const {
	pair<int,int> startCoords {x,y};
	if (!isPathValid(startCoords, endCoords)) {
		throw Unreachable{};
	}
	vector<unique_ptr<pair<int,int>>> path;
	path.push_back(make_unique<pair<int,int>>(endCoords.first, endCoords.second));
	return path;
}

void Knight::draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const {
	w.drawKnight(x, y, colour, zoomFactor);
}
