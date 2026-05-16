#include "RangeEnemy1.h"
#include "GameContext.h"
#include "Player.h"
#include "BulletPool.h"
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

    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
    float myCX = m_x + m_width / 2.0f;
    float myCY = m_y + m_height / 2.0f;

    float dx = playerCX - myCX;
    float dy = playerCY - myCY;
    float dist = std::sqrt(dx * dx + dy * dy);

    switch (m_state) {
    case RangeState::Moving:
        UpdateRotationToPlayer(ctx);

        if (dist < SAFE_DISTANCE) {
            float moveDirX = -dx / dist;
            float moveDirY = -dy / dist;
            m_x += moveDirX * m_moveSpeed * dt;
            m_y += moveDirY * m_moveSpeed * dt;
        }
        else {
            Move(dt, ctx);
        }

        if (m_stateTimer >= TIME_MOVE) {
            float aimAngle = GetAimAngle();
            SpawnAttackMarker(
                ctx,
                AttackMarkerSpawnData::Line(
                    AttackMarkerType::DangerLine,
                    myCX,
                    myCY,
                    myCX + std::cos(aimAngle) * 2000.0f,
                    myCY + std::sin(aimAngle) * 2000.0f,
                    TIME_AIM
                )
            );
            ResetState(RangeState::Aiming);
        }
        break;

    case RangeState::Aiming:
        UpdateRotationToPlayer(ctx);

        if (m_stateTimer >= TIME_AIM) {
            ResetState(RangeState::Shooting);
        }
        break;

    case RangeState::Shooting:
        if (m_stateTimer >= TIME_SHOOT) {
            float aimAngle = GetAimAngle();
            float targetX = myCX + std::cos(aimAngle) * 100.0f;
            float targetY = myCY + std::sin(aimAngle) * 100.0f;

            ctx.bulletPool.GetBullet(
                myCX, myCY,
                targetX, targetY,
                BULLET_SPEED,
                (int)m_attackPower,
                m_attackRange
            );

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
