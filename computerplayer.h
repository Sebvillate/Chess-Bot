#ifndef CPU_H
#define CPU_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "board.h"
using namespace std;

class ComputerPlayer {
    protected:
    string colour;
    public:
    ComputerPlayer(const string &colour);    
    virtual pair<pair<int, int>, pair<int, int>> chooseMove(shared_ptr<Board> board, string &promotionPiece) = 0;
};
#endif
