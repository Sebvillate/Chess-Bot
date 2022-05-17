#include "piece.h"
#include <utility>
#include <map>
#include <string>
#include <memory>

int abs(int x) {
	if ( x > 0) {
		return x;
	}
	return x * -1;
}
Piece::Piece(int x, int y, string colour, char pieceType, shared_ptr<Board> board, bool hasFirstMove)
	: x{x}, 
	y{y}, 
	colour{colour}, 
	pieceType{pieceType}, 
	board{board}, 
	boardType{board->isFourPlayer ? 4 : 2}, 
	isFirstMove{hasFirstMove}, 
	isInactive{false} {
	if ((pieceType == 'K') || (pieceType == 'k')) {
		val = 10; 	
	}
	else if((pieceType == 'P') || (pieceType == 'p')) {
		val = 1;
	}
	else if((pieceType == 'N') || (pieceType == 'n')) {
		val = 3;
	}
	else if((pieceType == 'R') || (pieceType == 'r')) {
		val = 5;
	}
	else if((pieceType == 'B') || (pieceType == 'b')) {
		val = 3;
	}
	else if((pieceType == 'Q') || (pieceType == 'q')) {
		val = 9;
	}
} 

void Piece::move(int X, int Y) {
	x = X;
	y = Y;
}

char Piece::getPieceType() const {
	return pieceType;
}

string Piece::getColour() const {
	return colour;
}

int Piece::getX() const {
	return x;
}

int Piece::getY() const {
	return y;
}

int Piece::getVal() const {
	return val;
}

bool Piece::hasMoved() const {
	return true;
}

bool Piece::getIsInactive() const {
	return isInactive;
}

void Piece::setIsInactive(bool new_activity) {
	isInactive = new_activity;
}

bool Piece::doesFirstMoveMatter() const {
    char pt = pieceType;
    return ((pt == 'p' || pt == 'P' || pt == 'r' || 
			    pt == 'R' || pt == 'k' || pt == 'K'));
}
