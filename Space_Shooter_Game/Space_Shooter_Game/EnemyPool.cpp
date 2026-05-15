#include "EnemyPool.h"
#include <array>

namespace {
const std::array<EnemyVisualDefinition, 3> ENEMY_VISUALS = { {
    {
        EnemyType::Melee_Basic,
        48.0f,
        48.0f,
        L"Assets/Enemy/Enemy1_lv1.png",
        "idle",
        0,
        0,
        128,
        128,
        1,
        1,
        1.0f,
        true,
        0,
        0,
        SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
    },
    {
        EnemyType::Melee_Fast,
        36.0f,
        36.0f,
        L"Assets/Enemy/Enemy1_lv1.png",
        "idle",
        0,
        0,
        128,
        128,
        1,
        1,
        1.0f,
        true,
        0,
        0,
        SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
    },
    {
        EnemyType::Ranged_Basic,
        48.0f,
        48.0f,
        L"Assets/Enemy/Enemy2_lv1.png",
        "idle",
        0,
        0,
        128,
        128,
        1,
        1,
        1.0f,
        true,
        0,
        0,
        SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
    }
} };
}

const EnemyVisualDefinition& EnemyPool::GetVisual(EnemyType type) {
    for (const auto& visual : ENEMY_VISUALS) {
        if (visual.type == type) {
            return visual;
        }
    }

    return ENEMY_VISUALS[0];
}
