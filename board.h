#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "window.h"

class Piece;
class Player;
class Move;

using std::string;
using std::vector;
using std::map;
using std::unique_ptr;
using std::pair;
using std::shared_ptr;

class Board: public std::enable_shared_from_this<Board> {
    public:	
	friend class ComputerPlayerDiff1;
	friend class ComputerPlayerDiff2;
	friend class ComputerPlayerDiff3;
	friend class ComputerPlayerDiff4;
        static const int TWO_PLAYER_DIMENSION;
        static const int FOUR_PLAYER_DIMENSION;
        static const string FOUR_PLAYER_VALID_COLOUR_CHARS;
        static const string TWO_PLAYER_VALID_PIECE_CHARS;
        static const string TWO_PLAYER_VALID_PROMO_PIECE_CHARS;
        static const string FOUR_PLAYER_VALID_PIECE_CHARS;

        class Cell {
            public:
                static const char WHITE_SPACE = ' ';
                static const char BLACK_SPACE = '_';
                static const char OUT_OF_BOUNDS_SPACE = '/';
                
                const int x, y;
                const char emptyChar;
                const bool isFourPlayer;
                const bool isOutOfBounds;

                Cell(int x, int y, bool isFourPlayer = 0);
                bool isEmpty() const;
                char getChar() const;
                void movePiece(Cell &other);
                Piece *getPiece() const;
                friend Board;
                friend std::ostream &operator<< (std::ostream &out, Board::Cell &cell);
            private:
                shared_ptr<Piece> p;
                void assignPiece(shared_ptr<Piece> newPiece);
        };

        bool const isFourPlayer;
        bool shouldDrawByInsufficient;

        Board(map<string, shared_ptr<Player>> &playerMap, int dimension);
        virtual ~Board();
        Cell *getCell(int x, int y) const;
        void render() const;
        void addPiece(string pieceType, pair<int,int> coord, bool boardSetup = 0, bool shouldRender = true);
        void removePiece(pair<int,int> coord, bool isCheckmate = false, bool shouldRender = true);
        void removePieceFromPlayer(pair<int,int> coord, string colour);
        void defaultSetup();
        bool isInBounds(int x, int y) const;
        int maxWidth() const;
        int maxHeight() const;
        bool isOccupied(pair<int,int> &coord) const;
        bool isEmpty() const;
        bool isInCheck(string player);
	int lastMoveTakenVal();
	pair<pair<int,int>,pair<int,int>> lastMadeMove();
	vector<shared_ptr<pair<int,int>>> enemyCanCheckWith(string playerToCheck);
	int movesCount();
        bool isPathClear(shared_ptr<Piece> piece, pair<int,int> &newPos) const;
        bool isMoveValid(shared_ptr<Piece> piece, pair<int,int> &newPos);
        void removePlayerPieces(string player, bool isCheckmate = false);
        void move(pair<int,int> &start, pair<int, int> &end, string &colour, string promotionPiece = "", bool shouldRender = true);
        shared_ptr<Piece> getPiece(pair<int,int> &coord) const;
        int isTargeted(pair<int,int> &coord, string targetedColour, bool isCheck = false);
	vector<shared_ptr<Piece>> piecesTargeting(pair<int,int> &coord, string targetedColour, bool isCheck);	
        void setAllKingPos();
        bool isCheckmate(string player);
        bool isStalemate(string player);
        bool isInsufficientMaterial();
	bool isInvalidCoord(pair<int,int> &coord) const;
        void undo(bool shouldRender = true);
        void movePiece(pair<int,int> &start, pair<int,int> &end);
        void moveAndRenderPiece(pair<int,int> &start, pair<int,int> &end);
        void validateMove(shared_ptr<Piece> piece, pair<int,int> &newPos);
        void addGraphicPiece(pair<int,int> &coord);
        void removeGraphicPiece(pair<int,int> &coord);
        void drawInactivePiece(pair<int,int> &coord);
        void zoom(int factor);
        void renderGraphicText(string message);
        void clearGraphicText();
        void enableDrawByInsufficient(bool newState);
	vector<shared_ptr<Piece>> piecesCanReach(pair<int,int> &coord, string movingColour); 
        vector<pair<int,int>> controlledSquares(string player);
	bool justMoved(pair<int,int> &coord);
        bool wasLastMoveCheckmate(string colour);
        void clearBoardPieces();
    private:
        unique_ptr<Xwindow> w;
        int graphicSquareDim;
        int graphicZoomFactor;
        int margin;
        int yMargin;
        int vCellMargin;
        int hCellMargin;
        vector<vector<unique_ptr<Cell>>> cells;
        const int dimension;
        map<string, shared_ptr<Player>> playerMap;
        vector<unique_ptr<Move>> allMoves;

        void addPiece(char pieceType, int x, int y, bool boardSetup, bool shouldRender = true);
        shared_ptr<Piece> createPiece(int x, int y, string colour, char pieceType, bool boardSetup);
        shared_ptr<Piece> getSingleAttacker(pair<int,int> &coord) const;
        bool isReachable(pair<int,int> &coord, string movingColour);
        void renderGraphicBoard();
        void renderGraphicPieces();
        int getPieceIndexWithinPlayer(shared_ptr<Piece> piece, string colour);

};

std::ostream &operator<< (std::ostream &out, Board::Cell &cell);

class InvalidMove {
    public:
    virtual string message() {return "Move invalid.";}
};

class FriendlySpace: public InvalidMove {
    public:
    string message() override {return "Move invalid. Space already occupied by you.";}
};

class InvalidPath: public InvalidMove {
    public:
    string message() override {return "Move invalid. Path is either unreachable or obstructed.";}
};

class BadEnPassant: public InvalidMove {
    public:
    string message() override {return "Move invalid. En passant conditions not met.";}
};

class PiecePreventsCheck: public InvalidMove {
    public:
    string message() override {return "Move invalid. Move will put you in check.";}
};

class UnsafeCastle: public InvalidMove {
    public:
    string message() override {return "Move invalid. Castle puts King or Rook in unsafe space.";}
};

class BadCastle: public InvalidMove {
    public:
    string message() override {return "Move invalid. Castle conditions not met.";}
};

class WrongPlayer:public InvalidMove {
    public:
    virtual string message() {return "Move invalid. Not your piece.";}
};

class PieceNotFound: public InvalidMove {
    public:
    virtual string message() {return "Move invalid. Piece not found.";}
};

class BadPromotionPiece: public InvalidMove {
    public:
    virtual string message() {return "Move invalid. Specified promotion piece invalid.";}
};

class OutOfBounds: public InvalidMove {
    public:
    virtual string message() {return "Move invalid. Square is out of bounds.";}
};

class NoMoreUndos {
    public:
    string message() {return "Undo invalid. No move to undo.";}
};

#endif
