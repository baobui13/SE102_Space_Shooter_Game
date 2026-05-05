#include "MeleeEnemy1.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

MeleeEnemy1::MeleeEnemy1(float x, float y, float width, float height,
                       float health, float moveSpeed, float attackPower, float attackRange)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, 1.0f, attackRange, EnemyType::Melee_Basic)
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

    float dx = playerCX - centerX;
    float dy = playerCY - centerY;

    const float PI = 3.1415926535f;
    m_rotation = std::atan2(dy, dx) - (PI / 2.0f);

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
    // Kiểm tra chạm bằng AABB overlap với margin nhỏ (contact damage)
    float margin = 4.0f;
    bool overlapX = (m_x - margin) < (player->GetX() + player->GetWidth()) &&
                    (m_x + m_width + margin) > player->GetX();
    bool overlapY = (m_y - margin) < (player->GetY() + player->GetHeight()) &&
                    (m_y + m_height + margin) > player->GetY();
    return overlapX && overlapY;
}
