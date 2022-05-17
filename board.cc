#include <iostream>
#include <iomanip>
#include <algorithm>
#include "piece.h"
#include "board.h"
#include "player.h"
#include "pawn.h"
#include "king.h"
#include "rook.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "move.h"

using std::make_unique;
using std::make_shared;
using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::count;

const string Board::FOUR_PLAYER_VALID_COLOUR_CHARS = "WBRG";
const string Board::TWO_PLAYER_VALID_PIECE_CHARS = "pPrRnNbBqQkK";
const string Board::TWO_PLAYER_VALID_PROMO_PIECE_CHARS = "rRnNbBqQ";
const string Board::FOUR_PLAYER_VALID_PIECE_CHARS = "prnbqk";
const int Board::TWO_PLAYER_DIMENSION = 8;
const int Board::FOUR_PLAYER_DIMENSION = 14;

Board::Cell::Cell(int x, int y, bool isFourPlayer)
	: x{x}, 
	y{y}, 
	emptyChar{((x + y) % 2 == 0) ? WHITE_SPACE : BLACK_SPACE}, 
	isFourPlayer{isFourPlayer}, 
	isOutOfBounds{isFourPlayer ? ((x < 3 || x > 10) && (y < 3 || y > 10)) : false} 
{}

char Board::Cell::getChar() const {
	if (isOutOfBounds) {
		return OUT_OF_BOUNDS_SPACE;
	}
	if (p) {
		return p->getPieceType();
	} else {
		return emptyChar;
	}
}

bool Board::Cell::isEmpty() const {
	if (p) {
		return false;
	} else {
		return true;
	}
}

void Board::Cell::assignPiece(shared_ptr<Piece> newPiece) {
	p = newPiece;
}

void Board::Cell::movePiece(Board::Cell &other) {
	if (p) {
		if (other.p) {
			cout << "Space already occupied!" << endl;
			return;
		} else {
			other.p = std::move(p);
			return;
		}
	} else {
		cout << "Nothing to move!" << endl;
		return;
	}
}

Piece *Board::Cell::getPiece() const {
	return p.get();
}

Board::Cell *Board::getCell(int x, int y) const {
	return cells[y][x].get();
}

std::ostream &operator<< (std::ostream &out, Board::Cell &cell) {
	if (cell.isFourPlayer) {
		if (cell.isEmpty()) {
			out << cell.getChar();
		} else {
			out << static_cast<char>(cell.p->getColour()[0] - ' ');
		}
	}
	if (cell.isEmpty()) {
		out << cell.getChar();
	} else {
		out << cell.p->getPieceType();
	}
	return out;
}



Board::Board(map<string, shared_ptr<Player>> &playerMap, int dimension)
	: isFourPlayer{playerMap.size() == 4},
	shouldDrawByInsufficient{true}, 
	dimension{dimension}, 
	playerMap{playerMap}, 
	graphicZoomFactor{1} 
{
	cells.resize(dimension);
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			unique_ptr<Board::Cell> newCell = make_unique<Board::Cell>(j, i, isFourPlayer);
			cells[i].push_back(std::move(newCell));
		}
	}
	// initializing empty graphical board
	w = make_unique<Xwindow>();
	graphicSquareDim = 15;
	vCellMargin = (graphicSquareDim - 10) / 2;
	hCellMargin = (graphicSquareDim - 5) / 2;
	margin = (500 - dimension * graphicSquareDim) / 2;
	yMargin = margin - 40;
	renderGraphicBoard();
}

void Board::enableDrawByInsufficient(bool newState) {
	shouldDrawByInsufficient = newState;
}

void Board::zoom(int factor) {
	if (factor <= 0) {
		cout << "Invalid zoom factor. Must be positive." << endl;
		return;
	}
	if (isFourPlayer && factor > 2) {
		cout << "Invalid zoom factor. Value will cause board to exceed maximum width/height of 500px." << endl;
		return;
	} else if (factor > 3) {
		cout << "Invalid zoom factor. Value will cause board to exceed maximum width/height of 500px." << endl;
		return;
	}
	w->clear(0,0,500,500);
	graphicZoomFactor = factor;
	graphicSquareDim = 15 * factor;
	vCellMargin = (graphicSquareDim - 10) / 2;
	hCellMargin = (graphicSquareDim - 5) / 2;
	margin = (500 - dimension * graphicSquareDim) / 2;
	yMargin = margin - 20;
	renderGraphicBoard();
	renderGraphicPieces();
}

void Board::renderGraphicBoard() {
	renderGraphicText("Rendering Board...");
	w->fillRectangle(margin - 1, yMargin - 1, dimension * graphicSquareDim + 2, dimension * graphicSquareDim + 2);
	for (int i = 0; i < dimension; i++) {
		char letterLabel = i + 65;
		int number = dimension - i;
		w->drawString(i * graphicSquareDim + margin + hCellMargin, 
				yMargin - vCellMargin, string(1, letterLabel));
		w->drawString(i * graphicSquareDim + margin + hCellMargin, 
				dimension * graphicSquareDim + yMargin + graphicSquareDim - vCellMargin, 
				string(1, letterLabel));
		if (number > 9) {
			string numberLabel = "1" + string(1, static_cast<char>(number + 48 - 10));
			w->drawString(margin - graphicSquareDim + hCellMargin - 5, 
					(i + 1) * graphicSquareDim + yMargin - vCellMargin, numberLabel);
			w->drawString(dimension * graphicSquareDim + margin + hCellMargin, 
					(i + 1) * graphicSquareDim + yMargin - vCellMargin, numberLabel);
		} else {
			string numberLabel = string(1, number + 48);
			w->drawString(margin - graphicSquareDim + hCellMargin, 
					(i + 1) * graphicSquareDim + yMargin - vCellMargin, numberLabel);
			w->drawString(dimension * graphicSquareDim + margin + hCellMargin, 
					(i + 1) * graphicSquareDim + yMargin - vCellMargin, numberLabel);
		}
		for (int j = 0; j < dimension; j++) {
			if (isInBounds(i, j)) {
				w->fillRectangle(i * graphicSquareDim + margin, j * graphicSquareDim + yMargin, 
						graphicSquareDim, graphicSquareDim, 
						((i + j) % 2 == 0) ? Xwindow::Orange : Xwindow::White);
			} else {
				w->fillRectangle(i * graphicSquareDim + margin, j * graphicSquareDim + yMargin, 
						graphicSquareDim, graphicSquareDim, Xwindow::Gray);
			}
		}
	}
	clearGraphicText();
}

