#include "PlayerConfig.h"
#include "ConfigUtils.h"
#include "SimpleJson.h"

namespace {
float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

int ReadInt(const JsonValue& object, const std::string& key, int fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<int>(value->AsNumber(fallback)) : fallback;
}

PlayerConfigValues LoadPlayerConfig() {
    PlayerConfigValues config;

    try {
        const JsonValue root = SimpleJson::ParseFile(
            ResolveConfigPath("config/player/player_stats.json"));

        config.size = ReadFloat(root, "size", config.size);
        config.speed = ReadFloat(root, "speed", config.speed);
        config.hp = ReadInt(root, "hp", config.hp);
        config.attackDamage = ReadInt(root, "attackDamage", config.attackDamage);
        config.attackSpeed = ReadFloat(root, "attackSpeed", config.attackSpeed);
        config.attackRange = ReadFloat(root, "attackRange", config.attackRange);
        config.startLevel = ReadInt(root, "startLevel", config.startLevel);
        config.startExp = ReadInt(root, "startExp", config.startExp);
        config.expToNextLevel = ReadInt(root, "expToNextLevel", config.expToNextLevel);
        config.expGrowthMultiplier = ReadFloat(root, "expGrowthMultiplier", config.expGrowthMultiplier);
        config.magnetRange = ReadFloat(root, "magnetRange", config.magnetRange);
        config.cooldownMultiplier = ReadFloat(root, "cooldownMultiplier", config.cooldownMultiplier);
        config.skillSizeMultiplier = ReadFloat(root, "skillSizeMultiplier", config.skillSizeMultiplier);

        const JsonValue& dash = root.At("dash");
        config.dashSpeed = ReadFloat(dash, "speed", config.dashSpeed);
        config.dashDuration = ReadFloat(dash, "duration", config.dashDuration);
        config.dashCharges = ReadInt(dash, "charges", config.dashCharges);
        config.dashRechargeTime = ReadFloat(dash, "rechargeTime", config.dashRechargeTime);

        const JsonValue& weapon = root.At("weapon");
        config.bulletOffset = ReadFloat(weapon, "bulletOffset", config.bulletOffset);
        config.bulletSpeed = ReadFloat(weapon, "bulletSpeed", config.bulletSpeed);
        config.shootRecoilOffset = ReadFloat(weapon, "shootRecoilOffset", config.shootRecoilOffset);
        config.shootRecoilRecoverSpeed = ReadFloat(weapon, "shootRecoilRecoverSpeed", config.shootRecoilRecoverSpeed);
        config.invulDuration = ReadFloat(root, "invulDuration", config.invulDuration);
    }
    catch (...) {
    }

    return config;
}
}

const PlayerConfigValues& GetPlayerConfig() {
    static const PlayerConfigValues config = LoadPlayerConfig();
    return config;
}
