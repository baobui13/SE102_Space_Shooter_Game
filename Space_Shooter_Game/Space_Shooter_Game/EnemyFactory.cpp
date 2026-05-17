#include "EnemyFactory.h"
#include "EnemyPool.h"
#include "MeleeEnemy1.h"
#include "MeleeEnemy2.h"
#include "RangeEnemy1.h"
#include "MovementStrategies.h"
#include "AssetManager.h"

namespace {
std::unique_ptr<IMovementStrategy> CreateSingleMovementStrategy(const EnemyMovementDefinition& movement) {
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

class SequencedMovementStrategy : public IMovementStrategy {
public:
    explicit SequencedMovementStrategy(EnemyMovementSequenceDefinition sequence)
        : m_sequence(std::move(sequence)) {
        if (!m_sequence.steps.empty()) {
            m_currentStrategy = CreateSingleMovementStrategy(m_sequence.steps[0].movement);
        }
    }

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
        float& outVx, float& outVy) override {
        AdvanceStep(dt);

        if (m_currentStrategy) {
            m_currentStrategy->CalculateVelocity(cx, cy, dt, ctx, outVx, outVy);
        }
        else {
            outVx = 0.0f;
            outVy = 0.0f;
        }
    }

private:
    EnemyMovementSequenceDefinition m_sequence;
    std::unique_ptr<IMovementStrategy> m_currentStrategy;
    size_t m_currentStepIndex = 0;
    float m_stepElapsed = 0.0f;

    void AdvanceStep(float dt) {
        if (m_sequence.steps.empty()) {
            return;
        }

        m_stepElapsed += dt;

        while (ShouldAdvanceCurrentStep()) {
            size_t nextIndex = m_currentStepIndex + 1;
            if (nextIndex >= m_sequence.steps.size()) {
                if (m_sequence.mode == EnemyMovementSequenceMode::Loop) {
                    nextIndex = 0;
                }
                else {
                    return;
                }
            }

            m_currentStepIndex = nextIndex;
            m_stepElapsed = 0.0f;
            m_currentStrategy = CreateSingleMovementStrategy(m_sequence.steps[m_currentStepIndex].movement);
        }
    }

    bool ShouldAdvanceCurrentStep() const {
        if (m_sequence.steps.empty()) {
            return false;
        }

        if (m_sequence.mode == EnemyMovementSequenceMode::Linear &&
            m_currentStepIndex + 1 >= m_sequence.steps.size()) {
            return false;
        }

        float duration = m_sequence.steps[m_currentStepIndex].duration;
        return duration > 0.0f && m_stepElapsed >= duration;
    }
};

std::unique_ptr<IMovementStrategy> CreateMovementStrategy(EnemyMovementSequenceDefinition sequence) {
    float fleeTriggerDistance = sequence.fleeTriggerDistance;
    float fleeSpeed = sequence.fleeSpeed;

    if (sequence.steps.empty()) {
        auto strategy = CreateSingleMovementStrategy(EnemyMovementDefinition::Chase(80.0f));
        if (fleeTriggerDistance > 0.0f) {
            if (fleeSpeed <= 0.0f) fleeSpeed = 80.0f;
            return std::make_unique<FleeFromPlayerMovement>(
                std::move(strategy), fleeTriggerDistance, fleeSpeed);
        }
        return strategy;
    }

    auto strategy = std::make_unique<SequencedMovementStrategy>(std::move(sequence));
    if (fleeTriggerDistance > 0.0f) {
        if (fleeSpeed <= 0.0f) fleeSpeed = 80.0f;
        return std::make_unique<FleeFromPlayerMovement>(
            std::move(strategy), fleeTriggerDistance, fleeSpeed);
    }
    return strategy;
}

std::unique_ptr<IMovementStrategy> CreateMovementStrategy(
    EnemyMovementSequenceDefinition sequence,
    float defaultFleeSpeed)
{
    if (sequence.fleeTriggerDistance > 0.0f && sequence.fleeSpeed <= 0.0f) {
        sequence.fleeSpeed = defaultFleeSpeed;
    }
    return CreateMovementStrategy(std::move(sequence));
}

LevelEnemySpawnDefinition DefaultSpawn(EnemyType type, float x, float y) {
    switch (type) {
    case EnemyType::Melee_Fast:
        return {
            type,
            x,
            y,
            { 50.0f, 150.0f, 5.0f, 1.0f, 20.0f, 15 },
            EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(120.0f))
        };
    case EnemyType::Ranged_Basic:
        return {
            type,
            x,
            y,
            { 80.0f, 100.0f, 15.0f, 1.0f, 500.0f, 20 },
            EnemyMovementSequenceDefinition::WithFleeFromPlayer(
                EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Linear(0.0f, 50.0f)),
                220.0f)
        };
    case EnemyType::Melee_Spawner:
        return {
            EnemyType::Melee_Spawner,
            x,
            y,
            { 300.0f, 40.0f, 12.0f, 1.0f, 30.0f, 25 },
            EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(50.0f))
        };
    case EnemyType::Melee_Basic:
    default:
        return {
            EnemyType::Melee_Basic,
            x,
            y,
            { 100.0f, 80.0f, 10.0f, 1.0f, 30.0f, 10 },
            EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(80.0f))
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
            visual.displayWidth,
            visual.displayHeight,
            stats.health,
            stats.moveSpeed,
            stats.attackPower,
            stats.attackRange,
            stats.attackSpeed,
            spawn.type
        );
        break;

    case EnemyType::Melee_Spawner: {
        auto spawner = std::make_unique<MeleeEnemy2>(
            spawn.x,
            spawn.y,
            visual.displayWidth,
            visual.displayHeight,
            stats.health,
            stats.moveSpeed,
            stats.attackPower,
            stats.attackRange,
            stats.attackSpeed,
            spawn.type
        );
        if (!spawn.spawnEntries.empty()) {
            spawner->SetSpawnEntries(spawn.spawnEntries);
        }
        spawner->SetPeriodicSpawnInterval(spawn.periodicSpawnInterval);
        spawner->SetDeathSpawnCount(spawn.deathSpawnCount);
        enemy = std::move(spawner);
        break;
    }

    case EnemyType::Ranged_Basic:
        enemy = std::make_unique<RangeEnemy1>(
            spawn.x,
            spawn.y,
            visual.displayWidth,
            visual.displayHeight,
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
            visual.displayWidth,
            visual.displayHeight,
            stats.health,
            stats.moveSpeed,
            stats.attackPower,
            stats.attackRange,
            stats.attackSpeed,
            EnemyType::Melee_Basic
        );
        break;
    }

    enemy->SetMovementStrategy(CreateMovementStrategy(spawn.movementSequence, stats.moveSpeed));
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