void Board::renderGraphicPieces() {
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			pair<int,int> coord = pair<int,int>(j,i);
			addGraphicPiece(coord);
		}
	}
}

void Board::clearGraphicText() {
	w->clear(0, yMargin + (dimension + 1) * graphicSquareDim + 10, 500, 20);
}

void Board::renderGraphicText(string message) {
	clearGraphicText();
	int textMargin = (500 - message.length() * 6) / 2;
	w->drawString(textMargin, yMargin + (dimension + 1) * graphicSquareDim + 20, message);
}

Board::~Board() {}

// adds piece at coord's identifier string/char onto the graphical board
void Board::addGraphicPiece(pair<int,int> &coord) {
	if (isOccupied(coord)) {
		int x = margin + coord.first * graphicSquareDim;
		int y = yMargin + (coord.second + 1) * graphicSquareDim;
		shared_ptr<Piece> pieceToBeDrawn = getPiece(coord);
		int colour = Xwindow::White;
		if (pieceToBeDrawn->getColour() == "black") {
			colour = Xwindow::Black;
		} else if (pieceToBeDrawn->getColour() == "red") {
			colour = Xwindow::Red;
		} else if (pieceToBeDrawn->getColour() == "green") {
			colour = Xwindow::Green;
		}
		pieceToBeDrawn->draw(x, y, colour, *w, graphicZoomFactor);
	}
}

// removes existing identifier string/char at coord on the graphical board
void Board::removeGraphicPiece(pair<int,int> &coord) {
	int x = coord.first;
	int y = coord.second;
	w->clear(margin + x * graphicSquareDim, yMargin + y * graphicSquareDim, 
			graphicSquareDim, graphicSquareDim);
	if ((x + y) % 2 == 0) {
		w->fillRectangle(margin + x * graphicSquareDim, 
				yMargin + y * graphicSquareDim, graphicSquareDim, 
				graphicSquareDim, Xwindow::Orange);
	}
}

// greys out piece to signify inactive piece in 4 player 
// 	chess for a piece whose Player resigned or was checkmated
void Board::drawInactivePiece(pair<int,int> &coord) {
	int x = margin + coord.first * graphicSquareDim;
	int y = yMargin + (coord.second + 1) * graphicSquareDim;
	if (isOccupied(coord)) {
		getPiece(coord)->draw(x, y, Xwindow::Gray, *w, graphicZoomFactor);
	}
}

void Board::addPiece(char pieceType, int x, int y, bool boardSetup, bool shouldRender) {
	if (isFourPlayer) {
		cout << "Invalid Piece input for 4 player chess!" << endl;
		return;
	} 
	if (TWO_PLAYER_VALID_PIECE_CHARS.find(pieceType) != std::string::npos) {
		if (isInBounds(x, y)) {
			if (cells[y][x]->isEmpty()) {
				string newColour = ('a' <= pieceType && 'z' >= pieceType) ? "black" : "white";
				shared_ptr<Piece> newPiece = createPiece(x, y, newColour, pieceType, boardSetup);
				cells[y][x]->assignPiece(newPiece);
				playerMap[newColour]->addPiece(newPiece);
				pair<int,int> coord = pair<int,int>(x,y);
				if (shouldRender) {
					addGraphicPiece(coord);
				}
				return;
			} else {
				cout << "Specified square already occupied!" << endl;
				return;
			}
		} else {
			cout << "Specified square not in bounds!" << endl;
			return;
		}
	} else {
		cout << "Invalid Piece character!" << endl;
		return;
	}
}

bool Board::isInBounds(int y, int x) const {
	if (isFourPlayer) {
		return (x >= 0 && x <= 13 && y >= 3 && y <= 10) || 
			(y >= 0 && y <= 13 && x >= 3 && x <= 10);
	} else {
		return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
	}
}

void Board::addPiece(string pieceType, pair<int,int> coord, bool boardSetup, bool shouldRender) {
	int x = coord.first;
	int y = coord.second;
	if (pieceType.length() == 2) {
		char colour = pieceType[0];
		char piece = pieceType[1];
		if (!isFourPlayer) {
			cout << "Invalid Piece input for 2 player chess!" << endl;
			return;
		}
		if (FOUR_PLAYER_VALID_COLOUR_CHARS.find(colour) != std::string::npos) {
			if (FOUR_PLAYER_VALID_PIECE_CHARS.find(piece) != std::string::npos) {
				if (isInBounds(x, y)) {
					if (cells[y][x]->isEmpty()) {
						string newColour;
						if (colour == 'W') {
							newColour = "white";
						} else if (colour == 'B') {
							newColour = "black";
						} else if (colour == 'R') {
							newColour = "red";
						} else if (colour == 'G') {
							newColour = "green";
						}
						shared_ptr<Piece> newPiece = createPiece(x, y, 
								newColour, pieceType[1], boardSetup);
						cells[y][x]->assignPiece(newPiece);
						playerMap[newColour]->pieces.push_back(newPiece);
						if (shouldRender) {
							addGraphicPiece(coord);
						}
						return;
					} else {
						cout << "Specified square already occupied!" << endl;
						return;
					}
				} else {
					cout << "Specified square not in bounds!" << endl;
					return;
				}
			} else {
				cout << "Invalid Piece character!" << endl;
				return;
			}
		} else {
			cout << "Invalid Colour character!" << endl;
			return;
		}
	} else if (pieceType.length() == 1) {
		addPiece(pieceType[0], x, y, boardSetup, shouldRender);
	} else {
		cout << "Invalid input " << 
			(isFourPlayer ? "for 4 player chess" : "for 2 player chess") << "!" << endl;
		return;
	}
}


