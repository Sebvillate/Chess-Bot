#include <iostream>
#include <algorithm>
#include "game.h"
#include "computerplayer.h"
#include "player.h"
#include "board.h"
#include "computerplayerdiff1.h"
#include "computerplayerdiff2.h"
#include "computerplayerdiff3.h"
#include "computerplayerdiff4.h"

vector<string> Game::colourOptions = {"green", "red", "black", "white"};

// Game constructor
Game::Game(int numPlayers, map<string, shared_ptr<Player>> players,
		map<string, shared_ptr<ComputerPlayer>> computerPlayers,
		vector<string> playersInGame)
	: numPlayers{numPlayers},
	players{players},
	computerPlayers{computerPlayers},
	playersInGame{playersInGame}
{
	if (numPlayers == 4) {
		for (int i = 0; i < 4; ++i) {
			makePlayer();
		}
	} else {
		for (int i = 0; i < 2; ++i) {
                        makePlayer();
                }
	}
	gameBoard = make_shared<Board>(this->players, ((numPlayers == 4) ? Board::FOUR_PLAYER_DIMENSION : Board::TWO_PLAYER_DIMENSION));
}

// initializes a new Player and assigns it a colour that's not in use
void Game::makePlayer() {
	string colour = colourOptions.back();
	colourOptions.pop_back();

	players[colour] = make_shared<Player>();
	playersInGame.emplace_back(colour);
}

// initializes a new Computer Player and assigns it a colour that's not in use
void Game::createComputerPlayer(int level, const string &colour) {
	if (level == 1) {
		computerPlayers[colour] =
		       	make_shared<ComputerPlayerDiff1>(colour);
	} else if (level == 2) {
                computerPlayers[colour] =
		       	make_shared<ComputerPlayerDiff2>(colour);
        } else if (level == 3) {
                computerPlayers[colour] =
		       	make_shared<ComputerPlayerDiff3>(colour);
        } else if (level == 4) {
                computerPlayers[colour] =
		       	make_shared<ComputerPlayerDiff4>(colour);
        }
}

// updates the score for a player
void Game::updateScore(const string &colour) {
	shared_ptr<Player> player = players[colour];
	player->setScore(player->getScore() + 1);
}

bool Game::isComputerPlayer(const string &colour) const {
	if (computerPlayers.count(colour) != 0) {
		return true;
	}
	return false;
}

void Game::printFinalScore() {
	cout << "Final Score:" << endl;

        cout << "White: " << players["white"]->getScore() << endl;
        cout << "Black: " << players["black"]->getScore() << endl;
	if (numPlayers == 4) {
		cout << "Red: " << players["red"]->getScore() << endl;
        	cout << "Green: " << players["green"]->getScore() << endl;
	}
}

int Game::getNumPlayers() const {
	return numPlayers;
}

void Game::setNumPlayers(int n) {
	numPlayers = n;
}

// adds a piece to the board
void Game::addPiece(string &pieceType, string &coord) {
	pair<int, int> cell = convertCoordinates(coord);
	gameBoard->addPiece(pieceType, cell, true);
}

// removes a piece from the board
void Game::removePiece(string &coord) {
	pair<int, int> cell = convertCoordinates(coord);
	gameBoard->removePiece(cell);
}

// converts chess cell input to integer coordinates
pair<int, int> Game::convertCoordinates(string &coord) const {
	pair<int, int> integerCoord;
	if ((coord.length() < 2) || ((coord.length() > 2) && (players.size() != 4))) {
		throw InvalidInput{};
	}
	try {
		int x = coord[0] - 'a';
		int y;
		if (coord.length() == 2) {
			y = (gameBoard->maxWidth() + 1) - (coord[1] - '0');
		} else {
			string coordNum = coord.substr(1, 2);
			y = (gameBoard->maxWidth() + 1) - ((coordNum[0] - '0') * 10 + (coordNum[1] - '0'));
		}
		integerCoord = pair<int, int>(x, y);
		return integerCoord;
	} catch (...) {
		throw InvalidInput{};
	}
}

