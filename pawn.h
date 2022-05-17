#ifndef PAWN_H
#define PAWN_H

#include "piece.h"
#include "window.h"
class Pawn : public Piece{
		bool doubleFirstMove;
		char direction;
	public:
		Pawn(int x, int y, string colour, char pieceType, shared_ptr<Board> board, char direction, bool hasFirstMove);
		virtual vector<unique_ptr<pair<int,int>>> generatePossibleMoves() const override;
		virtual bool isPathValid(pair<int,int> &start, pair<int,int> &end) const override;
		virtual vector<unique_ptr<pair<int,int>>> generatePath(pair<int,int> &endCoords) const override;
		virtual void move(int X, int Y) override;
		bool didDoubleFirstMove();
		bool hasMoved() const override;
		pair<int,int> forward() const;
		pair<int,int> doubleForward() const;
		pair<int,int> left() const;
		pair<int,int> right() const;
		pair<int,int> leftDiag() const;
		pair<int,int> rightDiag() const;
		char getDirection() const;
		friend void Board::undo(bool shouldRender);
		friend void Board::validateMove(shared_ptr<Piece> piece, pair<int,int> &newPos);
		void draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const override;
};

#endif