pair<pair<int,int>,pair<int,int>> Board::lastMadeMove(){
	if (allMoves.size() != 0) {
		pair<pair<int,int>,pair<int,int>> lastMove {allMoves[allMoves.size() -1]->startCoords, 
			allMoves[allMoves.size() - 1]->endCoords};
		return lastMove;
	}
	throw InvalidMove{};
}

int Board::movesCount(){
	return allMoves.size();
}

shared_ptr<Piece> Board::createPiece(int x, int y, string colour, char pieceType, bool boardSetup) {
	char direction;
	if (colour == "black") {
		direction = 'd';
	} else if (colour == "white") {
		direction = 'u';
	} else if (colour == "red") {
		direction = 'r';
	} else if (colour == "green") {
		direction = 'l';
	}
	if (pieceType == 'p' || pieceType == 'P') {
		return make_shared<Pawn>(x, y, colour, pieceType, 
				shared_from_this(), direction, !boardSetup);
	} else if (pieceType == 'k' || pieceType == 'K') {
		return make_shared<King>(x, y, colour, pieceType, 
				shared_from_this(), direction, !boardSetup);
	} else if (pieceType == 'r' || pieceType == 'R') {
		return make_shared<Rook>(x, y, colour, pieceType, 
				shared_from_this(), !boardSetup);
	} else if (pieceType == 'n' || pieceType == 'N') {
		return make_unique<Knight>(x, y, colour, 
				pieceType, shared_from_this());
	} else if (pieceType == 'b' || pieceType == 'B') {
		return make_unique<Bishop>(x, y, colour, 
				pieceType, shared_from_this());
	} else if (pieceType == 'q' || pieceType == 'Q') {
		return make_unique<Queen>(x, y, colour, pieceType, shared_from_this());
	} else {
		cout << "Invalid Piece type!" << endl;
		return nullptr;
	}
}

void Board::removePieceFromPlayer(pair<int,int> coord, string colour) {
	vector<shared_ptr<Piece>> &playerPieces = playerMap[colour]->pieces;
	vector<shared_ptr<Piece>>::iterator it = playerPieces.begin();
	for ( ; it != playerPieces.end(); ) {
		if (*it == cells[coord.second][coord.first]->p) {
			it = playerPieces.erase(it);
		} else {
			++it;
		}
	}
}

void Board::removePiece(pair<int,int> coord, bool isCheckmate, bool shouldRender) {
	int x = coord.first;
	int y = coord.second;
	if (isInBounds(x, y)) {
		if (!cells[y][x]->isEmpty()) {
			if (!cells[y][x]->p->getIsInactive()) {
				removePieceFromPlayer(coord, cells[y][x]->p->getColour());
			}
			if (shouldRender) {
				removeGraphicPiece(coord);
			}
			if (!isCheckmate || !isFourPlayer) {
				cells[y][x]->p.reset();
			} else {
				if (shouldRender) {
					// here, we don't remove an inactive piece from the board, 
					// 	just from the player's control
					drawInactivePiece(coord);
				}
				// draw it as an 'x' on graphical board and 
				// 	then set the piece to inactive
				cells[y][x]->p->setIsInactive(true);
			}
		}
	} else {
		cout << "Specified space not in bounds!" << endl;
	}
}

void Board::defaultSetup() {
	renderGraphicText("Rendering Pieces...");
	int startDimension = (dimension - 8)/2;
	for (int i = startDimension; i < startDimension + 8; i++) {
		if (isFourPlayer) {
			addPiece("Bp", pair<int,int>(i, 1));
			addPiece("Wp", pair<int,int>(i, 12));
			addPiece("Rp", pair<int,int>(1, i));
			addPiece("Gp", pair<int,int>(12, i));
			string firstRowW;
			string firstRowB;
			string firstRowR;
			string firstRowG;
			if (i == 3 || i == 10) {
				firstRowW = "r";
			} else if (i == 4 || i == 9) {
				firstRowW = "n";
			} else if (i == 5 || i == 8) {
				firstRowW = "b";
			} else if (i == 6) {
				firstRowW = "q";
				firstRowB = "k";
				firstRowR = "k";
				firstRowG = "q";
			} else if (i == 7) {
				firstRowW = "k";
				firstRowB = "q";
				firstRowR = "q";
				firstRowG = "k";
			}
			if (i != 6 && i != 7) {
				firstRowB = firstRowW;
				firstRowR = firstRowW;
				firstRowG = firstRowW;
			}
			addPiece("B" + firstRowB, pair<int,int>(i, 0));
			addPiece("W" + firstRowW, pair<int,int>(i, 13));
			addPiece("R" + firstRowR, pair<int,int>(0, i));
			addPiece("G" + firstRowG, pair<int,int>(13, i));
		} else {
			addPiece('p', i, 1, false);
			addPiece('P', i, 6, false);
			char firstRowBlack;
			char firstRowWhite;
			if (i == 0 || i == 7) {
				firstRowBlack = 'r';
				firstRowWhite = 'R';
			} else if (i == 1 || i == 6) {
				firstRowBlack = 'n';
				firstRowWhite = 'N';
			} else if (i == 2 || i == 5) {
				firstRowBlack = 'b';
				firstRowWhite = 'B';
			} else if (i == 3) {
				firstRowBlack = 'q';
				firstRowWhite = 'Q';
			} else {
				firstRowBlack = 'k';
				firstRowWhite = 'K';
			}
			addPiece(firstRowBlack, i, 0, false);
			addPiece(firstRowWhite, i, 7, false);
		}
	}
	setAllKingPos();
	clearGraphicText();
}

