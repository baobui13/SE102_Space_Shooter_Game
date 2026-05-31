#pragma once

#include <string>
#include <vector>

class EntityManager;
class Graphics;

struct ExpOrbConfig {
    float x = 0.0f;
    float y = 0.0f;
    int value = 0;
};

class LevelConfig {
public:
    static std::wstring GetBackgroundTexturePath(int levelIndex);
    static std::vector<ExpOrbConfig> GetExpOrbs(int levelIndex);
    static void SpawnObjects(int levelIndex, Graphics& gfx, EntityManager& entityManager);
};
