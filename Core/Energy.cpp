#include "Energy.h"
#include "Energy.h"

Energy::Energy(double initial, double max, double regenRate) noexcept
    : current_(std::min(initial, max)),
    max_(max),
    regenRate_(regenRate) {
}

void Energy::update(double deltaTime) noexcept {
    if (current_ < max_) {
        current_ = std::min(max_, current_ + regenRate_ * deltaTime);
    }
}

bool Energy::canAfford(int cost) const noexcept {
    return current_ >= static_cast<double>(cost);
}

bool Energy::consume(int cost) noexcept {
    if (!canAfford(cost)) {
        return false;
    }
    current_ -= static_cast<double>(cost);
    return true;
}

void Energy::reset(double initial) noexcept {
    current_ = std::min(initial, max_);
}

double Energy::getCurrent() const noexcept {
    return current_;
}

double Energy::getMax() const noexcept {
    return max_;
}

double Energy::getRegenRate() const noexcept {
    return regenRate_;
}

void Energy::setRegenRate(double rate) noexcept {
    regenRate_ = rate;
}