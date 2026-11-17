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

double Board::getPlayerEnergy() const noexcept {
    return playerEnergy_;
}

double Board::getMaxEnergy() const noexcept {
    return maxEnergy_;
}

bool Board::spendPlayerEnergy(double cost) noexcept {
    if (cost < 0.0 || playerEnergy_ < cost) {
        return false;
    }
    playerEnergy_ -= cost;
    return true;
}

void Board::updateEnergy(double deltaTime) noexcept {
    if (deltaTime > 0.0 && !isGameOver()) {
        playerEnergy_ = std::min(maxEnergy_, playerEnergy_ + energyRegenRate_ * deltaTime);
    }
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
    playerEnergy_ = 5.0;
}