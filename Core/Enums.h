#pragma once
#include <cstdint>

enum class UnitType : std::uint8_t {
    Soldat,
    Tanc,
    Arcas,
    Cercetas,
    Mag
};

enum class PlayerSide : std::uint8_t {
    Player,
    Computer
};

enum class Lane : std::uint8_t {
    Top = 0,    
    Bottom = 1  
};
