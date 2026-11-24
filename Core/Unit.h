#pragma once
#include <string>
#include <ostream>
#include <algorithm>
#include "Enums.h"
#include "UnitStats.h"

class Unit {
protected:
    UnitType type_;
    PlayerSide owner_;
    int hp_;
    UnitStats stats_;
    double positionX_{ 0.0 };
    int lane_{ 0 };
    bool isAlive_{ true };
    double attackTimer_{ 0.0 };

public:
    Unit(UnitType type, PlayerSide owner, int lane, double startX, const UnitStats& stats) noexcept
        : type_(type)
        , owner_(owner)
        , hp_(stats.maxHp)
        , stats_(stats)
        , positionX_(startX)
        , lane_(lane)
        , attackTimer_(stats.attackCooldown) { // gata de primul atac imediat la contact
    }

    virtual ~Unit() = default;

    [[nodiscard]] UnitType getType() const noexcept { return type_; }
    [[nodiscard]] PlayerSide getOwner() const noexcept { return owner_; }
    [[nodiscard]] int getHp() const noexcept { return hp_; }
    [[nodiscard]] int getMaxHp() const noexcept { return stats_.maxHp; }
    [[nodiscard]] int getDamage() const noexcept { return stats_.damage; }
    [[nodiscard]] double getRange() const noexcept { return stats_.range; }
    [[nodiscard]] double getPositionX() const noexcept { return positionX_; }
    [[nodiscard]] int getLane() const noexcept { return lane_; }
    [[nodiscard]] bool isAlive() const noexcept { return isAlive_; }

   
    [[nodiscard]] virtual std::string getTypeName() const = 0;

    virtual void move(double deltaTime) {
        const double direction = (owner_ == PlayerSide::Player) ? 1.0 : -1.0;
        positionX_ += direction * stats_.speed * deltaTime;
    }

    void updateCooldown(double deltaTime) noexcept {
        attackTimer_ += deltaTime;
    }

    [[nodiscard]] bool canAttack() const noexcept {
        return attackTimer_ >= stats_.attackCooldown;
    }

    void resetAttackTimer() noexcept {
        attackTimer_ = 0.0;
    }

    virtual int calculateAttackDamage() const noexcept {
        return stats_.damage;
    }

    virtual void takeDamage(int amount) noexcept {
        hp_ -= std::max(0, amount);
        if (hp_ <= 0) {
            hp_ = 0;
            isAlive_ = false;
        }
    }

   
    friend std::ostream& operator<<(std::ostream& os, const Unit& unit) {
        os << unit.getTypeName()
            << " [Side: " << (unit.owner_ == PlayerSide::Player ? "Player" : "CPU")
            << ", Lane: " << unit.lane_
            << ", HP: " << unit.hp_ << "/" << unit.stats_.maxHp
            << ", X: " << unit.positionX_ << "]";
        return os;
    }
};

class Soldat final : public Unit {
public:
    Soldat(PlayerSide owner, int lane, double startX, const UnitStats& stats = UnitStats::getDefaultsFor(UnitType::Soldat)) noexcept
        : Unit(UnitType::Soldat, owner, lane, startX, stats) {
    }

    [[nodiscard]] std::string getTypeName() const override { return "Soldat"; }
};

class Tanc final : public Unit {
public:
    Tanc(PlayerSide owner, int lane, double startX, const UnitStats& stats = UnitStats::getDefaultsFor(UnitType::Tanc)) noexcept
        : Unit(UnitType::Tanc, owner, lane, startX, stats) {
    }

    [[nodiscard]] std::string getTypeName() const override { return "Tanc"; }

    
    void takeDamage(int amount) noexcept override {
        const int reduced = std::max(1, static_cast<int>(amount * 0.85));
        Unit::takeDamage(reduced);
    }
};

class Arcas final : public Unit {
public:
    Arcas(PlayerSide owner, int lane, double startX, const UnitStats& stats = UnitStats::getDefaultsFor(UnitType::Arcas)) noexcept
        : Unit(UnitType::Arcas, owner, lane, startX, stats) {
    }

    [[nodiscard]] std::string getTypeName() const override { return "Arcas"; }
};

class Cercetas final : public Unit {
public:
    Cercetas(PlayerSide owner, int lane, double startX, const UnitStats& stats = UnitStats::getDefaultsFor(UnitType::Cercetas)) noexcept
        : Unit(UnitType::Cercetas, owner, lane, startX, stats) {
    }

    [[nodiscard]] std::string getTypeName() const override { return "Cercetas"; }
};

class Mag final : public Unit {
public:
    Mag(PlayerSide owner, int lane, double startX, const UnitStats& stats = UnitStats::getDefaultsFor(UnitType::Mag)) noexcept
        : Unit(UnitType::Mag, owner, lane, startX, stats) {
    }

    [[nodiscard]] std::string getTypeName() const override { return "Mag"; }
};