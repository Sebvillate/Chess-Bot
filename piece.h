#ifndef PIECE
#define PIECE

#include <memory>
#include <utility>
#include <string>
#include <vector>
#include "board.h"

class Xwindow;

using namespace std;
class Piece{
    protected:
        int x;
        int y;
        string colour;
        char pieceType;
        weak_ptr<Board> board;
        const int boardType;
	int val;
        bool isFirstMove;
        bool isInactive;
    public:
        Piece(int x, int y, string colour, char pieceType, shared_ptr<Board> board, bool hasFirstMove = false);
        virtual bool isPathValid(pair<int,int> &start, pair<int,int> &end) const = 0;
        virtual vector<unique_ptr<pair<int, int>>> generatePossibleMoves() const = 0; 
        virtual vector<unique_ptr<pair<int, int>>> generatePath(pair<int,int> &endCoords) const = 0;
        bool isPathClear(pair<int,int> &endCoords) const;
        virtual void move(int x, int y);
        char getPieceType() const;
        string getColour() const;
        int getX() const;
        int getY() const;
	int getVal() const;
        virtual bool hasMoved() const;
        virtual void draw(int x, int y, int colour, Xwindow &w, int zoomFactor) const = 0;
        bool getIsInactive() const;
        void setIsInactive(bool new_activity);
        bool doesFirstMoveMatter() const;
        friend void Board::undo(bool shouldRender);
        friend void Board::validateMove(shared_ptr<Piece> piece, pair<int,int> &newPos);
};

class OutofBounds{
	public:
	string message(){return "move is out of the bounds of the board";}
};

class Unreachable{
	public:
	string message(){return "end position is unreachable by this piece";}
};

class InvalidDirection {};

#endif
