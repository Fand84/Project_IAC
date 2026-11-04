#include "Board.h"

Board::Board(double laneLength, int castleHp) noexcept
    : laneLength_(laneLength),
    playerCastle_(PlayerSide::Player, castleHp),
    computerCastle_(PlayerSide::Computer, castleHp) {
}

double Board::getLaneLength() const noexcept {
    return laneLength_;
}

double Board::getPlayerSpawnX() const noexcept {
    return 0.0;
}

double Board::getComputerSpawnX() const noexcept {
    return laneLength_;
}

const Castle& Board::getPlayerCastle() const noexcept {
    return playerCastle_;
}

const Castle& Board::getComputerCastle() const noexcept {
    return computerCastle_;
}

Castle& Board::getPlayerCastle() noexcept {
    return playerCastle_;
}

Castle& Board::getComputerCastle() noexcept {
    return computerCastle_;
}

bool Board::isGameOver() const noexcept {
    return playerCastle_.isDestroyed() || computerCastle_.isDestroyed();
}

bool Board::isPlayerWinner() const noexcept {
    return computerCastle_.isDestroyed() && !playerCastle_.isDestroyed();
}

bool Board::isComputerWinner() const noexcept {
    return playerCastle_.isDestroyed() && !computerCastle_.isDestroyed();
}

void Board::reset() noexcept {
    playerCastle_.reset();
    computerCastle_.reset();
}