// moves a piece from start to end if it is a valid move
int Game::makeMove(pair<int, int> &start, pair<int, int> &end, string &currentTurn, int currentInt, string promotionPiece) {
	try {
		gameBoard->move(start, end, currentTurn, promotionPiece);
	} catch (InvalidMove &e) {
		throw;
	}

	for (auto &player : players){
		if (player.first != currentTurn){
			if ((gameBoard->isCheckmate(player.first)) && (count(playersInGame.begin(), playersInGame.end(), player.first) != 0)) {
				gameBoard->removePlayerPieces(player.first, true);
                                int i;
			        for (i = 0; i < numPlayers; ++i) {
                        		if (playersInGame[i] == player.first) {
                        	        	break;
                        		}
                		}
				playersInGame.erase(playersInGame.begin() + i);
                		numPlayers -= 1;
				if (reorderColours(player.first, currentTurn)) {
					currentInt -= 1;
				}
			} else if ((gameBoard->isStalemate(player.first) || (gameBoard->shouldDrawByInsufficient && gameBoard->isInsufficientMaterial())) && 
					(count(playersInGame.begin(), playersInGame.end(), player.first) != 0)) {
                                for (int j = 0; j < numPlayers; ++j) {
                                        gameBoard->removePlayerPieces(playersInGame[j]);
				}
				playersInGame.clear();
				numPlayers = -1;
                        }

		}
	}
	return currentInt;
}

// resets the board and the player's pieces
void Game::resetBoard() {
	gameBoard->clearGraphicText();
	computerPlayers = map<string, shared_ptr<ComputerPlayer>>();	
	for (int i = 0; i < numPlayers; ++i) {
		gameBoard->removePlayerPieces(playersInGame[i]);
	}
	gameBoard->clearBoardPieces();
}

void Game::resetComputerPlayers() {
	computerPlayers = map<string, shared_ptr<ComputerPlayer>>();
}

// return true if the setup is valid, false otherwise
bool Game::exitSetup() {
	const string colours[4] = {"white", "black", "red", "green"};
	int kingCounts[4] = {0, 0, 0, 0};
	for (int i = 0; i < numPlayers; ++i) {

		shared_ptr<Player> player = players[colours[i]];
		int kingCounts[4] = {0, 0, 0, 0};
		
		for (auto p : player->getPieces()) {

			if ((p->getPieceType() == 'k') || (p->getPieceType() == 'K')) {
				kingCounts[i] += 1;
			} else if ((p->getPieceType() == 'p') || (p->getPieceType() == 'P')) {
				
				if ((numPlayers == 2) && ((p->getY() == 0) || (p->getY() == 7))) {
					// Invalid if a player's pawn is on the first/last row
					cout << colours[i] << " player has a pawn on the first/last row of the board."
						<< " Invalid setup." << endl;
					return false;
				} else if ((numPlayers == 4) && ((p->getY() == 13) || (p->getY() == 0) ||
					       	(p->getX() == 0) || (p->getX() == 13))) {
					// Invalid if a player's pawn is on the first/last row
					cout << colours[i] << " player has a pawn on the first/last row of the board."
                                                << " Invalid setup." << endl;
                                        return false;
				}

			}

			if (kingCounts[i] > 1) {
				// Invalid if there's more then one king for a player
				cout << colours[i] << " player has too many kings on the board." 
					<< " Maximum of one king is allowed per player." << endl;
				return false;
			}
		}
		if (kingCounts[i] < 1) {
                        // Invalid if a player has no kings
                        cout << colours[i] << " player does not have a king on the board."
                                << " Must have one king per player." << endl;
                        return false;
		} else if (kingCounts[i] == 1) {
			gameBoard->setAllKingPos();
			if (gameBoard->isInCheck(colours[i])) {
				// Invalid if a player's king is in check
				cout << colours[i] << " player's king is in check."
					<< " Invalid setup." << endl;
				return false;
			}
		}
	}
	return true;
}

void Game::resetPlayersInGame() {
	const string colours[4] = {"white", "black", "red", "green"};
	playersInGame.clear();
	for (int i = 0; i < numPlayers; ++i) {
		playersInGame.emplace_back(colours[i]);
	}
}

bool reorderColours(string i, string j) {
	map<string, int> colourOrder;
	colourOrder["white"] = 0;
	colourOrder["black"] = 1;
	colourOrder["red"] = 2;
	colourOrder["green"] = 3;
	return (colourOrder[i] < colourOrder[j]);
}

int Game::undo(int times, int currentInt) {
	for (int i = 0; i < times; i++) {
		for (auto &player : players) {
			if (gameBoard->wasLastMoveCheckmate(player.first)) {
				string currentTurn = playersInGame[(currentInt - 1) % numPlayers];		
				playersInGame.emplace_back(player.first);
                                numPlayers += 1;
				if (reorderColours(player.first, currentTurn)) {
                                        currentInt += 1;
                                }
			} 
		}
		try {
			gameBoard->undo();
			sort(playersInGame.begin(), playersInGame.end(), reorderColours);
			if (currentInt >= 1) {
						currentInt -= 1;
				} else {
						currentInt = numPlayers - 1;
				}
		} catch (NoMoreUndos &e) {
			cout << e.message() << endl;
			gameBoard->renderGraphicText(e.message());
		}
	}
	return currentInt;
}

