#ifndef CPU1_H
#define CPU1_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "computerplayer.h"
using namespace std;

class ComputerPlayerDiff1: public ComputerPlayer {
    public:
    ComputerPlayerDiff1(const string &colour);    
    virtual pair<pair<int, int>, pair<int, int>> chooseMove(shared_ptr<Board> board, string &promotionPiece) override;
};
#endif
