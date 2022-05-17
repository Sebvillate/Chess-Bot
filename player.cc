#include "player.h"

Player::Player(int score, vector<shared_ptr<Piece>> pieces, pair<int, int> kingPos)
	: score{score},
	pieces{pieces},
	kingPos{kingPos}
{}

int Player::getScore() const {
	return score;
}

void Player::setScore(int newScore) {
	score = newScore;
}

vector<shared_ptr<Piece>> Player::getPieces() const {
	return pieces;
}

void Player::addPiece(shared_ptr<Piece> newPiece) {
	pieces.emplace_back(newPiece);
}
