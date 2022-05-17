#ifndef CPU4_H
#define CPU4_H
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include "computerplayerdiff3.h"
using namespace std;

class ComputerPlayerDiff4: public ComputerPlayerDiff3 {
    private:
    vector<vector<pair<pair<int,int>, pair<int,int>>>> openings;
    bool goToDiff3(shared_ptr<Board> board, string &promotionPiece);

    vector<vector<pair<pair<int,int>, pair<int,int>>>> setups;
    public:
    ComputerPlayerDiff4(const string &colour);    
    virtual pair<pair<int, int>, pair<int, int>> chooseMove(shared_ptr<Board> board, string &promotionPiece) override;
};
#endif
