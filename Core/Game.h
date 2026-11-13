#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Board.h"
#include "Unit.h"
#include "Enums.h"
#include "UnitStats.h"

class Game {
private:
    Board board_;
    std::vector<std::unique_ptr<Unit>> units_;

    double playerEnergy_{ 5.0 };
    double maxEnergy_{ 10.0 };
    double energyRegenRate_{ 1.0 };

    bool isRunning_{ true };

public:
    explicit Game(double laneLength = 1000.0, int castleHp = 1000) noexcept;
    ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;

    void update(double deltaTime);

    [[nodiscard]] bool canPlaceUnit(int cost) const noexcept;
    [[nodiscard]] bool spawnUnit(UnitType type, PlayerSide side, Lane lane);
    [[nodiscard]] bool spawnUnit(UnitType type, PlayerSide side, Lane lane, const UnitStats& stats);

    [[nodiscard]] double getPlayerEnergy() const noexcept;
    [[nodiscard]] double getMaxEnergy() const noexcept;
    [[nodiscard]] const Board& getBoard() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<Unit>>& getUnits() const noexcept;
    [[nodiscard]] bool isGameOver() const noexcept;

    void restart() noexcept;
};