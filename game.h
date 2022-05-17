#ifndef GAME_H
#define GAME_H
#include <utility>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "board.h"
#include "computerplayer.h"
#include "player.h"
using namespace std;

class Game {

    static vector<string> colourOptions;
    int numPlayers;
    map<string, shared_ptr<Player>> players;

    public:
    map<string, shared_ptr<ComputerPlayer>> computerPlayers;
    shared_ptr<Board> gameBoard;
    vector<string> playersInGame;

    Game(int numPlayers,
		    map<string, shared_ptr<Player>> players = map<string, shared_ptr<Player>>(),
		    map<string, shared_ptr<ComputerPlayer>> computerPlayers = map<string, shared_ptr<ComputerPlayer>>(),
		    vector<string> playersInGame = vector<string>());
    void makePlayer();
    void createComputerPlayer(int level, const string &colour);
    void updateScore(const string &colour);
    void printFinalScore();
    int getNumPlayers() const;
    void setNumPlayers(int n);
    bool isComputerPlayer(const string &colour) const;
    void addPiece(string &pieceType, string &coord);
    void removePiece(string &coord);
    pair<int, int> convertCoordinates(string &coord) const;
    int makeMove(pair<int, int> &start, pair<int, int> &end, string &currentTurn, int currentInt, string promotionPiece = "");
    void resetBoard();
    void resetComputerPlayers();
    bool exitSetup();
    void resetPlayersInGame();
    int undo(int times, int currentInt);
};

class InvalidInput {
    public:
    string message() {return "Invalid Input.";}
};

bool reorderColours(string i, string j);

#endif
