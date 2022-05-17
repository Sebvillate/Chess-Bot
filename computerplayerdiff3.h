#ifndef CPU3_H
#define CPU3_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "computerplayerdiff2.h"
using namespace std;

class ComputerPlayerDiff3: public ComputerPlayerDiff2 {
    vector<shared_ptr<Piece>> threatenedPieces(shared_ptr<Board> board);	
    bool newMaxValPiece(vector<shared_ptr<Piece>> & oldPieceVec, vector<shared_ptr<Piece>> & newPieceVec, int pieceMovedVal);
    public: 
    ComputerPlayerDiff3(const string &colour);    
    virtual pair<pair<int, int>, pair<int, int>> chooseMove(shared_ptr<Board> board, string &promotionPiece) override;
};
#endif
