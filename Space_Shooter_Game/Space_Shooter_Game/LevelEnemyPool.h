#pragma once
#include "EnemyDefinitions.h"
#include <vector>

class LevelEnemyPool {
public:
    static std::vector<EnemyPhaseDefinition> Create(int levelIndex);

private:
    static std::vector<EnemyPhaseDefinition> CreateLevel1();
    static std::vector<EnemyPhaseDefinition> CreateLevel2();
    static std::vector<EnemyPhaseDefinition> CreateLevel3();
    static std::vector<EnemyPhaseDefinition> CreateLevel4();
};
