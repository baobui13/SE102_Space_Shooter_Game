#include "EnemyBullet.h"
#include <DirectXMath.h>

EnemyBullet::EnemyBullet(Graphics& gfx)
	:GameObject(0, 0, 16.0f, 16.0f), m_

EnemyBullet::EnemyBullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance)
    : GameObject(startX, startY, 16.0f, 16.0f), 
    m_damage(damage), 
    m_maxDistance(maxDistance), 
    m_distanceTraveled(0.0f), 
    m_isActive(true) {

    // Tính toán hướng di chuyển (normalize hướng di chuyển từ điểm bắt đầu đến điểm đích)
    float dx = targetX - startX;
    float dy = targetY - startY;
    float length = sqrt(dx * dx + dy * dy);
    m_direction = { dx / length, dy / length, 0.0f }; // Hướng di chuyển (normalize)
}

void EnemyBullet::Update(float dt, ::GameContext& ctx) {
    if (!m_isActive) return;

    // Cập nhật vị trí của đạn
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;
    m_distanceTraveled += m_speed * dt;

    // Kiểm tra nếu đạn đã bay hết khoảng cách tối đa
    if (m_distanceTraveled >= m_maxDistance) {
        Deactivate(); 
    }
}

bool EnemyBullet::CheckCollision(const GameObject& target) {

    return false;
}