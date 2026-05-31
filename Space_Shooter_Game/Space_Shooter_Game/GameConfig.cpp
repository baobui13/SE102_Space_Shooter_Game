#include "GameConfig.h"
#include "SimpleJson.h"

#include <filesystem>

namespace {
GameConfigValues g_config;

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

float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

void ApplyGlobals() {
    VIRTUAL_WIDTH = g_config.virtualWidth;
    VIRTUAL_HEIGHT = g_config.virtualHeight;
    MOVE_BLEND_DURATION = g_config.moveBlendDuration;
    ROTATION_SMOOTH_SPEED = g_config.rotationSmoothSpeed;
}
}

const GameConfigValues& GetGameConfig() {
    return g_config;
}

void LoadGameConfig(const std::string& filePath) {
    try {
        const JsonValue root = SimpleJson::ParseFile(ResolveConfigPath(filePath));
        g_config.virtualWidth = ReadFloat(root, "virtualWidth", g_config.virtualWidth);
        g_config.virtualHeight = ReadFloat(root, "virtualHeight", g_config.virtualHeight);
        g_config.moveBlendDuration = ReadFloat(root, "moveBlendDuration", g_config.moveBlendDuration);
        g_config.rotationSmoothSpeed = ReadFloat(root, "rotationSmoothSpeed", g_config.rotationSmoothSpeed);
    }
    catch (...) {
        // Keep built-in defaults when the external config is missing or invalid.
    }

    ApplyGlobals();
}
