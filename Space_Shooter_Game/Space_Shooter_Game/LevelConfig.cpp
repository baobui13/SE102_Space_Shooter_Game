#include "LevelConfig.h"
#include "ConfigUtils.h"
#include "EntityManager.h"
#include "ExpOrb.h"
#include "GameConfig.h"
#include "SimpleJson.h"
#include "TalentOrb.h"

#include <iomanip>
#include <memory>
#include <sstream>

namespace {
std::wstring ToWide(const std::string& value) {
    return std::wstring(value.begin(), value.end());
}

int ReadInt(const JsonValue& object, const std::string& key, int fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<int>(value->AsNumber(fallback)) : fallback;
}

std::string DefaultLevelPath(int levelIndex) {
    std::ostringstream path;
    path << "config/levels/level_" << std::setw(2) << std::setfill('0') << levelIndex << ".json";
    return path.str();
}

std::string LevelPathFromManifest(int levelIndex) {
    try {
        const JsonValue manifest = SimpleJson::ParseFile(
            ResolveConfigPath("config/level_manifest.json"));
        for (const auto& level : manifest.At("levels").AsArray()) {
            if (ReadInt(level, "id", 0) == levelIndex) {
                return level.At("config").AsStringOr(DefaultLevelPath(levelIndex));
            }
        }
    }
    catch (...) {
    }

    return DefaultLevelPath(levelIndex);
}

JsonValue LoadLevel(int levelIndex) {
    return SimpleJson::ParseFile(ResolveConfigPath(LevelPathFromManifest(levelIndex)));
}

float ReadCoordinate(const JsonValue& value, float fallback, bool isX) {
    if (value.IsNumber()) {
        return static_cast<float>(value.AsNumber(fallback));
    }

    const std::string token = value.AsStringOr("");
    if (token == "center") {
        return isX ? VIRTUAL_WIDTH * 0.5f : VIRTUAL_HEIGHT * 0.5f;
    }
    if (!token.empty() && token.back() == '%') {
        try {
            const float percent = std::stof(token.substr(0, token.size() - 1)) / 100.0f;
            return (isX ? VIRTUAL_WIDTH : VIRTUAL_HEIGHT) * percent;
        }
        catch (...) {
            return fallback;
        }
    }

    return fallback;
}
}

std::wstring LevelConfig::GetBackgroundTexturePath(int levelIndex) {
    try {
        const JsonValue level = LoadLevel(levelIndex);
        return ToWide(level.At("background").AsStringOr(""));
    }
    catch (...) {
        return L"";
    }
}

std::vector<ExpOrbConfig> LevelConfig::GetExpOrbs(int levelIndex) {
    std::vector<ExpOrbConfig> orbs;

    try {
        const JsonValue level = LoadLevel(levelIndex);
        for (const auto& orb : level.At("objects").At("expOrbs").AsArray()) {
            orbs.push_back({
                ReadCoordinate(orb.At("x"), 0.0f, true),
                ReadCoordinate(orb.At("y"), 0.0f, false),
                ReadInt(orb, "value", 0)
            });
        }
    }
    catch (...) {
    }

    return orbs;
}

std::vector<TalentOrbConfig> LevelConfig::GetTalentOrbs(int levelIndex) {
    std::vector<TalentOrbConfig> orbs;

    try {
        const JsonValue level = LoadLevel(levelIndex);
        for (const auto& orb : level.At("objects").At("talentOrbs").AsArray()) {
            orbs.push_back({
                ReadCoordinate(orb.At("x"), 0.0f, true),
                ReadCoordinate(orb.At("y"), 0.0f, false),
                ReadInt(orb, "points", 1)
            });
        }
    }
    catch (...) {
    }

    return orbs;
}

void LevelConfig::SpawnObjects(int levelIndex, Graphics& gfx, EntityManager& entityManager) {
    for (const auto& orb : GetExpOrbs(levelIndex)) {
        entityManager.AddEntity(std::make_unique<ExpOrb>(gfx, orb.x, orb.y, orb.value));
    }

    for (const auto& orb : GetTalentOrbs(levelIndex)) {
        entityManager.AddEntity(std::make_unique<TalentOrb>(gfx, orb.x, orb.y, orb.points));
    }
}
