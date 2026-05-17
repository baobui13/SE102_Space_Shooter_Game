#include "EnemyPool.h"
#include <array>

namespace {
const std::array<EnemyVisualDefinition, 4> ENEMY_VISUALS = { {
    {
        .type = EnemyType::Melee_Basic,
        .displayWidth = 128.0f,
        .displayHeight = 128.0f,
        .texturePath = L"Assets/Enemy/Enemy3.png",
        .clipName = "idle",
        .frameX = 0,
        .frameY = 0,
        .frameWidth = 256,
        .frameHeight = 256,
        .frameCount = 1,
        .columns = 1,
        .frameDuration = 1.0f,
        .loop = true,
        .spacingX = 0,
        .spacingY = 0,
        .spriteForwardAngle = SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
    },
    {
        .type = EnemyType::Melee_Fast,
        .displayWidth = 36.0f,
        .displayHeight = 36.0f,
        .texturePath = L"Assets/Enemy/Enemy1.png",
        .clipName = "idle",
        .frameX = 0,
        .frameY = 0,
        .frameWidth = 128,
        .frameHeight = 128,
        .frameCount = 1,
        .columns = 1,
        .frameDuration = 1.0f,
        .loop = true,
        .spacingX = 0,
        .spacingY = 0,
        .spriteForwardAngle = SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
    },
    {
        .type = EnemyType::Ranged_Basic,
        .displayWidth = 64.0f,
        .displayHeight = 64.0f,
        .texturePath = L"Assets/Enemy/Enemy2.png",
        .clipName = "idle",
        .frameX = 0,
        .frameY = 0,
        .frameWidth = 128,
        .frameHeight = 128,
        .frameCount = 1,
        .columns = 1,
        .frameDuration = 1.0f,
        .loop = true,
        .spacingX = 0,
        .spacingY = 0,
        .spriteForwardAngle = SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
    },
    {
        .type = EnemyType::Melee_Spawner,
        .displayWidth = 160.0f,
        .displayHeight = 160.0f,
        .texturePath = L"Assets/Enemy/Enemy4.png",
        .clipName = "idle",
        .frameX = 0,
        .frameY = 0,
        .frameWidth = 1024,
        .frameHeight = 1024,
        .frameCount = 1,
        .columns = 1,
        .frameDuration = 1.0f,
        .loop = true,
        .spacingX = 0,
        .spacingY = 0,
        .spriteForwardAngle = SpriteHeadDirectionToRadians(SpriteHeadDirection::Down)
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
