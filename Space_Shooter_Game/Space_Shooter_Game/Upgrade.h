#pragma once
#include <string>
#include <functional>

class Player;

struct UpgradeOption {
    std::string type;        // Ví dụ: "ATTACK", "DEFENSE", "SPEED"
    std::string name;        // Ví dụ: "Sung Nhieu Nong"
    std::string description; // Ví dụ: "Tang 20% toc do ban"
    std::string iconTag;     // Ví dụ: "sword", "heart", "shield", "boot"

    std::function<void(Player&)> applyEffect;
};