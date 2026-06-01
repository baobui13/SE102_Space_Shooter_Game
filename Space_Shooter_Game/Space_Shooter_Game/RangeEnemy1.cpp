#include "RangeEnemy1.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

RangeEnemy1::RangeEnemy1(float x, float y, float width, float height,
    float health, float moveSpeed, float attackPower,
    float attackSpeed, float attackRange, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
    m_state = RangeState::Moving;
    m_stateTimer = 0.0f;
    m_attackCooldown = 1.0f;
}

void RangeEnemy1::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    UpdateAnimation(dt);
    m_stateTimer += dt;

    float myCX = m_x + m_width / 2.0f;
    float myCY = m_y + m_height / 2.0f;

    switch (m_state) {
    case RangeState::Moving:
        Move(dt, ctx);
        UpdateRotationToMovement();

        if (m_stateTimer >= TIME_MOVE) {
            UpdateRotationToPlayer(ctx);
            SpawnAttackMarker(
                ctx,
                AttackMarkerSpawnData::FollowPlayerLine(
                    AttackMarkerType::DangerLine,
                    myCX,
                    myCY,
                    TIME_MARKER
                )
            );
            ResetState(RangeState::Marking);
        }
        break;

    case RangeState::Marking:
        UpdateRotationToPlayer(ctx);

        if (m_stateTimer >= TIME_MARKER) {
            m_lockedAimAngle = GetAimAngle();
            ResetState(RangeState::WaitingAttack);
        }
        break;

    case RangeState::WaitingAttack:
        m_rotation = m_lockedAimAngle - m_spriteForwardAngle;

        if (m_stateTimer >= TIME_WAIT_ATTACK) {
            FireLockedShot(ctx);
            ResetState(RangeState::Moving);
        }
        break;
    }
}

void RangeEnemy1::Render(Graphics& gfx) {
    BaseEnemy::Render(gfx);
}

void RangeEnemy1::ResetState(RangeState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

void RangeEnemy1::UpdateRotationToPlayer(GameContext& ctx) {
    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;

    FacePoint(playerCX, playerCY);
}

void RangeEnemy1::UpdateRotationToMovement() {
    float speedSq = m_vx * m_vx + m_vy * m_vy;
    if (speedSq > 0.001f) {
        m_rotation = std::atan2(m_vy, m_vx) - m_spriteForwardAngle;
    }
}

void RangeEnemy1::FireLockedShot(GameContext& ctx) {
    float myCX = m_x + m_width / 2.0f;
    float myCY = m_y + m_height / 2.0f;
    float dirX = std::cos(m_lockedAimAngle);
    float dirY = std::sin(m_lockedAimAngle);
    float spawnDistance = (m_width > m_height ? m_width : m_height) * 0.5f + BULLET_DISPLAY_SIZE * 0.5f + 4.0f;
    float spawnX = myCX + dirX * spawnDistance - BULLET_DISPLAY_SIZE * 0.5f;
    float spawnY = myCY + dirY * spawnDistance - BULLET_DISPLAY_SIZE * 0.5f;
    float targetX = spawnX + dirX * 100.0f;
    float targetY = spawnY + dirY * 100.0f;

    Bullet* bullet = ctx.bulletPool.GetEnemyBullet(
        spawnX,
        spawnY,
        targetX,
        targetY,
        BULLET_SPEED,
        (int)m_attackPower,
        BULLET_MAX_DISTANCE
    );

    if (bullet) {
        bullet->SetAnimationById("bullet_enemy_fire");
    }
}
