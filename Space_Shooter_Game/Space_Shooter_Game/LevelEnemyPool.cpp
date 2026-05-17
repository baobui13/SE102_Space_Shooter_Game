#include "LevelEnemyPool.h"

namespace {
EnemyStatsDefinition Stats(
    float health,
    float moveSpeed,
    float attackPower,
    float attackRange,
    int expReward,
    float attackSpeed = 1.0f)
{
    return { health, moveSpeed, attackPower, attackSpeed, attackRange, expReward };
}

LevelEnemySpawnDefinition Enemy(
    EnemyType type,
    float x,
    float y,
    EnemyStatsDefinition stats,
    EnemyMovementSequenceDefinition movementSequence)
{
    return { type, x, y, stats, std::move(movementSequence) };
}

const EnemyStatsDefinition MELEE_BASIC = Stats(100.0f, 80.0f, 10.0f, 30.0f, 10);
const EnemyStatsDefinition MELEE_FAST = Stats(50.0f, 150.0f, 5.0f, 20.0f, 15);
const EnemyStatsDefinition MELEE_SPAWNER = Stats(300.0f, 40.0f, 12.0f, 30.0f, 25);
const EnemyStatsDefinition RANGED_BASIC = Stats(80.0f, 100.0f, 15.0f, 500.0f, 20);

LevelEnemySpawnDefinition MeleeBasic(float x, float y) {
    return Enemy(
        EnemyType::Melee_Basic,
        x,
        y,
        MELEE_BASIC,
        EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(80.0f))
    );
}

LevelEnemySpawnDefinition MeleeFast(float x, float y) {
    return Enemy(
        EnemyType::Melee_Fast,
        x,
        y,
        MELEE_FAST,
		EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(150.0f))
    );
}

LevelEnemySpawnDefinition RangedBasic(float x, float y) {
    return Enemy(
        EnemyType::Ranged_Basic,
        x,
        y,
        RANGED_BASIC,
        EnemyMovementSequenceDefinition::Linear({
            EnemyMovementStepDefinition::Step(2.0f, EnemyMovementDefinition::Linear(0.0f, 50.0f)),
            EnemyMovementStepDefinition::Step(1.5f, EnemyMovementDefinition::SineWave(70.0f, 40.0f, 2.0f)),
            EnemyMovementStepDefinition::Step(0.0f, EnemyMovementDefinition::Linear(0.0f, 30.0f)),
        })
    );
}

LevelEnemySpawnDefinition MeleeSpawner(
    float x,
    float y,
    std::vector<LevelEnemySpawnDefinition> spawnEntries)
{
    LevelEnemySpawnDefinition spawner = Enemy(
        EnemyType::Melee_Spawner,
        x,
        y,
        MELEE_SPAWNER,
        EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(40.0f))
    );
    spawner.spawnEntries = std::move(spawnEntries);
    spawner.periodicSpawnInterval = 3.0f;
    spawner.deathSpawnCount = 3;
    return spawner;
}
}

std::vector<EnemyPhaseDefinition> LevelEnemyPool::Create(int levelIndex) {
    switch (levelIndex) {
    case 1:
        return CreateLevel1();
    case 2:
        return CreateLevel2();
    case 3:
        return CreateLevel3();
    case 4:
        return CreateLevel4();
    default:
        return CreateLevel1();
    }
}

std::vector<EnemyPhaseDefinition> LevelEnemyPool::CreateLevel1() {
    return {
        EnemyPhaseDefinition(3.0f, {
            MeleeBasic(100.0f, -50.0f),
            MeleeBasic(300.0f, -50.0f),
            MeleeBasic(500.0f, -50.0f),
            RangedBasic(300.0f, -100.0f),
            MeleeSpawner(500.0f, -50.0f, {
                MeleeFast(0.0f, 0.0f),
            }),
            MeleeSpawner(800.0f, -50.0f, {
                RangedBasic(0.0f, 0.0f),
            }),
        }),
        EnemyPhaseDefinition(10.0f, {
            RangedBasic(50.0f, -50.0f),
            RangedBasic(550.0f, -50.0f),
            MeleeFast(150.0f, -50.0f),
            MeleeFast(450.0f, -50.0f),
            MeleeBasic(300.0f, -80.0f),
        }),
        EnemyPhaseDefinition(20.0f, {
            MeleeBasic(50.0f, -50.0f),
            MeleeBasic(150.0f, -50.0f),
            MeleeFast(250.0f, -50.0f),
            MeleeFast(350.0f, -50.0f),
            MeleeBasic(450.0f, -50.0f),
            MeleeBasic(550.0f, -50.0f),
            RangedBasic(50.0f, -50.0f),
            RangedBasic(550.0f, -50.0f),
        }),
    };
}

