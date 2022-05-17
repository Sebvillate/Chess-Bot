#ifndef CPU2_H
#define CPU2_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "computerplayerdiff1.h"
using namespace std;

class ComputerPlayerDiff2: public ComputerPlayerDiff1 {
    public:	
    ComputerPlayerDiff2(const string &colour);    
    virtual pair<pair<int, int>, pair<int, int>> chooseMove(shared_ptr<Board> board, string &promotionPiece) override;
};
#endif
