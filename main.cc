#include <iostream>
#include <sstream>
#include "game.h"

class InvalidPlayer {
        public:
        virtual string message() {return "Invalid Player Initialization. Must be from human or computer[1-4].";}
};

class InvalidPlayerAmount {
        public:
        virtual string message() {return "Invalid Player Amount.";}
};

int advanceTurn(int i, int numPlayers) {
	if (i <= numPlayers - 2) {
		i += 1;
	} else {
		i = 0;
	}
	return i;
}

int main() {
	unique_ptr<Game> game;
	int n;
	while (true) {
		cout << "Please enter the number of players (2 or 4): ";
		if ((cin >> n) && ((n == 2) || (n == 4))) {
			game = make_unique<Game>(Game(n));
			cout << endl;
			break;
		} else if (cin.fail() && ! cin.eof()) {
			cin.clear();
			cin.ignore();
		}
		cout << endl;
		if (cin.eof()) {
			return 0;
		}
	}

	string line;
	const string playerColours[4] = {"white", "black", "red", "green"};
	int i = 0;
	string currentTurn = playerColours[i];

	while (getline(cin, line)) {
		if (cin.eof()) {
			break;
		}
		string c;
		istringstream s{line};
		while (s >> c) {
			if (c == "setup") {
				// setup has to be done when a game is not running
				game->gameBoard->render();
				string setupLine;
				while (getline(cin, setupLine)) {
					string x;
					istringstream s2{setupLine};

					s2 >> x;
					if (x == "+") {
						try {
							string pieceType;
							string coord;
							s2 >> pieceType;
							s2 >> coord;
							if (!(((pieceType.length() == 1) && (game->getNumPlayers() == 2)) ||
								((pieceType.length() == 2) && (game->getNumPlayers() == 4)))) {
								throw InvalidInput{};
							}
							game->addPiece(pieceType, coord);
						} catch (InvalidInput &e) {
							cout << e.message() << endl;
						}
					} else if (x == "-") {
						try { 
							string coord;
							s2 >> coord;
							game->removePiece(coord);
						} catch (InvalidInput &e) {
							cout << e.message() << endl;
						}
					} else if (x == "=") {
						string colour;
						s2 >> colour;
						if ((n == 2) && (colour == "white")) {
							currentTurn = colour;
							i = 0;
							cout << "It is " << currentTurn << " player's turn" << endl;
						} else if ((n == 2) && (colour == "black")) {
                                                        currentTurn = colour;
                                                        i = 1;
                                                        cout << "It is " << currentTurn << " player's turn" << endl;
						} else if ((n == 4) && (colour == "white")) {
                                                        currentTurn = colour;
                                                        i = 0;
                                                        cout << "It is " << currentTurn << " player's turn" << endl;
						} else if ((n == 4) && (colour == "black")) {
                                                        currentTurn = colour;
                                                        i = 1;
                                                        cout << "It is " << currentTurn << " player's turn" << endl;
						} else if ((n == 4) && (colour == "red")) {
                                                        currentTurn = colour;
                                                        i = 2;
                                                        cout << "It is " << currentTurn << " player's turn" << endl;
						} else if ((n == 4) && (colour == "green")) {
                                                        currentTurn = colour;
                                                        i = 3;
                                                        cout << "It is " << currentTurn << " player's turn" << endl;
						} else {
							cout << "Invalid input. It is " << currentTurn << " player's turn" << endl;
						}	       
					} else if (x == "clear") {
					 	game->resetBoard();
					} else if (x == "zoom") {
                                                int z;
                                                if (s2 >> z) {
                                                        game->gameBoard->zoom(z);
                                                } else {
                                                        cout << "Invalid zoom factor. Enter an integer.";
                                                }
					} else if (x == "done") {
						if (game->exitSetup()) {
							game->gameBoard->enableDrawByInsufficient(!game->gameBoard->isInsufficientMaterial());
							break;
						}
					} else {
						cout << "Invalid command" << endl;
					}
					game->gameBoard->render();
				}
			} else if (c == "game") {
				if (game->gameBoard->isEmpty()) {
					game->gameBoard->defaultSetup();
				}
				int colourPos = 0;

				try {
					int countInputPlayers = 0;
					while (s >> c) {
						if ((c == "computer[1]") || (c == "computer[2]") ||
							(c == "computer[3]") || (c == "computer[4]")) {
							int level = (int) c[9] - '0';
							game->createComputerPlayer(level, playerColours[colourPos]);
						} else if (c != "human") {
							throw InvalidPlayer{};
						}
						colourPos += 1;
						countInputPlayers += 1;
					}
					if (countInputPlayers != game->getNumPlayers()) {
						throw InvalidPlayerAmount{};
					}
					game->gameBoard->render();
				} catch (InvalidPlayer &e) {
					cout << e.message() << endl;
					game->resetComputerPlayers();
					break;
				} catch (InvalidPlayerAmount &e) {
					cout << e.message() << endl;
					game->resetComputerPlayers();
					break;
				}
				string gameLine;
				while (getline(cin, gameLine)) {
					string g;
					istringstream s2{gameLine};

					s2 >> g;
					if (g == "move") {
						game->gameBoard->clearGraphicText();
						try {
							if (game->isComputerPlayer(currentTurn)) {
								// default pawn promotion piece is q
								string promotionPiece = "q";
								pair<pair<int, int>, pair<int, int>> chosenMove = 
									game->computerPlayers[currentTurn]->chooseMove(game->gameBoard, promotionPiece);
								i = game->makeMove(chosenMove.first, chosenMove.second, currentTurn, i, promotionPiece);
							} else {
								string from;
								string to;
								s2 >> from;
								s2 >> to;
		
								pair<int, int> startCoord = game->convertCoordinates(from);
								pair<int, int> endCoord = game->convertCoordinates(to);
								if (s2 >> g) {
									// pawn promotion
									i = game->makeMove(startCoord, endCoord, currentTurn, i, g);
								} else {
									i = game->makeMove(startCoord, endCoord, currentTurn, i);
								}
							}
							if (game->getNumPlayers() == 1) {
                                	                	cout << (char) toupper(game->playersInGame[0][0]) << 
									game->playersInGame[0].substr(1, game->playersInGame[0].length() - 1) << 
									" wins!" << endl;
                        					game->gameBoard->renderGraphicText("Checkmate! " + 
										string(1, (char) toupper(game->playersInGame[0][0])) + 
										game->playersInGame[0].substr(1, game->playersInGame[0].length() - 1) + 
										" wins!");
                                	                	game->updateScore(game->playersInGame[0]);
                                	                	i = 0;
                                	                	game->resetBoard();
								game->setNumPlayers(n);
                                	                        game->resetPlayersInGame();
                                	                        currentTurn = playerColours[i];
                                	                	break;
							} else if (game->getNumPlayers() == 0) {
                                	                        cout << "Draw!" << endl;
								game->gameBoard->renderGraphicText("Draw!");
								i = 0;
                                	                        game->resetBoard();
                                	                        game->setNumPlayers(n);
                                	                        game->resetPlayersInGame();
                                	                        currentTurn = playerColours[i];
                                	                        break;
                                	                } else if (game->getNumPlayers() == -1) {
                                                                i = 0;
                                                                game->resetBoard();
                                                                game->setNumPlayers(n);
                                                                game->resetPlayersInGame();
                                                                currentTurn = playerColours[i];
                                                                break;
                                                        }

							game->gameBoard->render();
							i = advanceTurn(i, game->getNumPlayers());
                                        	        currentTurn = game->playersInGame[i];
						} catch (InvalidMove &e) {
                                                        cout << e.message() << endl;
                                                        game->gameBoard->render();
							game->gameBoard->renderGraphicText(e.message());
                                                } catch (InvalidInput &e) {
							cout << e.message() << endl;
							game->gameBoard->render();
							game->gameBoard->renderGraphicText(e.message());
						}
					} else if (g == "resign") {
						string resignedColour = game->playersInGame[i];
						string tempNextColour = game->playersInGame[advanceTurn(i, game->getNumPlayers())];
						
						game->setNumPlayers(game->getNumPlayers() - 1);
						game->gameBoard->removePlayerPieces(game->playersInGame[i]);
						game->playersInGame.erase(game->playersInGame.begin() + i);
						if (game->getNumPlayers() == 1) {
							cout << (char) toupper(game->playersInGame[0][0]) <<
                                                                        game->playersInGame[0].substr(1, game->playersInGame[0].length() - 1) << 
									" wins!" << endl;
							game->updateScore(game->playersInGame[0]);
							game->resetBoard();
							game->setNumPlayers(n);
							game->resetPlayersInGame();
							i = 0;
							currentTurn = playerColours[i];
							break;
						}
						if (reorderColours(resignedColour, tempNextColour)) {
                                                       i -= 1;
                                                }
						i = advanceTurn(i, game->getNumPlayers());
						currentTurn = game->playersInGame[i];
					} else if (g == "undo") {
						int numOfUndos;

						if (s2 >> numOfUndos) {
							bool undo = true;
                                                	for (int j = 0; j < game->getNumPlayers(); ++j) {
                                                	        if (!(game->isComputerPlayer(game->playersInGame[j]))) {
                                                	                cout << "Does " << game->playersInGame[j] << " player agree to undo " 
										<< numOfUndos << " moves? (type 'yes' to agree): " << endl;
                                                	                string response;
                                                	                cin >> response;
                                                	                if (response != "yes") {
                                                	                        undo = false;
                                                	                        cout << "Not all players agreed to undo " << numOfUndos << " moves.";
                                                	                        i = advanceTurn(i, game->getNumPlayers());
                                                	                        currentTurn = game->playersInGame[i];
                                                	                        cout << " It is " << currentTurn << " player's turn." << endl;
                                                	                        break;
                                                	                }
                                                	        }
                                                	}

                                                	if (undo) {
								cout << "Undoing " << numOfUndos << " moves." << endl;
								i = game->undo(numOfUndos, i);
								currentTurn = game->playersInGame[i];
								game->gameBoard->render();
							}
						}
					} else if (g == "draw") {
						bool draw = true;
						for (int j = 0; j < game->getNumPlayers(); ++j) {
							if (!(game->isComputerPlayer(game->playersInGame[j]))) {
								cout << "Does " << game->playersInGame[j] << 
									" player agree to tie the game? (type 'yes' to agree): ";
								string response;
								cin >> response;
								if (response != "yes") {
									draw = false;
									cout << "Not all players agreed to tie the game.";
                                                			i = advanceTurn(i, game->getNumPlayers());
                                                			currentTurn = game->playersInGame[i];
									cout << " It is " << currentTurn << " player's turn." << endl;
									break;
								}
							}
						}
						if (draw) {
							cout << "Draw!" << endl;
                                                        i = 0;
                                                        game->resetBoard();
                                                        game->setNumPlayers(n);
                                                        game->resetPlayersInGame();
                                                        currentTurn = playerColours[i];
							break;
                                                }
					} else if (g == "zoom") {
						int z;
						if (s2 >> z) {
							game->gameBoard->zoom(z);
						} else {
							cout << "Invalid zoom factor. Enter an integer.";
						}
					}
				}
			}	
		}
	}
	game->setNumPlayers(n);
	game->printFinalScore();
}
