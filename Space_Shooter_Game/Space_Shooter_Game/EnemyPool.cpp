#include "EnemyPool.h"
#include "SimpleJson.h"

#include <filesystem>
#include <string>
#include <vector>

namespace {
std::wstring ToWide(const std::string& value) {
    return std::wstring(value.begin(), value.end());
}

std::string ResolveConfigPath(const std::string& filePath) {
    if (std::filesystem::exists(filePath)) {
        return filePath;
    }

    std::filesystem::path current = std::filesystem::current_path();
    const std::filesystem::path relativePath(filePath);
    for (int i = 0; i < 6 && !current.empty(); ++i) {
        const std::filesystem::path candidate = current / relativePath;
        if (std::filesystem::exists(candidate)) {
            return candidate.string();
        }
        current = current.parent_path();
    }

    const std::filesystem::path projectPath =
        std::filesystem::path("Space_Shooter_Game") / "Space_Shooter_Game" / filePath;
    if (std::filesystem::exists(projectPath)) {
        return projectPath.string();
    }

    return filePath;
}

EnemyType EnemyTypeFromString(const std::string& value) {
    if (value == "Melee_Fast") return EnemyType::Melee_Fast;
    if (value == "Melee_Spawner") return EnemyType::Melee_Spawner;
    if (value == "Ranged_Basic") return EnemyType::Ranged_Basic;
    if (value == "Ranged_Burst") return EnemyType::Ranged_Burst;
    if (value == "Boss_Stage1") return EnemyType::Boss_Stage1;
    return EnemyType::Melee_Basic;
}

SpriteHeadDirection SpriteHeadDirectionFromString(const std::string& value) {
    if (value == "Right") return SpriteHeadDirection::Right;
    if (value == "Left") return SpriteHeadDirection::Left;
    if (value == "Up") return SpriteHeadDirection::Up;
    return SpriteHeadDirection::Down;
}

float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

int ReadInt(const JsonValue& object, const std::string& key, int fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<int>(value->AsNumber(fallback)) : fallback;
}

EnemyVisualDefinition ParseVisual(const std::string& typeName, const JsonValue& value) {
    EnemyVisualDefinition visual{};
    visual.type = EnemyTypeFromString(typeName);
    visual.displayWidth = ReadFloat(value, "displayWidth", 64.0f);
    visual.displayHeight = ReadFloat(value, "displayHeight", 64.0f);
    visual.texturePath = ToWide(value.At("texture").AsStringOr("Assets/Enemy/Enemy1.png"));
    visual.clipName = value.At("clipName").AsStringOr("idle");
    visual.frameX = ReadInt(value, "frameX", 0);
    visual.frameY = ReadInt(value, "frameY", 0);
    visual.frameWidth = ReadInt(value, "frameWidth", 64);
    visual.frameHeight = ReadInt(value, "frameHeight", 64);
    visual.frameCount = ReadInt(value, "frameCount", 1);
    visual.columns = ReadInt(value, "columns", 1);
    visual.frameDuration = ReadFloat(value, "frameDuration", 1.0f);
    visual.loop = value.At("loop").AsBool(true);
    visual.spacingX = ReadInt(value, "spacingX", 0);
    visual.spacingY = ReadInt(value, "spacingY", 0);
    visual.spriteForwardAngle = SpriteHeadDirectionToRadians(
        SpriteHeadDirectionFromString(value.At("spriteHeadDirection").AsStringOr("Down")));
    return visual;
}

std::vector<EnemyVisualDefinition> LoadVisuals() {
    std::vector<EnemyVisualDefinition> visuals;

    try {
        const JsonValue root = SimpleJson::ParseFile(
            ResolveConfigPath("config/enemies/enemy_visuals.json"));
        for (const auto& [typeName, value] : root.AsObject()) {
            visuals.push_back(ParseVisual(typeName, value));
        }
    }
    catch (...) {
    }

    if (visuals.empty()) {
        visuals.push_back(ParseVisual("Melee_Basic", JsonValue(JsonValue::Object{})));
    }
    return visuals;
}
}

const EnemyVisualDefinition& EnemyPool::GetVisual(EnemyType type) {
    static const std::vector<EnemyVisualDefinition> visuals = LoadVisuals();

    for (const auto& visual : visuals) {
        if (visual.type == type) {
            return visual;
        }
    }

    return visuals[0];
}
