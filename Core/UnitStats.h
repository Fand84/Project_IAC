#pragma once
#include "Enums.h"

struct UnitStats {
    int maxHp{ 100 };
    int damage{ 10 };
    double speed{ 40.0 }; 
    int cost{ 3 };
    double range{ 25.0 }; 
	double attackCooldown{ 1.0 };

    [[nodiscard]] static constexpr UnitStats getDefaultsFor(UnitType type) noexcept {
        switch (type) {
            case UnitType::Soldat:   return { 120, 15, 45.0, 3, 20.0, 1.0 };
            case UnitType::Tanc:     return { 260, 20, 25.0, 5, 20.0, 1.5 };
            case UnitType::Arcas:    return {  70, 18, 40.0, 3, 90.0, 1.2 };
            case UnitType::Cercetas: return {  60, 10, 75.0, 2, 18.0, 0.8 };
            case UnitType::Mag:      return {  80, 30, 35.0, 4, 70.0, 1.4 };
        }
        return {};
    }
};
