#pragma once
#include "BaseEnemy.h"

enum class RangeState {
    Moving,
    Marking,
    WaitingAttack
};

class RangeEnemy1 : public BaseEnemy {
private:
    RangeState m_state = RangeState::Moving;
    float m_stateTimer = 0.0f;
    float m_lockedAimAngle = 0.0f;

    const float TIME_MOVE = 3.0f;
    const float TIME_MARKER = 1.0f;
    const float TIME_WAIT_ATTACK = 1.0f;
    const float BULLET_SPEED = 900.0f;
    const float BULLET_MAX_DISTANCE = 2000.0f;
    const float BULLET_DISPLAY_SIZE = 40.0f;

public:
    RangeEnemy1(float x, float y, float width, float height,
        float health, float moveSpeed, float attackPower,
        float attackSpeed = 1.0f, float attackRange = 500.0f,
        EnemyType type = EnemyType::Ranged_Basic);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;

private:
    void ResetState(RangeState newState);
    void UpdateRotationToPlayer(GameContext& ctx);
    void UpdateRotationToMovement();
    void FireLockedShot(GameContext& ctx);
};
