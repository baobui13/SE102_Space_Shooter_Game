#pragma once
#include "BaseEnemy.h"
#include "EnemyDefinitions.h"
#include "Graphics.h"
#include <memory>

// Factory tạo enemy từ EnemyType
// Khi thêm loại enemy mới: thêm enum value + thêm case trong Create()
class EnemyFactory {
public:
    static std::unique_ptr<BaseEnemy> Create(
        EnemyType type, Graphics& gfx,
        float x, float y
    );

    static std::unique_ptr<BaseEnemy> Create(
        const LevelEnemySpawnDefinition& spawn, Graphics& gfx
    );
};
