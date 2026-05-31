#pragma once

#include <string>

struct GameConfigValues {
    float virtualWidth = 1920.0f;
    float virtualHeight = 1080.0f;
    float moveBlendDuration = 0.35f;
    float rotationSmoothSpeed = 8.0f;
};

inline float VIRTUAL_WIDTH = 1920.0f;
inline float VIRTUAL_HEIGHT = 1080.0f;
inline float MOVE_BLEND_DURATION = 0.35f;
inline float ROTATION_SMOOTH_SPEED = 8.0f;

const GameConfigValues& GetGameConfig();
void LoadGameConfig(const std::string& filePath = "config/gameplay.json");
