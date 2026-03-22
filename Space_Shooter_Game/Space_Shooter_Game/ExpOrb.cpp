#include "ExpOrb.h"
#include "AssetManager.h"
#include <cmath>

ExpOrb::ExpOrb(Graphics& gfx, float x, float y, int expValue)
    : GameObject(x, y, 16.0f, 16.0f), m_expAmount(expValue),
    m_magnetSpeed(450.0f) // Xóa dòng m_magnetRange ở đây
{
    m_vx = 0.0f;
    m_vy = 50.0f;

    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Bullets1.png"));
    m_anim.AddClip("Idle", 15, 80, 17, 17, 1, 1, 1.0f, true);
    m_anim.Play("Idle");
}

void ExpOrb::Update(float dt) {}

// CẬP NHẬT: Sử dụng playerMagnetRange
void ExpOrb::Update(float dt, float playerX, float playerY, float playerMagnetRange) {
    float orbCenterX = m_x + m_width / 2.0f;
    float orbCenterY = m_y + m_height / 2.0f;

    float dx = playerX - orbCenterX;
    float dy = playerY - orbCenterY;
    float distance = std::sqrt(dx * dx + dy * dy);

    // THAY THẾ m_magnetRange bằng playerMagnetRange
    if (distance < playerMagnetRange && distance > 0.0f) {
        dx /= distance;
        dy /= distance;
        m_vx = dx * m_magnetSpeed;
        m_vy = dy * m_magnetSpeed;
    }
    else {
        m_vx = 0.0f;
        m_vy = 50.0f;
    }

    GameObject::Update(dt);

    if (m_y > 1000.0f) {
        Destroy();
    }
}