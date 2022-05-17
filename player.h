#ifndef PLAYER_H
#define PLAYER_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "piece.h"
using namespace std;
class Player {
    int score;
    vector<shared_ptr<Piece>> pieces;
    pair<int, int> kingPos;
    public:
    Player(int score = 0,
		    vector<shared_ptr<Piece>> pieces = vector<shared_ptr<Piece>>(),
		    pair<int, int> kingPos = pair<int, int>());
    int getScore() const;
    void setScore(int newScore);
    vector<shared_ptr<Piece>> getPieces() const;
    void addPiece(shared_ptr<Piece> newPiece);
    friend class Board;
    friend class ComputerPlayerDiff1;
    friend class ComputerPlayerDiff2;
    friend class ComputerPlayerDiff3;
    friend class ComputerPlayerDiff4;
};
#endif
