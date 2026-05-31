#include "SkillConfig.h"
#include "ConfigUtils.h"
#include "SimpleJson.h"

#include <map>

namespace {
float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

int ReadInt(const JsonValue& object, const std::string& key, int fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<int>(value->AsNumber(fallback)) : fallback;
}

char ReadKey(const JsonValue& object, const std::string& key, char fallback) {
    const std::string value = object.At(key).AsStringOr("");
    return value.empty() ? fallback : value[0];
}

SkillConfigValues DefaultsFor(const std::string& id) {
    if (id == "laser") {
        return { "Laser Beam", "Fires a massive laser beam from your ship.", '1', 10.0f, 5, 5.0f, 0.0f, 0.0f, 0.0f, 50, 20, 20, 1.5f };
    }
    if (id == "exploding_bullet") {
        return { "Explosive Bullet", "Fires a slow bullet toward your cursor. Explodes on impact!", '2', 3.0f, 5, 0.0f, 0.0f, 160.0f, 20.0f, 50, 20, 25, 1.2f };
    }
    if (id == "shield") {
        return { "Shield", "Creates a shield around your ship and blocks incoming damage.", '3', 12.0f, 5, 3.0f, 0.75f, 0.0f, 0.0f, 0, 0, 0, 0.0f };
    }
    return {};
}

std::map<std::string, SkillConfigValues> LoadSkillConfigs() {
    std::map<std::string, SkillConfigValues> configs;

    try {
        const JsonValue root = SimpleJson::ParseFile(
            ResolveConfigPath("config/skills/skills.json"));
        for (const auto& [id, value] : root.At("skills").AsObject()) {
            SkillConfigValues config = DefaultsFor(id);
            config.name = value.At("name").AsStringOr(config.name);
            config.description = value.At("description").AsStringOr(config.description);
            config.activationKey = ReadKey(value, "activationKey", config.activationKey);
            config.cooldown = ReadFloat(value, "cooldown", config.cooldown);
            config.maxLevel = ReadInt(value, "maxLevel", config.maxLevel);
            config.baseDuration = ReadFloat(value, "baseDuration", config.baseDuration);
            config.durationLevelBonus = ReadFloat(value, "durationLevelBonus", config.durationLevelBonus);
            config.baseSpeed = ReadFloat(value, "baseSpeed", config.baseSpeed);
            config.speedLevelBonus = ReadFloat(value, "speedLevelBonus", config.speedLevelBonus);
            config.baseDamage = ReadInt(value, "baseDamage", config.baseDamage);
            config.damageLevelBonus = ReadInt(value, "damageLevelBonus", config.damageLevelBonus);
            config.levelUpDamageBonus = ReadInt(value, "levelUpDamageBonus", config.levelUpDamageBonus);
            config.playerDamageScale = ReadFloat(value, "playerDamageScale", config.playerDamageScale);
            configs[id] = config;
        }
    }
    catch (...) {
    }

    return configs;
}
}

SkillConfigValues GetSkillConfig(const std::string& id) {
    static const std::map<std::string, SkillConfigValues> configs = LoadSkillConfigs();
    auto it = configs.find(id);
    return it == configs.end() ? DefaultsFor(id) : it->second;
}