void Board::render() const {
	for (int i = -1; i <= dimension; i++) {
		if (i == dimension || i == -1) {
			if (i == dimension) {
				cout << endl;
			}
			cout << "   ";
			for (int j = 'a'; j < 'a' + dimension; j++) {
				cout << static_cast<char>(j);
				if (isFourPlayer) {
					cout << ' ';
				}
			}
			if (i == -1) {
				cout << endl;
			}
		} else {
			cout << left << setw(2) << dimension - i << ' ';
			for (int j = 0; j < dimension; j++) {
				cout << *(cells[i][j]);
			}
			cout << ' ' << dimension - i;
		}
		cout << endl;
	}
}

// for readability
int Board::maxWidth() const {
	return dimension - 1;
}

// for readability
int Board::maxHeight() const {
	return maxWidth();
}

bool Board::isOccupied(pair<int, int> &coord) const {
	return !cells[coord.second][coord.first]->isEmpty();
}

bool Board::isEmpty() const {
	for (auto& row: cells) {
		for (auto& c: row) {
			if (!c->isOutOfBounds && !c->isEmpty()) {
				return false;
			}
		}
	}
	return true;
}

bool Board::isInCheck(string player) {
	return (isTargeted(playerMap[player]->kingPos, player, true) != 0);
}

shared_ptr<Piece> Board::getSingleAttacker(pair<int,int> &coord) const {
	for (auto const &otherPlayer: playerMap) {
		if (otherPlayer.first != getPiece(coord)->getColour()) {
			for (auto &piece: otherPlayer.second->pieces) {
				pair<int,int> targetCoord = pair<int,int>(getPiece(coord)->getX(), 
						getPiece(coord)->getY());
				if (isPathClear(piece, targetCoord)) {
					return piece;
				}
			}
		}
	}
	return nullptr;
}

vector<shared_ptr<Piece>> Board::piecesTargeting(pair<int,int> &coord, string targetedColour, bool isCheck){
	vector<shared_ptr<Piece>> targetingPieces;
	for (auto const &otherPlayer: playerMap) {
		if (otherPlayer.first != targetedColour) {
			for (auto &piece: otherPlayer.second->pieces) {
				bool targeted = isCheck ? isPathClear(piece, coord) : isMoveValid(piece, coord);
				if (targeted) {
					targetingPieces.push_back(piece);
				}
			}
		}
	}
	return targetingPieces;
}

int Board::isTargeted(pair<int,int> &coord, string targetedColour, bool isCheck) {
	return piecesTargeting(coord, targetedColour, isCheck).size();
}

// for intercepting a check path if only one path
bool Board::isReachable(pair<int,int> &coord, string movingColour) {
	for (auto &piece: playerMap[movingColour]->pieces) {
		if ((piece->getPieceType() != 'k' && piece->getPieceType() != 'K')) {
			if (isMoveValid(piece, coord)) {
				return true;
			}
		}
	}
	return false;
}

vector<shared_ptr<Piece>>  Board::piecesCanReach(pair<int,int> &coord, string movingColour) { 
	vector<shared_ptr<Piece>> pieces;
	for (auto &piece: playerMap[movingColour]->pieces) {
		if (isMoveValid(piece, coord)) {
			int piecesLen = pieces.size();
			for (int i = 0; i <= piecesLen; i++){
				if (i == piecesLen){
					pieces.push_back(piece);
				}
				if (piece->getVal() <=  pieces[i]->getVal()){
					pieces.insert(pieces.begin() + i, piece);
				}
			}
		}
	}
	return pieces;
}

bool Board::isStalemate(string player) {
	if (!(isInCheck(player))) {
		for (auto &piece: playerMap[player]->pieces) {
        		for (auto &pieceMove: piece->generatePossibleMoves()) {
				if (isMoveValid(piece, *pieceMove)) {
					return false;
            			}
			}
        	}
		return true;
	}
	return false;
}

bool Board::isInsufficientMaterial() {
	if (!isFourPlayer) {
		bool whiteCanDraw = false;
		vector<shared_ptr<Piece>> whitePieces = playerMap["white"]->pieces;
		if (whitePieces.size() == 1) {
			whiteCanDraw = true;
		} else if (whitePieces.size() == 2) {
			if (whitePieces[0]->getPieceType() == 'B' ||
					whitePieces[1]->getPieceType() == 'B' ||
					whitePieces[0]->getPieceType() == 'N' ||
					whitePieces[1]->getPieceType() == 'N' ) {
				whiteCanDraw = true;
			}
		}
		if (whiteCanDraw) {
			vector<shared_ptr<Piece>> blackPieces = playerMap["black"]->pieces;
			if (blackPieces.size() == 1) {
				return true;
			} else if (blackPieces.size() == 2) {
				if (blackPieces[0]->getPieceType() == 'b' ||
						blackPieces[1]->getPieceType() == 'b' ||
						blackPieces[0]->getPieceType() == 'n' ||
						blackPieces[1]->getPieceType() == 'n' ) {
					return true;
				}
			}
		} else {
			return false;
		}
	}
	return false;
}

