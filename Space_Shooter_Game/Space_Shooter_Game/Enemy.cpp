#include "Enemy.h"
#include <DirectXMath.h>

Enemy::Enemy(Graphics& gfx, float startX, float startY, int health, float speed)
    : GameObject(startX, startY, 50.0f, 50.0f), 
    m_health(health), 
    m_speed(speed), 
    m_isAlive(true), 
    m_shootCooldown(1.0f), 
    m_timeSinceLastShot(0.0f) {
    // Khởi tạo các thuộc tính của kẻ địch
}

void Enemy::Render(Graphics& gfx) {
    GameObject::Render(gfx);
}

void Enemy::Update(float dt, ::GameContext& ctx) {
    if (!m_isAlive) return;

    // Di chuyển kẻ địch từ trên xuống
    m_position.y += m_speed * dt;

    // Kiểm tra cooldown để bắn đạn
    m_timeSinceLastShot += dt;
    if (m_timeSinceLastShot >= m_shootCooldown) {
        // Bắn đạn nếu đủ thời gian
        Shoot(m_position.x, m_position.y + 1.0f, 5.0f, 10, 50.0f);
        m_timeSinceLastShot = 0.0f;
    }

    // Cập nhật tất cả đạn của kẻ địch
    for (auto& bullet : m_bullets) {
        bullet.Update(dt, ctx);
    }
}

bool Enemy::CheckCollision(const GameObject& target) {
    // Kiểm tra va chạm giữa kẻ địch và đối tượng khác (ví dụ: Player)
    return false;  
}

void Enemy::Shoot(float targetX, float targetY, float speed, int damage, float maxDistance) {
    //// Tạo đạn mới và thêm vào danh sách đạn của kẻ địch
    //EnemyBullet newBullet(m_gfx, m_x, m_y, targetX, targetY, speed, damage, maxDistance);
    //m_bullets.push_back(newBullet);
}

void Enemy::TakeDamage(int damage) {
    m_health -= damage;
    if (m_health <= 0) {
        m_isAlive = false;  // Kẻ địch chết khi hp giảm về 0
    }
}