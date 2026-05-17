#pragma once
#include "BaseEnemy.h"

class RangeEnemy2 : public BaseEnemy {
public:
    RangeEnemy2(float x, float y, float width, float height,
                float health, float moveSpeed, float attackPower,
                float attackSpeed = 1.0f, float attackRange = 500.0f,
                EnemyType type = EnemyType::Ranged_Burst);

    void Update(float dt, GameContext& ctx) override;

private:
    void LaunchCircleAttack(GameContext& ctx);

    float m_attackTimer = 0.0f;

    static constexpr float ATTACK_INTERVAL = 3.0f;
    static constexpr float MARKER_DURATION = 2.0f;
    static constexpr float ATTACK_RADIUS = 100.0f;
};
