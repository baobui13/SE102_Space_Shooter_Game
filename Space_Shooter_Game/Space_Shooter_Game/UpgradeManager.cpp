#include "UpgradeManager.h"
#include "ConfigUtils.h"
#include "Player.h"
#include "SimpleJson.h"

#include <algorithm>
#include <random>

namespace {
void ApplyUpgradeEffect(Player& player, const std::string& target, const std::string& operation, float value) {
    if (target == "attackDamage") {
        player.SetAttackDamage(player.GetAttackDamage() + static_cast<int>(value));
    } else if (target == "attackSpeed") {
        player.SetAttackSpeed(player.GetAttackSpeed() + value);
    } else if (target == "attackRange") {
        player.SetAttackRange(player.GetAttackRange() + value);
    } else if (target == "cooldownMultiplier") {
        player.SetCooldownMultiplier(
            operation == "multiply"
                ? player.GetCooldownMultiplier() * value
                : player.GetCooldownMultiplier() + value);
    } else if (target == "skillSizeMultiplier") {
        player.SetSkillSizeMultiplier(player.GetSkillSizeMultiplier() + value);
    } else if (target == "heal") {
        player.Heal(static_cast<int>(value));
    } else if (target == "maxHp") {
        player.SetMaxHp(player.GetMaxHp() + static_cast<int>(value));
    } else if (target == "speed") {
        player.SetSpeed(player.GetSpeed() + value);
    } else if (target == "magnetRange") {
        player.SetMagnetRange(player.GetMagnetRange() + value);
    }
}

UpgradeOption ParseUpgrade(const JsonValue& value) {
    UpgradeOption option;
    option.type = value.At("type").AsStringOr("UTILITY");
    option.name = value.At("name").AsStringOr("Upgrade");
    option.description = value.At("description").AsStringOr("");
    option.iconTag = value.At("iconTag").AsStringOr("shield");

    struct EffectConfig {
        std::string target;
        std::string operation;
        float value;
    };

    std::vector<EffectConfig> effects;
    for (const auto& effect : value.At("effects").AsArray()) {
        effects.push_back({
            effect.At("target").AsStringOr(""),
            effect.At("operation").AsStringOr("add"),
            static_cast<float>(effect.At("value").AsNumber(0.0))
        });
    }

    option.applyEffect = [effects](Player& player) {
        for (const auto& effect : effects) {
            ApplyUpgradeEffect(player, effect.target, effect.operation, effect.value);
        }
    };

    return option;
}
}

UpgradeManager::UpgradeManager() {
    Initialize();
}

void UpgradeManager::Initialize() {
    m_allUpgrades.clear();

    try {
        const JsonValue root = SimpleJson::ParseFile(
            ResolveConfigPath("config/upgrades/upgrades.json"));
        for (const auto& upgrade : root.At("upgrades").AsArray()) {
            m_allUpgrades.push_back(ParseUpgrade(upgrade));
        }
    }
    catch (...) {
    }
}

std::vector<UpgradeOption> UpgradeManager::GetRandomUpgrades(int count) {
    std::vector<UpgradeOption> result;
    if (m_allUpgrades.empty()) return result;

    std::vector<UpgradeOption> deck = m_allUpgrades;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    for (int i = 0; i < count && i < deck.size(); ++i) {
        result.push_back(deck[i]);
    }

    return result;
}
