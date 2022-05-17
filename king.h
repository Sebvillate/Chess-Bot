#ifndef KING_H
#define KING_H

#include "piece.h"
#include "window.h"
class King: public Piece{
		char direction;
	public:
		King(int x, int y, string colour, char pieceType, shared_ptr<Board> board, char direction, bool hasFirstMove);
		virtual vector<unique_ptr<pair<int,int>>> generatePossibleMoves() const override;
		virtual bool isPathValid(pair<int,int> &start, pair<int,int> &end) const override;
		virtual vector<unique_ptr<pair<int,int>>> generatePath(pair<int,int> &end) const override;
		virtual void move(int X, int Y) override;
		bool hasMoved() const override;
		bool isCastling(pair<int,int> &endPos, pair<int,int> &rookPos, pair<int,int> &rookEndPos) const;
		char getKingDir() const;	
		void draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const override;

};

#endif
