#pragma once
#include <algorithm>


class Energy
{
private:
    double current_{ 5.0 };
    double max_{ 10.0 };
    double regenRate_{ 1.0 }; 

public:
    explicit Energy(double initial = 5.0, double max = 10.0, double regenRate = 1.0) noexcept;

   
    void update(double deltaTime) noexcept;

    
    [[nodiscard]] bool canAfford(int cost) const noexcept;
    bool consume(int cost) noexcept;
    void reset(double initial = 5.0) noexcept;

   
    [[nodiscard]] double getCurrent() const noexcept;
    [[nodiscard]] double getMax() const noexcept;
    [[nodiscard]] double getRegenRate() const noexcept;
    void setRegenRate(double rate) noexcept;
};


