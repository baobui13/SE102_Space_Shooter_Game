#include "Boss1.h"
#include "EntityManager.h"
#include "ExpOrb.h"
#include "GameConfig.h"
#include "GameContext.h"
#include "Player.h"

Boss1::Boss1(float x, float y, float width, float height,
             float health, float moveSpeed, float attackPower,
             float attackSpeed, float attackRange, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
    m_x = x;
    m_y = -height + 40.0f;
    m_entryTargetY = 30.0f;
    m_enterSpeed = moveSpeed > 0.0f ? moveSpeed : 140.0f;
}

void Boss1::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    m_lastCtx = &ctx;

    UpdateAnimation(dt);
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    const float prevX = m_x;
    const float prevY = m_y;

    if (m_state == BossState::Entering) {
        m_y += m_enterSpeed * dt;
        m_vx = 0.0f;
        m_vy = m_enterSpeed;

        if (m_y >= m_entryTargetY) {
            m_y = m_entryTargetY;
            m_state = BossState::Patrolling;
        }

        HandlePlayerCombat(ctx, prevX, prevY);
        UpdateBossRotation(dt);
        return;
    }

    Move(dt, ctx);
    HandlePlayerCombat(ctx, prevX, prevY);
    UpdateBossRotation(dt);
}

void Boss1::HandlePlayerCombat(GameContext& ctx, float prevX, float prevY) {
    if (CheckCollision(ctx.player)) {
        m_x = prevX;
        m_y = prevY;
        m_vx = 0.0f;
        m_vy = 0.0f;
    }

    if (IsPlayerInContact(&ctx.player)) {
        Attack(&ctx.player);
    }
}

bool Boss1::IsPlayerInContact(const GameObject* player) const {
    if (!player) return false;

    constexpr float margin = 4.0f;
    const bool overlapX = (m_x - margin) < (player->GetX() + player->GetWidth()) &&
        (m_x + m_width + margin) > player->GetX();
    const bool overlapY = (m_y - margin) < (player->GetY() + player->GetHeight()) &&
        (m_y + m_height + margin) > player->GetY();
    return overlapX && overlapY;
}

void Boss1::Attack(GameObject* target) {
    if (m_attackCooldown <= 0 && target) {
        Player* player = dynamic_cast<Player*>(target);
        if (player) {
            player->TakeDamage(static_cast<int>(m_attackPower));
            m_isAttacking = true;
            m_attackCooldown = 1.0f / m_attackSpeed;
        }
    }
}

void Boss1::OnDeath() {
    if (m_lastCtx && m_expReward > 0) {
        const float orbX = m_x + m_width * 0.5f;
        const float orbY = m_y + m_height * 0.5f;
        m_lastCtx->entityManager.AddEntity(
            std::make_unique<ExpOrb>(m_lastCtx->gfx, orbX, orbY, m_expReward));
    }
    BaseEnemy::OnDeath();
}

void Boss1::UpdateBossRotation(float dt) {
    const float speedSq = m_vx * m_vx + m_vy * m_vy;
    if (speedSq > 0.001f) {
        const float targetAngle = std::atan2(m_vy, m_vx) - m_spriteForwardAngle;
        SmoothRotationToward(targetAngle, dt, ROTATION_SMOOTH_SPEED);
    }
}

void Boss1::Move(float dt, GameContext& ctx) {
    if (!m_movementStrategy) {
        return;
    }

    float vx = 0.0f;
    float vy = 0.0f;
    const float centerX = m_x + m_width * 0.5f;
    const float centerY = m_y + m_height * 0.5f;

    m_movementStrategy->CalculateVelocity(centerX, centerY, dt, ctx, vx, vy);
    m_vx = vx;
    m_vy = vy;

    m_x += m_vx * dt;
    m_y += m_vy * dt;

    if (m_x < 0.0f) m_x = 0.0f;
    if (m_y < 0.0f) m_y = 0.0f;
    if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
    if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;
}
