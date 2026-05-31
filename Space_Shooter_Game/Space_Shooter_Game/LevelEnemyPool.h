#pragma once
#include "EnemyDefinitions.h"
#include <vector>

class LevelEnemyPool {
public:
    static std::vector<EnemyPhaseDefinition> Create(int levelIndex);
};
