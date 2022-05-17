#include "queen.h"

bool Queen::isPathValid(pair<int,int> & start, pair<int,int> & end) const {
	if ((start.first == end.first) && (start.second == end.second)) { //cannot move to itself
		return false;
	}
	return ((abs(end.first - start.first) == abs(end.second - start.second)) || 
			((start.first == end.first) || (start.second == end.second)));
}

vector<unique_ptr<pair<int,int>>> Queen::generatePossibleMoves() const {
	vector<unique_ptr<pair<int,int>>> moves;	
	if (shared_ptr<Board> tmpBoard = board.lock()) {	
		int xAdj;
		int yAdj;
		for (int i = 0; i < 8; i++) {
			int currX = x;
			int currY = y;
			if (i == 0) {
				xAdj = 1;
				yAdj = 1;
			}
			else if (i == 1) {
				xAdj = -1;
				yAdj = 1;
			}
			else if (i == 2) {
				xAdj = 1;
				yAdj = -1;
			}
			else if (i == 3) {
				xAdj = -1;
				yAdj = -1;
			}
			else if (i == 4) {
				xAdj = 1;
				yAdj = 0;
			}
			else if (i == 5) {
				xAdj = -1;
				yAdj = 0;
			}
			else if (i == 6) {
				xAdj = 0;
				yAdj = 1;
			}
			else if (i == 7) {
				xAdj = 0;
				yAdj = -1;
			}
			while ((currX + xAdj >= 0) && (currX + xAdj <= tmpBoard->maxWidth()) && 
					(currY + yAdj >= 0) && (currY + yAdj <= tmpBoard->maxHeight())) {
				currX += xAdj;
				currY += yAdj;
				moves.push_back(make_unique<pair<int,int>>(currX, currY));
				pair<int, int> currCoord{currX, currY};
				if (tmpBoard->isOccupied(currCoord)) {
					break;
				}
			}
		}
	}
	return moves;
}

vector<unique_ptr<pair<int,int>>> Queen::generatePath(pair<int,int> & endCoords) const {
	pair<int,int> startCoords {x,y};
	if (!isPathValid(startCoords, endCoords)) {
		throw Unreachable{};
	}
	vector<unique_ptr<pair<int,int>>> path;
	int xAdj = 0;
	int yAdj = 0;
	if (endCoords.first > x) {
		xAdj = 1;
	}
	else if (endCoords.first < x) {
		xAdj = -1;
	}
	if (endCoords.second > y) {
		yAdj = 1;
	}
	else if (endCoords.second < y) {
		yAdj = -1;
	}
	int currX = x + xAdj;
	int currY = y + yAdj;
	while ((currX != endCoords.first) || (currY != endCoords.second)) {
		path.push_back(make_unique<pair<int,int>>(currX, currY));
		currX += xAdj;
		currY += yAdj;		
	}
	path.push_back(make_unique<pair<int,int>>(currX, currY));
	return path;
}

void Queen::draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const {
	w.drawQueen(x, y, colour, zoomFactor);
}
