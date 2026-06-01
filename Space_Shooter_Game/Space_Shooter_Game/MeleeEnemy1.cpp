#include "MeleeEnemy1.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

MeleeEnemy1::MeleeEnemy1(float x, float y, float width, float height,
                       float health, float moveSpeed, float attackPower, float attackRange,
                       float attackSpeed, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
}

void MeleeEnemy1::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;
    UpdateAnimation(dt);
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // Lưu vị trí trước khi di chuyển
    float prevX = m_x;
    float prevY = m_y;

    // Di chuyển theo strategy
    Move(dt, ctx);

    float centerX = m_x + m_width / 2.0f;
    float centerY = m_y + m_height / 2.0f;

    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;

    FacePoint(playerCX, playerCY);

    if (CheckCollision(ctx.player)) {
        m_x = prevX;
        m_y = prevY;
        m_vx = 0.0f;
        m_vy = 0.0f;
    }

    if (IsPlayerInRange(&ctx.player)) {
        Attack(&ctx.player);
    }
}

void MeleeEnemy1::Attack(GameObject* target) {
    if (m_attackCooldown <= 0 && target) {
        // Cast target về Player để gọi TakeDamage
        Player* player = dynamic_cast<Player*>(target);
        if (player) {
            player->TakeDamage((int)m_attackPower);
            m_isAttacking = true;
            m_attackCooldown = 1.0f / m_attackSpeed;
        }
    }
}

bool MeleeEnemy1::IsPlayerInRange(GameObject* player) const {
    if (!player) return false;
    const Collider contactCollider = ColliderRegistry::GetInstance().CreateCollider(
        "melee_contact",
        m_x,
        m_y,
        m_width,
        m_height);
    return player->CheckCollision(contactCollider);
}