bool Board::wasLastMoveCheckmate(string player) {
	if (!allMoves.empty()) {
		if (allMoves[allMoves.size() - 1]->checkmatePieces.size()) {
			for (int i=0; i < allMoves[allMoves.size() - 1]->checkmatePieces.size(); i++) {
				if (player == allMoves[allMoves.size() - 1]->checkmatePieces[i]->getColour()) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Board::isCheckmate(string player) {
	int numAttackers = isTargeted(playerMap[player]->kingPos, player, true);
	if (numAttackers) {
		shared_ptr<Piece> kingPiece = getPiece(playerMap[player]->kingPos);
		for (auto &kingMove: kingPiece->generatePossibleMoves()) {
			if (isMoveValid(kingPiece, *kingMove)) {
				// guarantees the move will not put you in check
				return false;
			}
		}
		if (numAttackers > 1) {
			return true;
		} else if (numAttackers == 1) {
			pair<int,int> kingCoords = pair<int,int>(kingPiece->getX(), 
					kingPiece->getY());
			shared_ptr<Piece> attacker = getSingleAttacker(kingCoords);
			
			// iterate through all squares in the attacker's path 
			// 	to see if checked player can intercept with any other pieces
			for (auto &attackerMove: attacker->generatePath(kingCoords)) {
				if (isReachable(*attackerMove, kingPiece->getColour())) {
					return false;
				}
			}
			pair<int,int> attackerCoords = pair<int,int>(attacker->getX(), attacker->getY());
			
			// check if the attacking piece is takeable
			if (isReachable(attackerCoords, kingPiece->getColour())) {
				return false;
			}
			return true;
		}
	}
	return false;
}

bool Board::justMoved(pair<int,int> &coord) {
	int backstep;
	if (isFourPlayer) {
		backstep = 3;
	}
	else {
		backstep = 1;
	}
	if (allMoves.size() < 3) {
		backstep = allMoves.size();
	}
	for (int i = 0; i < backstep; i ++) {
		if (allMoves[allMoves.size() - (1 + i)]->endCoords == coord) {
			return true;
		}
	}
	return false;	
}

void Board::removePlayerPieces(string player, bool isCheckmate) {
	for (int i = playerMap[player]->pieces.size() - 1; i >= 0 ; i--) {
		auto piece = playerMap[player]->pieces[i];
		removePiece(pair<int,int>(piece->getX(), piece->getY()), isCheckmate);
	}
}

void Board::clearBoardPieces() {
	for (auto &row: cells) {
		for (auto &cell: row) {
			if (!cell->isEmpty()) {
				pair<int,int> pieceToBeRemoved = pair<int,int>(cell->p->getX(), cell->p->getY());
				removeGraphicPiece(pieceToBeRemoved);
				cell->p.reset();
			}
		}
	}
}

void Board::move(pair<int,int> &start, pair<int, int> &end, string & colour, string promotionPiece, bool shouldRender) {
	if (!cells[start.second][start.first]->isEmpty()) {
		if (getPiece(start)->getColour() == colour) {
			int startX = start.first;
			int startY = start.second;
			int endX = end.first;
			int endY = end.second;
			bool isKing = false;
			bool isCastle = false;
			pair<int,int> rookPos;
			pair<int,int> rookCastlePos;
			Cell *startCell = cells[startY][startX].get();
			Cell *endCell = cells[endY][endX].get();
			shared_ptr<Piece> startPiece = getPiece(start);
			try {
				validateMove(startPiece, end);
			} catch (InvalidMove &e) {
				throw;
			}
			if (auto kingPiece = std::dynamic_pointer_cast<King>(startPiece)) { 
				isKing = true;
				isCastle = kingPiece->isCastling(end, rookPos, rookCastlePos);
			}
			unique_ptr<Move> newMove = make_unique<Move>(start, end);
			bool isTake = false;
			if (isOccupied(end) && !isCastle) {
				newMove->takenPiece = getPiece(end);
				isTake = true;
			}
			if (auto pawnPiece = std::dynamic_pointer_cast<Pawn>(startPiece)){
				pair<int,int> leftDiag = pawnPiece->leftDiag();
				pair<int,int> rightDiag = pawnPiece->rightDiag();
				pair<int,int> left = pawnPiece->left();
				pair<int,int> right = pawnPiece->right();
				if (pawnPiece->didDoubleFirstMove()) {
					newMove->wasDoubleFirstMove = true;
				}
				if ((pawnPiece->leftDiag() == end) && (!isOccupied(leftDiag)) && justMoved(left)){
					newMove->takenPiece = getPiece(left);
					isTake = true;
				}
				else if ((pawnPiece->rightDiag() == end) && (!isOccupied(rightDiag)) && justMoved(right)){
					newMove->takenPiece = getPiece(right);
					isTake = true;
				}
				if ((!isFourPlayer && ((pawnPiece->getDirection() == 'u') && (endY == 0))) || 
						((pawnPiece->getDirection() == 'u') && (endY == 6)) ||
						((pawnPiece->getDirection() == 'd') && (endY == 7)) ||
						((pawnPiece->getDirection() == 'l') && (endX == 6)) ||
						((pawnPiece->getDirection() == 'r') && (endX == 7))) {
					newMove->isPromotion = true;

					// setting the promotionPiece variable depending on 2 or 4 players
					if (isFourPlayer) {
						// since 4 player will always promote to queen, won't throw on bad
						//     initial user input for promotionPiece
						promotionPiece = std::string(1, 
								static_cast<char>(pawnPiece->getColour()[0] - ' ')) + "q";
					} else {
						// validate promotion piece input in this entire block, throw on bad input, 
						// 	so we don't change any pieces until input validated
						if (promotionPiece == "") {
							cout << "Please specify a piece to promote your pawn to" << endl;
							cin >> promotionPiece;
						}
						if (promotionPiece.length() > 1) {
							throw BadPromotionPiece{};
						} else {
							// captial or lowercase char, set to proper one based on colour
							if (TWO_PLAYER_VALID_PROMO_PIECE_CHARS.find(promotionPiece) != string::npos) {
								if ('a' <= promotionPiece[0] && promotionPiece[0] <= 'z') {
									if (startPiece->getColour() == "white") {
										promotionPiece[0] -= ' ';
									}
								} else if ('A' <= promotionPiece[0] && promotionPiece[0] <= 'Z') {
									if (startPiece->getColour() == "black") {
										promotionPiece[0] += ' ';
									}
								}
							} else {
								throw BadPromotionPiece{};
							}
						}
					}
					if (isTake) {
						if (!newMove->takenPiece->getIsInactive()) {
							newMove->takenIndex = getPieceIndexWithinPlayer(newMove->takenPiece, newMove->takenPiece->getColour());
						}
						removePiece(pair<int,int>(newMove->takenPiece->getX(), 
									newMove->takenPiece->getY()), false, shouldRender);
					}
					// we just remove start and add promoted piece instead of moving and then 
					// 	removing the pawn which is redundant, so we check later if the move 
					// 	was a promotion. If it wasn't, then we actually do move the piece (see below)
					newMove->promotionIndex = getPieceIndexWithinPlayer(startPiece, colour);
					newMove->promotedPawn = startPiece;
					removePiece(start, false, shouldRender);
					addPiece(promotionPiece, end, true, shouldRender);
				}
			}
			if (startPiece->doesFirstMoveMatter() && !startPiece->hasMoved()) {
				newMove->wasFirstMove = true;
			}
			// as stated above, if is promotion, then we technically already "moved" start 
			// 	piece to end piece and promoted (really just removed start and add end)
			if (!newMove->isPromotion) {
				if (isTake) {
					if (!newMove->takenPiece->getIsInactive()) {
						newMove->takenIndex = getPieceIndexWithinPlayer(newMove->takenPiece, newMove->takenPiece->getColour());
					}
					removePiece(pair<int,int>(newMove->takenPiece->getX(), 
								newMove->takenPiece->getY()), false, shouldRender);
				}
				if (shouldRender) {
		    		    moveAndRenderPiece(start, end);
			        } else {
					movePiece(start, end);
				}
			}
			for (auto &player : playerMap) {
				if (player.first != colour) {
					if ((isCheckmate(player.first)) && (!player.second->pieces.empty())) {
						// checkmate
						if (shouldRender) {
							render();
							if (isFourPlayer) {
								cout << "Checkmate!" << endl;
								renderGraphicText("Checkmate!");
							} else {
								cout << "Checkmate! ";
							}
						}
						for (auto &piece: playerMap[player.first]->pieces) {
							newMove->checkmatePieces.push_back(piece);
						}
					} else if ((isInCheck(player.first)) && (!player.second->pieces.empty())) {
						// check
						if (shouldRender) {
							cout << (char) toupper(player.first[0]) << 
								player.first.substr(1, player.first.length() - 1) << 
								" is in check." << endl;
							renderGraphicText(string(1, (char) toupper(player.first[0])) + 
									player.first.substr(1, player.first.length() - 1) + 
									" is in check.");
						}
					} else if ((isStalemate(player.first)) && (!player.second->pieces.empty())) {
						// stalemate
						if (shouldRender) {
							render();
							cout << "Stalemate!" << endl;
							renderGraphicText("Stalemate!");
							for (auto &playerToRemove : playerMap) {
								for (auto &piece: playerMap[playerToRemove.first]->pieces) {
									newMove->checkmatePieces.push_back(piece);
								}
							}
						}
					}
				}
			}
			if (shouldRender) {
				if (shouldDrawByInsufficient && isInsufficientMaterial()) {
					render();
					cout << "Draw by Insufficient Material!" << endl;
					renderGraphicText("Draw by Insufficient Material!");
				}
			}
			allMoves.push_back(std::move(newMove));
			if ((isKing) && (isCastle)){
				if(shouldRender){
					moveAndRenderPiece(rookPos, rookCastlePos);
				} else {
					movePiece(rookPos, rookCastlePos);
				}
				unique_ptr<Move> castleMove = make_unique<Move>(rookPos, rookCastlePos);
				castleMove->wasFirstMove = true;
				castleMove->isCastle = true;
				allMoves.push_back(std::move(castleMove));
			}
			setAllKingPos();
			return;
		} else {
			throw WrongPlayer{};
		}
	} else {
		throw PieceNotFound{};
	}
}

shared_ptr<Piece> Board::getPiece(pair<int,int> &coord) const {
	if (cells[coord.second][coord.first]->p) {
		return cells[coord.second][coord.first]->p;
	} else {
		return shared_ptr<Piece>();
	}
}

int Board::lastMoveTakenVal() {
	int takenVal = 0;
	if (allMoves.size() >= 1) {
    		if (allMoves[allMoves.size() - 1]->takenPiece) {
      			takenVal += allMoves[allMoves.size() - 1]->takenPiece->getVal();
    		}
		for (auto &takenPiece: allMoves[allMoves.size()-1]->checkmatePieces) {
			takenVal += takenPiece->getVal();
		}
	}
	return takenVal;
}

bool Board::isInvalidCoord(pair<int,int> &coord) const {
	return cells[coord.second][coord.first]->isOutOfBounds;
} 

bool Board::isPathClear(shared_ptr<Piece> piece, pair<int,int> &newPos) const {
	vector<unique_ptr<pair<int,int>>> path;
	try {
		path = piece->generatePath(newPos);
	}
	catch(Unreachable & e) {
		return false;
	}
	for (vector<unique_ptr<pair<int,int>>>::iterator i = path.begin(); i != path.end() - 1; i++) {
		if (isOccupied(**i) || isInvalidCoord(**i)) {
			return false;
		}
	}
	if (auto pawnPiece = std::dynamic_pointer_cast<Pawn>(piece)) {
		if ((pawnPiece->forward() == newPos) && (isOccupied(newPos))) {
			return false;
		}
	} 
	return true;
}


vector<shared_ptr<pair<int,int>>> Board::enemyCanCheckWith(string playerToCheck) {	
	vector<shared_ptr<pair<int,int>>> piecePos;
	for (auto &otherPlayer: playerMap) {
		if (otherPlayer.first != playerToCheck) {
			string otherPlayerColour = otherPlayer.first;
			for (auto &enemyPiece: otherPlayer.second->pieces) {
				pair<int,int> startPos = pair<int,int>(enemyPiece->getX(), enemyPiece->getY());
				vector<unique_ptr<pair<int,int>>> possibleMoves = enemyPiece->generatePossibleMoves();
				int moveCount = possibleMoves.size();
				for (int i = 0; i < moveCount; i++) {
					if (isMoveValid(enemyPiece, *possibleMoves[i])) {
						move(startPos,*possibleMoves[i], otherPlayerColour, "q", false);
						if (isMoveValid(enemyPiece, playerMap[playerToCheck]->kingPos)) {
							piecePos.push_back(make_unique<pair<int,int>>(startPos.first, startPos.second));
							break;
						}
						undo(false);
					}
				}
			}
		}
	}
	return piecePos;
}

bool Board::isMoveValid(shared_ptr<Piece> piece, pair<int,int> &newPos) {
	try {
		validateMove(piece, newPos);
		return true;
	} catch (...) {
		return false;
	}
}

int Board::getPieceIndexWithinPlayer(shared_ptr<Piece> piece, string player) {
	for (int i = 0; i < playerMap[player]->pieces.size(); i++) {
		if (playerMap[player]->pieces[i] == piece) {
			return i;
		}
	}
	return -1;
}

void Board::validateMove(shared_ptr<Piece> piece, pair<int,int> &newPos) {
	pair<int,int> oldCoords {piece->getX(), piece->getY()};
	if (isInBounds(newPos.first, newPos.second)) {
		if (piece->isPathValid(oldCoords, newPos)) {
			if (!isPathClear(piece, newPos)) {
				throw InvalidPath{};
			}
			if (isOccupied(newPos) && 
					(cells[newPos.second][newPos.first]->p->getColour() == piece->getColour())) {
				throw FriendlySpace{};
			} else if (auto pawnPiece = std::dynamic_pointer_cast<Pawn>(piece)) {
				bool isDoubleForward = (pawnPiece->doubleForward() == newPos);
				bool isDiagLeft = (pawnPiece->leftDiag() == newPos);
				bool isDiagRight = (pawnPiece->rightDiag() == newPos);

				if ((isDiagLeft || isDiagRight) && (!isOccupied(newPos))) {
					pair<int,int> targetPos = isDiagLeft ? pawnPiece->left() : pawnPiece->right();
					shared_ptr<Piece> targetPiece = getPiece(targetPos);
				
					if (auto otherPawn = std::dynamic_pointer_cast<Pawn>(targetPiece)){
						if (!otherPawn->didDoubleFirstMove() || !justMoved(targetPos)){
							throw BadEnPassant{};
						}
					} else {
						throw InvalidPath{};
					}
				} else if (isDoubleForward && !pawnPiece->hasMoved() && (isOccupied(newPos))) {
					throw InvalidPath{};
				}
			}
			unique_ptr<Board::Cell> tempTakeCell = make_unique<Board::Cell>(-1, -1);
			bool isTake = isOccupied(newPos);
			int takenIndex = -1;
			if (isTake) {
				if (!getPiece(newPos)->getIsInactive()) {
					takenIndex = getPieceIndexWithinPlayer(getPiece(newPos), getPiece(newPos)->getColour());
					removePieceFromPlayer(newPos, cells[newPos.second][newPos.first]->p->getColour());
				}
				cells[newPos.second][newPos.first]->movePiece(*tempTakeCell);
			}
			bool firstMatters = piece->doesFirstMoveMatter() && !piece->hasMoved();
			bool doubleFirstMatters = false;
			if (auto pawnPiece = dynamic_pointer_cast<Pawn>(piece)) {
				if (pawnPiece->didDoubleFirstMove()) {
					doubleFirstMatters = true;
				}
			}
			
			// this is only to see if the move will cause check, so state should 
			// 	revert back after this block.
			// movePiece should not throw since all conditions have now been 
			// 	validated (besides what is being checked)
			movePiece(oldCoords, newPos);
			setAllKingPos();
			
			// check if player is now in check after making move and updating king positions
			bool check = isInCheck(piece->getColour());
			
			// move piece back
			movePiece(newPos, oldCoords);
			
			// change the state back depending on conditions
			if (firstMatters) {
				piece->isFirstMove = true;
			}
			if (doubleFirstMatters) {
				static_pointer_cast<Pawn>(piece)->doubleFirstMove = true;
			} else if (auto pawn = dynamic_pointer_cast<Pawn>(piece)) {
				pawn->doubleFirstMove = false;
			}
			
			if (isTake) {
				tempTakeCell->movePiece(*cells[newPos.second][newPos.first]);
				if (takenIndex != -1) {
					vector<shared_ptr<Piece>>::iterator pieceIt = playerMap[getPiece(newPos)->getColour()]->pieces.begin();
					playerMap[getPiece(newPos)->getColour()]->pieces.insert(pieceIt + takenIndex, getPiece(newPos));
				}
			}
			setAllKingPos();
			if (!check) {
				return;
			} else {
				throw PiecePreventsCheck{};
			}
		}
		if (auto kingPiece = std::dynamic_pointer_cast<King>(piece)) {
			pair<int,int> rookPos {0,0};
			pair<int,int> finalRookPos {0,0};
			if (kingPiece->isCastling(newPos, rookPos, finalRookPos)) {
				if (auto rookPiece = std::dynamic_pointer_cast<Rook>(getPiece(rookPos))) {
					if (!rookPiece->hasMoved()) {
						pair<int,int> piecePos {piece->getX(), piece->getY()};
						string colour = rookPiece->getColour();
						if (isPathClear(getPiece(rookPos), finalRookPos)) {
							if ((!isTargeted(finalRookPos, colour, true)) &&
								(!isTargeted(newPos, colour, true)) && 
								(!isTargeted(piecePos, colour, true))) {
								return;
							} else {
								throw UnsafeCastle{};
							}
						}
						else {
							throw BadCastle{};
						}
					}
				} else {
					throw BadCastle{};
				}
			}
		}
		throw InvalidPath{};
	} else {
		throw OutOfBounds{};
	}
}

void Board::setAllKingPos() {
	for (auto &player: playerMap) {
		player.second->kingPos = pair<int,int>(-1,-1);
		for (auto &piece: player.second->pieces) {
			if (piece) {
				if (auto kingPiece = std::dynamic_pointer_cast<King>(piece)) {				
					player.second->kingPos = pair<int,int>(piece->getX(), piece->getY());
				}
			}
		}
	}
}

void Board::moveAndRenderPiece(pair<int,int> &start, pair<int,int> &end) {
	movePiece(start, end);
	removeGraphicPiece(start);
	addGraphicPiece(end);
}

void Board::movePiece(pair<int,int> &start, pair<int,int> &end) {
	getPiece(start)->move(end.first, end.second);
	cells[start.second][start.first]->movePiece(*cells[end.second][end.first]);
}

void Board::undo(bool shouldRender) {
	if (shouldRender) {
		renderGraphicText("Undoing...");
	}
	if (allMoves.empty()) {
		throw NoMoreUndos{};
	}
	unique_ptr<Move> lastMove = std::move(allMoves.back());
	allMoves.pop_back();
	string colour = getPiece(lastMove->endCoords)->getColour();
	if (lastMove->isPromotion) {
		removePiece(lastMove->endCoords, false, shouldRender);
	} else {
		if (shouldRender) {
			moveAndRenderPiece(lastMove->endCoords, lastMove->startCoords);
		} else {
			movePiece(lastMove->endCoords, lastMove->startCoords);
		}
		if (getPiece(lastMove->startCoords)->doesFirstMoveMatter() && lastMove->wasFirstMove) {
			getPiece(lastMove->startCoords)->isFirstMove = true;
		}
	}
	if (auto pawnPiece = std::dynamic_pointer_cast<Pawn>(getPiece(lastMove->startCoords))) {
		if (lastMove->wasDoubleFirstMove) {
			pawnPiece->doubleFirstMove = true;
		} else {
			pawnPiece->doubleFirstMove = false;
		}
	}
	if (lastMove->checkmatePieces.size()) {
		for (auto &matedPiece: lastMove->checkmatePieces) {
			pair<int,int> matedCoord = pair<int,int>(matedPiece->getX(), matedPiece->getY());
			playerMap[matedPiece->getColour()]->addPiece(matedPiece);
			matedPiece->isInactive = false;
			if (shouldRender) {
				addGraphicPiece(matedCoord);
			}
			matedPiece.reset();
		}
	}
	if (lastMove->takenPiece) {
		shared_ptr<Piece> taken = lastMove->takenPiece;
		pair<int,int> takenCoord = pair<int,int>(taken->getX(), taken->getY());
		cells[taken->getY()][taken->getX()]->assignPiece(taken);
		if (lastMove->takenIndex != -1) {
			playerMap[taken->getColour()]->pieces.insert(playerMap[taken->getColour()]->pieces.begin() + lastMove->takenIndex, taken);
		}
		if (shouldRender) {
			addGraphicPiece(takenCoord);
		}
		lastMove->takenPiece.reset();
	}
	if (lastMove->isPromotion) {
		shared_ptr<Piece> taken = lastMove->promotedPawn;
		pair<int,int> takenCoord = pair<int,int>(taken->getX(), taken->getY());
		cells[taken->getY()][taken->getX()]->assignPiece(taken);
		playerMap[taken->getColour()]->pieces.insert(playerMap[taken->getColour()]->pieces.begin() + lastMove->promotionIndex, taken);
		if (shouldRender) {
			addGraphicPiece(takenCoord);
		}
		lastMove->promotedPawn.reset();
	}
	if (lastMove->isCastle) {
		undo(shouldRender);
		return;
	}
	clearGraphicText();
	if (shouldRender && isInCheck(colour)) {
		renderGraphicText(string(1, (char) toupper(colour[0])) + 
				colour.substr(1, colour.length() - 1) + " is in check.");
	}
}

vector<pair<int,int>> Board::controlledSquares(string player) {
	vector<pair<int,int>> squares;
	for (int j = 0; j < playerMap[player]->pieces.size(); j++) {
		shared_ptr<Piece> piece = playerMap[player]->pieces[j];
		for (auto &move: piece->generatePossibleMoves()) {
			pair<int,int> endCoord = pair<int,int>(move->first,move->second);
			if (!(count(squares.begin(), squares.end(), endCoord)) && 
					!(isTargeted(endCoord, player))) {
				squares.emplace_back(endCoord.first, endCoord.second);
			}
		}
	}
	return squares;
}
