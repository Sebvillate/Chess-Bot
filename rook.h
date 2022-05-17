#ifndef ROOK
#define ROOK

#include "piece.h"
#include "window.h"
class Rook: public Piece{
	public:
	using Piece::Piece;
    	virtual bool isPathValid(pair<int,int> &start, pair<int,int> &end) const override;
    	virtual vector<unique_ptr<pair<int, int>>> generatePossibleMoves() const override; 
    	virtual vector<unique_ptr<pair<int, int>>> generatePath(pair<int,int> &endCoords) const override;
	virtual void move(int X, int Y) override;
	bool hasMoved() const override;
	void draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const override;
};

#endif
