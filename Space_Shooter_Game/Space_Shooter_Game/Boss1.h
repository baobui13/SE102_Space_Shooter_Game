#pragma once
#include "BaseEnemy.h"

enum class BossState {
    Entering,
    Patrolling
};

class Boss1 : public BaseEnemy {
public:
    Boss1(float x, float y, float width, float height,
          float health, float moveSpeed, float attackPower,
          float attackSpeed = 1.0f, float attackRange = 600.0f,
          EnemyType type = EnemyType::Boss_Stage1);

    void Update(float dt, GameContext& ctx) override;
    void Move(float dt, GameContext& ctx) override;
    void Attack(GameObject* target) override;
    void TakeDamage(float damage) override;
    void OnDeath() override;

private:
    void UpdateBossRotation(float dt);
    void HandlePlayerCombat(GameContext& ctx, float prevX, float prevY);
    bool IsPlayerInContact(const GameObject* player) const;
    bool IsPlayerInAttackRange(const GameObject* player) const;
    void FireBossBulletRing(GameContext& ctx);

    BossState m_state = BossState::Entering;
    float m_entryTargetY = 0.0f;
    float m_enterSpeed = 140.0f;
    GameContext* m_lastCtx = nullptr;
    bool m_isDying = false;
};
