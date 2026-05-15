#include "EnemyFactory.h"
#include "EnemyPool.h"
#include "MeleeEnemy1.h"
#include "RangeEnemy1.h"
#include "MovementStrategies.h"
#include "AssetManager.h"

namespace {
std::unique_ptr<IMovementStrategy> CreateMovementStrategy(const EnemyMovementDefinition& movement) {
    switch (movement.kind) {
    case EnemyMovementKind::Chase:
        return std::make_unique<ChaseMovement>(movement.a);
    case EnemyMovementKind::Linear:
        return std::make_unique<LinearMovement>(movement.a, movement.b);
    case EnemyMovementKind::SineWave:
        return std::make_unique<SineWaveMovement>(movement.a, movement.b, movement.c);
    case EnemyMovementKind::Circular:
        return std::make_unique<CircularMovement>(movement.a, movement.b, movement.c, movement.d);
    default:
        return std::make_unique<ChaseMovement>(80.0f);
    }
}

LevelEnemySpawnDefinition DefaultSpawn(EnemyType type, float x, float y) {
    switch (type) {
    case EnemyType::Melee_Fast:
        return {
            type,
            x,
            y,
            { 50.0f, 150.0f, 5.0f, 1.0f, 20.0f, 15 },
            EnemyMovementDefinition::SineWave(120.0f, 60.0f, 3.0f)
        };
    case EnemyType::Ranged_Basic:
        return {
            type,
            x,
            y,
            { 80.0f, 100.0f, 15.0f, 1.0f, 500.0f, 20 },
            EnemyMovementDefinition::Linear(0.0f, 50.0f)
        };
    case EnemyType::Melee_Basic:
    default:
        return {
            EnemyType::Melee_Basic,
            x,
            y,
            { 100.0f, 80.0f, 10.0f, 1.0f, 30.0f, 10 },
            EnemyMovementDefinition::Chase(80.0f)
        };
    }
}
}

std::unique_ptr<BaseEnemy> EnemyFactory::Create(
    EnemyType type, Graphics& gfx, float x, float y)
{
    return Create(DefaultSpawn(type, x, y), gfx);
}

std::unique_ptr<BaseEnemy> EnemyFactory::Create(
    const LevelEnemySpawnDefinition& spawn, Graphics& gfx)
{
    const auto& visual = EnemyPool::GetVisual(spawn.type);
    const auto& stats = spawn.stats;
    std::unique_ptr<BaseEnemy> enemy;

    switch (spawn.type) {
    case EnemyType::Melee_Basic:
    case EnemyType::Melee_Fast:
        enemy = std::make_unique<MeleeEnemy1>(
            spawn.x,
            spawn.y,
            visual.width,
            visual.height,
            stats.health,
            stats.moveSpeed,
            stats.attackPower,
            stats.attackRange,
            stats.attackSpeed,
            spawn.type
        );
        break;

    case EnemyType::Ranged_Basic:
        enemy = std::make_unique<RangeEnemy1>(
            spawn.x,
            spawn.y,
            visual.width,
            visual.height,
            stats.health,
            stats.moveSpeed,
            stats.attackPower,
            stats.attackSpeed,
            stats.attackRange,
            spawn.type
        );
        break;

    default:
        enemy = std::make_unique<MeleeEnemy1>(
            spawn.x,
            spawn.y,
            visual.width,
            visual.height,
            stats.health,
            stats.moveSpeed,
            stats.attackPower,
            stats.attackRange,
            stats.attackSpeed,
            EnemyType::Melee_Basic
        );
        break;
    }

    enemy->SetMovementStrategy(CreateMovementStrategy(spawn.movement));
    enemy->SetSpriteForwardAngle(visual.spriteForwardAngle);
    enemy->SetExpReward(stats.expReward);

    auto tex = AssetManager::GetInstance().GetTexture(gfx, visual.texturePath);
    enemy->GetAnim().Initialize(tex);
    enemy->GetAnim().AddClip(
        visual.clipName,
        visual.frameX,
        visual.frameY,
        visual.frameWidth,
        visual.frameHeight,
        visual.frameCount,
        visual.columns,
        visual.frameDuration,
        visual.loop,
        visual.spacingX,
        visual.spacingY
    );
    enemy->GetAnim().Play(visual.clipName);

    return enemy;
}
