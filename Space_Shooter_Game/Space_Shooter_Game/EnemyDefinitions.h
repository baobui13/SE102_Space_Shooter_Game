#pragma once
#include "BaseEnemy.h"
#include <string>
#include <utility>
#include <vector>

constexpr float ENEMY_PI = 3.1415926535f;

enum class SpriteHeadDirection {
    Right,
    Down,
    Left,
    Up
};

inline float SpriteHeadDirectionToRadians(SpriteHeadDirection direction) {
    switch (direction) {
    case SpriteHeadDirection::Right:
        return 0.0f;
    case SpriteHeadDirection::Down:
        return ENEMY_PI / 2.0f;
    case SpriteHeadDirection::Left:
        return ENEMY_PI;
    case SpriteHeadDirection::Up:
        return -ENEMY_PI / 2.0f;
    default:
        return 0.0f;
    }
}

struct EnemyVisualDefinition {
    EnemyType type;
    float displayWidth;
    float displayHeight;
    // ID animation trong AnimationManager (ví dụ: "melee_basic_idle")
    // Object chỉ lưu tên, không chứa thông số frame/texture.
    std::string animationId;
    float spriteForwardAngle;
};

struct EnemyStatsDefinition {
    float health;
    float moveSpeed;
    float attackPower;
    float attackSpeed;
    float attackRange;
    int expReward;
    float talentDropChance = 0.0f;
    int talentPointReward = 1;
};

enum class EnemyMovementKind {
    Chase,
    Linear,
    SineWave,
    Circular,
    FigureEight
};

struct EnemyMovementDefinition {
    EnemyMovementKind kind;
    float a;
    float b;
    float c;
    float d;

    static EnemyMovementDefinition Chase(float speed) {
        return { EnemyMovementKind::Chase, speed, 0.0f, 0.0f, 0.0f };
    }

    static EnemyMovementDefinition Linear(float speedX, float speedY) {
        return { EnemyMovementKind::Linear, speedX, speedY, 0.0f, 0.0f };
    }

    static EnemyMovementDefinition SineWave(float speedY, float amplitude, float frequency) {
        return { EnemyMovementKind::SineWave, speedY, amplitude, frequency, 0.0f };
    }

    static EnemyMovementDefinition Circular(float centerX, float centerY, float radius, float angularSpeed) {
        return { EnemyMovementKind::Circular, centerX, centerY, radius, angularSpeed };
    }

    static EnemyMovementDefinition FigureEight(float centerX, float centerY, float radiusX, float radiusY) {
        return { EnemyMovementKind::FigureEight, centerX, centerY, radiusX, radiusY };
    }
};

struct EnemyMovementStepDefinition {
    float duration;
    EnemyMovementDefinition movement;

    static EnemyMovementStepDefinition Step(float durationSeconds, EnemyMovementDefinition movementDefinition) {
        return { durationSeconds, movementDefinition };
    }
};

enum class EnemyMovementSequenceMode {
    Linear,
    Loop
};

struct EnemyMovementSequenceDefinition {
    EnemyMovementSequenceMode mode;
    std::vector<EnemyMovementStepDefinition> steps;

    // Tránh player: kích hoạt khi player trong fleeTriggerDistance (0 = tắt)
    float fleeTriggerDistance = 0.0f;
    float fleeSpeed = 0.0f; // 0 = dùng moveSpeed của enemy

    static EnemyMovementSequenceDefinition Single(EnemyMovementDefinition movement) {
        return {
            EnemyMovementSequenceMode::Linear,
            { EnemyMovementStepDefinition::Step(0.0f, movement) }
        };
    }

    static EnemyMovementSequenceDefinition Linear(std::vector<EnemyMovementStepDefinition> movementSteps) {
        return { EnemyMovementSequenceMode::Linear, std::move(movementSteps) };
    }

    static EnemyMovementSequenceDefinition Loop(std::vector<EnemyMovementStepDefinition> movementSteps) {
        return { EnemyMovementSequenceMode::Loop, std::move(movementSteps) };
    }

    static EnemyMovementSequenceDefinition WithFleeFromPlayer(
        EnemyMovementSequenceDefinition sequence,
        float triggerDistance,
        float fleeSpeed = 0.0f)
    {
        sequence.fleeTriggerDistance = triggerDistance;
        sequence.fleeSpeed = fleeSpeed;
        return sequence;
    }

    // Boss: vòng tròn quanh màn hình rồi hình số 8, lặp vô hạn
    static EnemyMovementSequenceDefinition BossPatrol(float screenWidth, float screenHeight) {
        const float centerX = screenWidth * 0.5f;
        const float centerY = screenHeight * 0.5f;
        const float screenMin = screenWidth < screenHeight ? screenWidth : screenHeight;
        const float orbitRadius = screenMin * 0.30f;

        return Loop({
            EnemyMovementStepDefinition::Step(
                10.0f,
                EnemyMovementDefinition::Circular(centerX, centerY, orbitRadius, 0.42f)),
            EnemyMovementStepDefinition::Step(
                12.0f,
                EnemyMovementDefinition::FigureEight(
                    centerX,
                    centerY,
                    screenWidth * 0.34f,
                    screenHeight * 0.22f)),
        });
    }
};

struct LevelEnemySpawnDefinition {
    EnemyType type;
    float x;
    float y;
    EnemyStatsDefinition stats;
    EnemyMovementSequenceDefinition movementSequence;

    // Cấu hình cho Melee_Spawner (MeleeEnemy2): danh sách enemy được sinh ra
    std::vector<LevelEnemySpawnDefinition> spawnEntries;
    float periodicSpawnInterval = 3.0f;
    int deathSpawnCount = 3;
};

struct EnemyPhaseDefinition {
    float triggerTime;
    std::vector<LevelEnemySpawnDefinition> enemies;
    bool triggered = false;

    EnemyPhaseDefinition(float time, std::vector<LevelEnemySpawnDefinition> enemyEntries)
        : triggerTime(time), enemies(std::move(enemyEntries)) {}
};
