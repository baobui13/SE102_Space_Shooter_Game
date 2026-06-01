#include "BaseEnemy.h"
#include "AttackMarker.h"
#include "GameContext.h"
#include "EntityManager.h" 
#include "Player.h"
#include <cmath>

BaseEnemy::BaseEnemy(float x, float y, float width, float height,
    float health, float moveSpeed, float attackPower,
    float attackSpeed, float attackRange, EnemyType type)
    : GameObject(x, y, width, height),
    m_health(health),
    m_maxHealth(health),
    m_moveSpeed(moveSpeed),
    m_attackPower(attackPower),
    m_attackSpeed(attackSpeed),
    m_attackRange(attackRange),
    m_isAttacking(false),
    m_attackCooldown(0.0f),
    m_type(type),
    m_expReward(10),
    m_rotation(0.0f),
    m_spriteForwardAngle(0.0f)
{
    SetColliderName("enemy");
}

void BaseEnemy::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    // 1. Cập nhật Animation
    UpdateAnimation(dt);

    // 2. Hồi chiêu tấn công
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // 3. Di chuyển (bao gồm tránh va chạm với quái khác)
    Move(dt, ctx);

    // 4. Tính toán góc xoay hướng về Player
    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;

    FacePoint(playerCX, playerCY);
}

void BaseEnemy::Render(Graphics& gfx) {
    m_anim.Render(gfx, m_x, m_y, m_width, m_height, DirectX::Colors::White, m_rotation);
}

void BaseEnemy::Move(float dt, GameContext& ctx) {
    if (m_movementStrategy) {
        float vx = 0.0f, vy = 0.0f;
        float centerX = m_x + m_width / 2.0f;
        float centerY = m_y + m_height / 2.0f;

        // Tính vận tốc từ Strategy (đuổi theo, bay hình sin...)
        m_movementStrategy->CalculateVelocity(centerX, centerY, dt, ctx, vx, vy);

        // --- Logic Separation: Giúp các quái vật đẩy nhau ra, không bị chồng khít ---
        float sepVx = 0.0f;
        float sepVy = 0.0f;
        float separationRadius = 40.0f; // Khoảng cách bắt đầu đẩy nhau
        float separationForce = 0.5f;   // Độ mạnh của lực đẩy

        auto& allEntities = ctx.entityManager.GetEntities();
        for (auto& entity : allEntities) {
            if (entity.get() == this || !entity->IsActive()) continue;

            // Kiểm tra xem thực thể đó có phải là Enemy không
            BaseEnemy* other = dynamic_cast<BaseEnemy*>(entity.get());
            if (other) {
                float dx = centerX - (other->GetX() + other->GetWidth() / 2.0f);
                float dy = centerY - (other->GetY() + other->GetHeight() / 2.0f);
                float distSq = dx * dx + dy * dy;

                if (distSq > 0 && distSq < separationRadius * separationRadius) {
                    float dist = std::sqrt(distSq);
                    float pushFactor = (separationRadius - dist) / separationRadius;
                    sepVx += (dx / dist) * pushFactor * separationForce;
                    sepVy += (dy / dist) * pushFactor * separationForce;
                }
            }
        }

        // Cộng vận tốc gốc và lực đẩy
        m_vx = vx + (sepVx * m_moveSpeed);
        m_vy = vy + (sepVy * m_moveSpeed);

        // Cập nhật vị trí
        m_x += m_vx * dt;
        m_y += m_vy * dt;

        if (m_x < 0.0f) m_x = 0.0f;
        if (m_y < 0.0f) m_y = 0.0f;
        if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
        if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;
    }
}

void BaseEnemy::Attack(GameObject* target) {
    if (m_attackCooldown <= 0 && target) {
        target->Destroy();
        m_isAttacking = true;
        m_attackCooldown = 1.0f / m_attackSpeed;
    }
}

void BaseEnemy::TakeDamage(float damage) {
    m_health -= damage;
    if (m_health <= 0) {
        m_health = 0;
        OnDeath();
    }
}

void BaseEnemy::OnDeath() {
    Destroy();
}

void BaseEnemy::UpdateAnimation(float dt) {
    m_anim.Update(dt);
}

void BaseEnemy::SetMovementStrategy(std::unique_ptr<IMovementStrategy> strategy) {
    m_movementStrategy = std::move(strategy);
}

void BaseEnemy::FacePoint(float targetX, float targetY) {
    float centerX = m_x + m_width / 2.0f;
    float centerY = m_y + m_height / 2.0f;

    float dx = targetX - centerX;
    float dy = targetY - centerY;

    m_rotation = std::atan2(dy, dx) - m_spriteForwardAngle;
}

void BaseEnemy::SpawnAttackMarker(GameContext& ctx, const AttackMarkerSpawnData& markerData) {
    ctx.entityManager.AddEntity(std::make_unique<AttackMarker>(markerData));
}

void BaseEnemy::UpdateRotationToMovement() {
    const float speedSq = m_vx * m_vx + m_vy * m_vy;
    if (speedSq > 0.001f) {
        m_rotation = std::atan2(m_vy, m_vx) - m_spriteForwardAngle;
    }
}

void BaseEnemy::SmoothRotationToward(float targetAngle, float dt, float maxAngularSpeed) {
    float delta = targetAngle - m_rotation;
    constexpr float TWO_PI = 6.283185307f;
    constexpr float PI = 3.14159265f;

    while (delta > PI) {
        delta -= TWO_PI;
    }
    while (delta < -PI) {
        delta += TWO_PI;
    }

    const float maxStep = maxAngularSpeed * dt;
    if (delta > maxStep) {
        delta = maxStep;
    } else if (delta < -maxStep) {
        delta = -maxStep;
    }

    m_rotation += delta;
}
