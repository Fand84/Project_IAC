#pragma once
#include "Enums.h"
#include "UnitStats.h"

class Unit
{
protected:
    UnitType type_;
    PlayerSide owner_;
    int hp_;
    UnitStats stats_;
    double positionX_{ 0.0 }; 
    int lane_{ 0 };           
    bool isAlive_{ true };

public:
    Unit(UnitType type, PlayerSide owner, int lane, double startX, const UnitStats& stats) noexcept
        : type_(type), owner_(owner), hp_(stats.maxHp), stats_(stats), positionX_(startX), lane_(lane) {
    }

    virtual ~Unit() = default; 

   
    [[nodiscard]] UnitType getType() const noexcept { return type_; }
    [[nodiscard]] PlayerSide getOwner() const noexcept { return owner_; }
    [[nodiscard]] int getHp() const noexcept { return hp_; }
    [[nodiscard]] double getPositionX() const noexcept { return positionX_; }
    [[nodiscard]] int getLane() const noexcept { return lane_; }
    [[nodiscard]] bool isAlive() const noexcept { return isAlive_; }

    virtual void move(double deltaTime) {
        double direction = (owner_ == PlayerSide::Player) ? 1.0 : -1.0;
        positionX_ += direction * stats_.speed * deltaTime;
    }

    virtual void takeDamage(int amount) noexcept {
        hp_ -= amount;
        if (hp_ <= 0) {
            hp_ = 0;
            isAlive_ = false;
        }
    }
};


