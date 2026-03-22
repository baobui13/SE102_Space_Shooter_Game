#pragma once
#include <string>
#include <functional>

class Player;

struct UpgradeOption {
    std::string name;
    std::string description;

    std::function<void(Player&)> applyEffect;
};