std::vector<EnemyPhaseDefinition> LevelEnemyPool::CreateLevel2() {
    return {
        EnemyPhaseDefinition(1.0f, {
            MeleeFast(100.0f, -50.0f),
            MeleeFast(500.0f, -50.0f),
        }),
        EnemyPhaseDefinition(8.0f, {
            MeleeBasic(200.0f, -50.0f),
            MeleeBasic(400.0f, -50.0f),
            MeleeFast(300.0f, -80.0f),
            MeleeFast(100.0f, -80.0f),
            MeleeFast(500.0f, -80.0f),
        }),
        EnemyPhaseDefinition(15.0f, {
            MeleeBasic(80.0f, -50.0f),
            MeleeFast(180.0f, -50.0f),
            MeleeBasic(280.0f, -50.0f),
            MeleeFast(380.0f, -50.0f),
            MeleeBasic(480.0f, -50.0f),
            MeleeFast(580.0f, -50.0f),
        }),
        EnemyPhaseDefinition(25.0f, {
            MeleeFast(100.0f, -50.0f),
            MeleeFast(200.0f, -80.0f),
            MeleeFast(300.0f, -50.0f),
            MeleeFast(400.0f, -80.0f),
            MeleeFast(500.0f, -50.0f),
            MeleeBasic(300.0f, -120.0f),
            MeleeBasic(150.0f, -120.0f),
            MeleeBasic(450.0f, -120.0f),
        }),
    };
}

std::vector<EnemyPhaseDefinition> LevelEnemyPool::CreateLevel3() {
    return {
        EnemyPhaseDefinition(2.0f, {
            MeleeFast(50.0f, -50.0f),
            MeleeFast(550.0f, -50.0f),
            MeleeBasic(300.0f, -50.0f),
        }),
        EnemyPhaseDefinition(7.0f, {
            MeleeBasic(100.0f, -50.0f),
            MeleeBasic(200.0f, -80.0f),
            MeleeFast(300.0f, -50.0f),
            MeleeBasic(400.0f, -80.0f),
            MeleeBasic(500.0f, -50.0f),
        }),
        EnemyPhaseDefinition(14.0f, {
            MeleeFast(80.0f, -50.0f),
            MeleeFast(180.0f, -50.0f),
            MeleeBasic(280.0f, -80.0f),
            MeleeFast(380.0f, -50.0f),
            MeleeFast(480.0f, -50.0f),
            MeleeBasic(580.0f, -80.0f),
        }),
        EnemyPhaseDefinition(22.0f, {
            MeleeBasic(50.0f, -50.0f),
            MeleeFast(130.0f, -80.0f),
            MeleeBasic(210.0f, -50.0f),
            MeleeFast(290.0f, -80.0f),
            MeleeBasic(370.0f, -50.0f),
            MeleeFast(450.0f, -80.0f),
            MeleeBasic(530.0f, -50.0f),
            MeleeFast(300.0f, -120.0f),
        }),
    };
}

std::vector<EnemyPhaseDefinition> LevelEnemyPool::CreateLevel4() {
    return {
        EnemyPhaseDefinition(1.0f, {
            MeleeFast(100.0f, -50.0f),
            MeleeFast(300.0f, -50.0f),
            MeleeFast(500.0f, -50.0f),
        }),
        EnemyPhaseDefinition(6.0f, {
            MeleeBasic(100.0f, -50.0f),
            MeleeBasic(200.0f, -80.0f),
            MeleeFast(300.0f, -50.0f),
            MeleeBasic(400.0f, -80.0f),
            MeleeBasic(500.0f, -50.0f),
            MeleeFast(150.0f, -110.0f),
            MeleeFast(450.0f, -110.0f),
        }),
        EnemyPhaseDefinition(12.0f, {
            MeleeFast(50.0f, -50.0f),
            MeleeBasic(130.0f, -50.0f),
            MeleeFast(210.0f, -80.0f),
            MeleeBasic(290.0f, -50.0f),
            MeleeFast(370.0f, -80.0f),
            MeleeBasic(450.0f, -50.0f),
            MeleeFast(530.0f, -50.0f),
        }),
        EnemyPhaseDefinition(20.0f, {
            MeleeFast(60.0f, -50.0f),
            MeleeFast(140.0f, -80.0f),
            MeleeFast(220.0f, -50.0f),
            MeleeBasic(300.0f, -120.0f),
            MeleeFast(380.0f, -50.0f),
            MeleeFast(460.0f, -80.0f),
            MeleeFast(540.0f, -50.0f),
            MeleeBasic(200.0f, -150.0f),
            MeleeBasic(400.0f, -150.0f),
        }),
        EnemyPhaseDefinition(28.0f, {
            MeleeBasic(100.0f, -50.0f),
            MeleeBasic(200.0f, -50.0f),
            MeleeBasic(300.0f, -50.0f),
            MeleeBasic(400.0f, -50.0f),
            MeleeBasic(500.0f, -50.0f),
            MeleeFast(150.0f, -100.0f),
            MeleeFast(250.0f, -100.0f),
            MeleeFast(350.0f, -100.0f),
            MeleeFast(450.0f, -100.0f),
        }),
        EnemyPhaseDefinition(35.0f, {
            MeleeSpawner(300.0f, -80.0f, {
                MeleeFast(0.0f, 0.0f),
                MeleeBasic(0.0f, 0.0f),
            }),
        }),
    };
}
