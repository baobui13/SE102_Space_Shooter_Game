#include "Bullet.h"
#include "GameContext.h"
#include "AssetManager.h"
#include <cmath>

Bullet::Bullet(Graphics& gfx)
    : GameObject(0, 0, 16.0f, 16.0f), m_damage(0), m_maxDistance(0), m_distanceTraveled(0)
{
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Bullets1.png"));
    m_anim.AddClip("Fly", 15, 15, 17, 17, 1, 1, 1.0f, true);
    m_anim.Play("Fly");
}

Bullet::Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance)
    : Bullet(gfx)
{
    ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance);
}

void Bullet::ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance) {
    m_x = startX;
    m_y = startY;
    m_damage = damage;
    m_maxDistance = maxDistance;
    m_distanceTraveled = 0.0f;
    m_isActive = true;

    float dx = targetX - startX;
    float dy = targetY - startY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance != 0.0f) {
        dx /= distance;
        dy /= distance;
    }
    else {
        dx = 0.0f;
        dy = 1.0f;
    }

    m_vx = dx * speed;
    m_vy = dy * speed;
}

void Bullet::Update(float dt, ::GameContext& ctx) {
    // Kế thừa logic di chuyển mặc định từ GameObject (m_x += m_vx * dt)
    GameObject::Update(dt, ctx);

    // TÍNH TOÁN QUÃNG ĐƯỜNG ĐÃ BAY
    float speed = std::sqrt(m_vx * m_vx + m_vy * m_vy);
    m_distanceTraveled += speed * dt;

    // DỌN RÁC : Xóa đạn nếu bay quá tầm BÁN KÍNH cho phép
    if (m_distanceTraveled >= m_maxDistance) {
        Destroy();
    }

    // LOGIC DỌN RÁC: Xóa viên đạn nếu nó bay khỏi màn hình
    if (m_x < -100.0f || m_x > 900.0f || m_y < -100.0f || m_y > 1100.0f) {
        Destroy();
    }
}
