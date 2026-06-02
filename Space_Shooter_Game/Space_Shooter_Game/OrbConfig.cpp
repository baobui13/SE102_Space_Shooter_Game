#include "OrbConfig.h"
#include "ConfigUtils.h"
#include "SimpleJson.h"

#include <unordered_map>

namespace {
constexpr const char* ORB_CONFIG_PATH = "config/objects/orbs.json";

float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

std::string ReadString(const JsonValue& object, const std::string& key, const std::string& fallback) {
    const JsonValue* value = object.Find(key);
    return value ? value->AsStringOr(fallback) : fallback;
}

OrbDefinition ParseDefinition(const JsonValue& value, const OrbDefinition& fallback) {
    OrbDefinition definition = fallback;
    definition.animationId = ReadString(value, "animationId", definition.animationId);
    definition.colliderName = ReadString(value, "colliderName", definition.colliderName);
    definition.collectColliderName = ReadString(value, "collectColliderName", definition.collectColliderName);
    definition.width = ReadFloat(value, "width", definition.width);
    definition.height = ReadFloat(value, "height", definition.height);
    definition.magnetSpeed = ReadFloat(value, "magnetSpeed", definition.magnetSpeed);
    definition.fallSpeed = ReadFloat(value, "fallSpeed", definition.fallSpeed);
    return definition;
}

std::unordered_map<std::string, OrbDefinition> CreateDefaults() {
    std::unordered_map<std::string, OrbDefinition> definitions;

    definitions["exp"] = {
        "exp_orb_idle",
        "exp_orb",
        "exp_orb_collect",
        16.0f,
        16.0f,
        450.0f,
        50.0f
    };

    definitions["talent"] = {
        "talent_orb_idle",
        "talent_orb",
        "talent_orb_collect",
        18.0f,
        18.0f,
        500.0f,
        40.0f
    };

    return definitions;
}

const std::unordered_map<std::string, OrbDefinition>& LoadDefinitions() {
    static std::unordered_map<std::string, OrbDefinition> definitions = CreateDefaults();
    static bool loaded = false;

    if (loaded) {
        return definitions;
    }

    loaded = true;

    try {
        const JsonValue root = SimpleJson::ParseFile(ResolveConfigPath(ORB_CONFIG_PATH));
        for (const auto& [id, value] : root.AsObject()) {
            const auto fallbackIt = definitions.find(id);
            const OrbDefinition fallback = fallbackIt != definitions.end()
                ? fallbackIt->second
                : OrbDefinition{};
            definitions[id] = ParseDefinition(value, fallback);
        }
    }
    catch (...) {
    }

    return definitions;
}
}

const OrbDefinition& OrbConfig::GetExpOrb() {
    return GetDefinition("exp");
}

const OrbDefinition& OrbConfig::GetTalentOrb() {
    return GetDefinition("talent");
}

const OrbDefinition& OrbConfig::GetDefinition(const std::string& id) {
    const auto& definitions = LoadDefinitions();
    const auto it = definitions.find(id);
    if (it != definitions.end()) {
        return it->second;
    }
    return definitions.at("exp");
}
