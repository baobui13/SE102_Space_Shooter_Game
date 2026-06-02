#include "Boss1.h"
#include "AnimationManager.h"
#include "AudioManager.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "EntityManager.h"
#include "ExpOrb.h"
#include "GameConfig.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

namespace {
constexpr float BOSS_BULLET_SPEED = 260.0f;
constexpr float BOSS_BULLET_MAX_DISTANCE = 900.0f;
constexpr float BOSS_BULLET_DISPLAY_SIZE = 40.0f;
constexpr int BOSS_BULLET_RING_COUNT = 16;
constexpr float TWO_PI = 6.283185307f;
constexpr const char* BOSS_BULLET_ANIMATION = "bullet_boss_green";
constexpr const char* BOSS_DEATH_ANIMATION = "boss_stage1_dead";
}

Boss1::Boss1(float x, float y, float width, float height,
             float health, float moveSpeed, float attackPower,
             float attackSpeed, float attackRange, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
    SetColliderName("boss");
    m_x = x;
    m_y = -height + 40.0f;
    m_entryTargetY = 30.0f;
    m_enterSpeed = moveSpeed > 0.0f ? moveSpeed : 140.0f;
}

void Boss1::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    m_lastCtx = &ctx;

    if (m_isDying) {
        UpdateAnimation(dt);
        if (m_anim.IsFinished()) {
            Destroy();
        }
        return;
    }

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

    if (IsPlayerInAttackRange(&ctx.player)) {
        Attack(&ctx.player);
    }
}

bool Boss1::IsPlayerInContact(const GameObject* player) const {
    if (!player) return false;

    const Collider contactCollider = ColliderRegistry::GetInstance().CreateCollider(
        "melee_contact",
        m_x,
        m_y,
        m_width,
        m_height);
    return player->CheckCollision(contactCollider);
}

bool Boss1::IsPlayerInAttackRange(const GameObject* player) const {
    if (!player) return false;

    const float bossCenterX = m_x + m_width * 0.5f;
    const float bossCenterY = m_y + m_height * 0.5f;
    const float playerCenterX = player->GetX() + player->GetWidth() * 0.5f;
    const float playerCenterY = player->GetY() + player->GetHeight() * 0.5f;
    const float dx = playerCenterX - bossCenterX;
    const float dy = playerCenterY - bossCenterY;
    return dx * dx + dy * dy <= m_attackRange * m_attackRange;
}

void Boss1::Attack(GameObject* target) {
    if (m_attackCooldown <= 0 && target) {
        if (m_lastCtx) {
            FireBossBulletRing(*m_lastCtx);
            m_isAttacking = true;
            m_attackCooldown = 1.0f / m_attackSpeed;
        }
    }
}

void Boss1::FireBossBulletRing(GameContext& ctx) {
    const float bossCenterX = m_x + m_width * 0.5f;
    const float bossCenterY = m_y + m_height * 0.5f;

    for (int i = 0; i < BOSS_BULLET_RING_COUNT; ++i) {
        const float angle = static_cast<float>(i) * TWO_PI / static_cast<float>(BOSS_BULLET_RING_COUNT);
        const float dirX = std::cos(angle);
        const float dirY = std::sin(angle);
        const float spawnX = bossCenterX - BOSS_BULLET_DISPLAY_SIZE * 0.5f;
        const float spawnY = bossCenterY - BOSS_BULLET_DISPLAY_SIZE * 0.5f;
        const float targetX = spawnX + dirX * 100.0f;
        const float targetY = spawnY + dirY * 100.0f;

        Bullet* bullet = ctx.bulletPool.GetEnemyBullet(
            spawnX,
            spawnY,
            targetX,
            targetY,
            BOSS_BULLET_SPEED,
            static_cast<int>(m_attackPower),
            BOSS_BULLET_MAX_DISTANCE);

        if (bullet) {
            bullet->SetAnimationById(BOSS_BULLET_ANIMATION);
        }
    }
}

void Boss1::TakeDamage(float damage) {
    if (m_isDying) {
        return;
    }

    BaseEnemy::TakeDamage(damage);
}

void Boss1::OnDeath() {
    if (m_isDying) {
        return;
    }

    m_isDying = true;
    m_vx = 0.0f;
    m_vy = 0.0f;
    m_attackCooldown = 0.0f;
    m_isAttacking = false;
    m_damageFlashTimer = 0.0f;
    if (!AnimationManager::GetInstance().PlayAnimation(BOSS_DEATH_ANIMATION, m_anim)) {
        Destroy();
        return;
    }
    AudioManager::GetInstance().PlaySoundEffect(AudioIds::BossDeath);

    if (m_lastCtx && m_expReward > 0) {
        const float orbX = m_x + m_width * 0.5f;
        const float orbY = m_y + m_height * 0.5f;
        m_lastCtx->entityManager.AddEntity(
            std::make_unique<ExpOrb>(m_lastCtx->gfx, orbX, orbY, m_expReward));
    }
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
