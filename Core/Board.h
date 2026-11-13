#pragma once

#include <algorithm>
#include <cstdint>
#include "Enums.h"

class Castle {
private:
    int maxHp_{ 1000 };
    int currentHp_{ 1000 };
    PlayerSide owner_{ PlayerSide::Player };

public:
    explicit Castle(PlayerSide owner, int maxHp = 1000) noexcept
        : maxHp_(maxHp), currentHp_(maxHp), owner_(owner) {
    }

    [[nodiscard]] int getCurrentHp() const noexcept { return currentHp_; }
    [[nodiscard]] int getMaxHp() const noexcept { return maxHp_; }
    [[nodiscard]] PlayerSide getOwner() const noexcept { return owner_; }
    [[nodiscard]] bool isDestroyed() const noexcept { return currentHp_ <= 0; }

    void takeDamage(int amount) noexcept {
        currentHp_ = std::max(0, currentHp_ - amount);
    }

    void reset() noexcept {
        currentHp_ = maxHp_;
    }
};

class Board {
private:
    double laneLength_{ 1000.0 };
    Castle playerCastle_;
    Castle computerCastle_;

public:
    explicit Board(double laneLength = 1000.0, int castleHp = 1000) noexcept;

    [[nodiscard]] double getLaneLength() const noexcept;
    [[nodiscard]] double getPlayerSpawnX() const noexcept;
    [[nodiscard]] double getComputerSpawnX() const noexcept;

    [[nodiscard]] const Castle& getPlayerCastle() const noexcept;
    [[nodiscard]] const Castle& getComputerCastle() const noexcept;
    [[nodiscard]] Castle& getPlayerCastle() noexcept;
    [[nodiscard]] Castle& getComputerCastle() noexcept;

    [[nodiscard]] bool isGameOver() const noexcept;
    [[nodiscard]] bool isPlayerWinner() const noexcept;
    [[nodiscard]] bool isComputerWinner() const noexcept;

    void reset() noexcept;
};