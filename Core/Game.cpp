#include "Game.h"

Game::Game(double laneLength, int castleHp) noexcept
    : board_(laneLength, castleHp) {
}

void Game::update(double deltaTime) {
    if (!isRunning_ || board_.isGameOver()) {
        return;
    }

    
    if (playerEnergy_ < maxEnergy_) {
        playerEnergy_ = std::min(maxEnergy_, playerEnergy_ + energyRegenRate_ * deltaTime);
    }

    for (const auto& unit : units_) {
        if (unit && unit->isAlive()) {
            unit->move(deltaTime);
        }
    }

    std::erase_if(units_, [](const std::unique_ptr<Unit>& unit) {
        return !unit || !unit->isAlive();
        });

    if (board_.isGameOver()) {
        isRunning_ = false;
    }
}

bool Game::canPlaceUnit(int cost) const noexcept {
    return isRunning_ && (playerEnergy_ >= static_cast<double>(cost));
}

bool Game::spawnUnit(UnitType type, PlayerSide side, Lane lane, const UnitStats& stats) {
    if (side == PlayerSide::Player) {
        if (!canPlaceUnit(stats.cost)) {
            return false;
        }
        playerEnergy_ -= static_cast<double>(stats.cost);
    }

    const double startX = (side == PlayerSide::Player)
        ? board_.getPlayerSpawnX()
        : board_.getComputerSpawnX();

    units_.push_back(std::make_unique<Unit>(
        type,
        side,
        static_cast<int>(lane),
        startX,
        stats
    ));

    return true;
}

double Game::getPlayerEnergy() const noexcept {
    return playerEnergy_;
}

double Game::getMaxEnergy() const noexcept {
    return maxEnergy_;
}

const Board& Game::getBoard() const noexcept {
    return board_;
}

const std::vector<std::unique_ptr<Unit>>& Game::getUnits() const noexcept {
    return units_;
}

bool Game::isGameOver() const noexcept {
    return board_.isGameOver();
}

void Game::restart() noexcept {
    units_.clear();
    playerEnergy_ = 5.0;
    board_.reset();
    isRunning_ = true;
}