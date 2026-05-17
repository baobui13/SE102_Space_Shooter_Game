#include "RangeEnemy2.h"
#include "AttackMarkerDefinitions.h"
#include "CircleGroundAttack.h"
#include "EntityManager.h"
#include "GameContext.h"
#include "Player.h"

RangeEnemy2::RangeEnemy2(float x, float y, float width, float height,
                         float health, float moveSpeed, float attackPower,
                         float attackSpeed, float attackRange, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
}

void RangeEnemy2::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    UpdateAnimation(dt);
    Move(dt, ctx);

    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() * 0.5f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() * 0.5f;
    FacePoint(playerCX, playerCY);

    m_attackTimer += dt;
    if (m_attackTimer >= ATTACK_INTERVAL) {
        LaunchCircleAttack(ctx);
        m_attackTimer = 0.0f;
    }
}

void RangeEnemy2::LaunchCircleAttack(GameContext& ctx) {
    float targetX = ctx.player.GetX() + ctx.player.GetWidth() * 0.5f;
    float targetY = ctx.player.GetY() + ctx.player.GetHeight() * 0.5f;

    SpawnAttackMarker(
        ctx,
        AttackMarkerSpawnData::Circle(
            AttackMarkerType::DangerCircle,
            targetX,
            targetY,
            ATTACK_RADIUS,
            MARKER_DURATION
        )
    );

    ctx.entityManager.AddEntity(std::make_unique<CircleGroundAttack>(
        ctx.gfx,
        targetX,
        targetY,
        ATTACK_RADIUS,
        static_cast<int>(m_attackPower),
        MARKER_DURATION
    ));
}
