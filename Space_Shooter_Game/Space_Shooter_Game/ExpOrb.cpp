#include "ExpOrb.h"
#include "GameContext.h"
#include "Player.h"
#include "AssetManager.h"
#include <cmath>

ExpOrb::ExpOrb(Graphics& gfx, float x, float y, int expValue)
    : GameObject(x, y, 16.0f, 16.0f), m_expAmount(expValue),
    m_magnetSpeed(450.0f)
{
    m_vx = 0.0f;
    m_vy = 50.0f;

    // Sửa về texture đúng
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Bullets1.png"));
    m_anim.AddClip("Idle", 15, 80, 17, 17, 1, 1, 1.0f, true);
    m_anim.Play("Idle");
}

void ExpOrb::Update(float dt, ::GameContext& ctx) {
    // 1. Tính toán tọa độ và khoảng cách
    float playerCenterX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCenterY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
    float orbCenterX = m_x + m_width / 2.0f;
    float orbCenterY = m_y + m_height / 2.0f;

    float dx = playerCenterX - orbCenterX;
    float dy = playerCenterY - orbCenterY;
    float distance = std::sqrt(dx * dx + dy * dy);

    // 2. Cập nhật vận tốc m_vx, m_vy (Nam châm)
    if (distance < ctx.player.GetMagnetRange() && distance > 0.0f) {
        m_vx = (dx / distance) * m_magnetSpeed;
        m_vy = (dy / distance) * m_magnetSpeed;
    }
    else {
        m_vx = 0.0f;
        m_vy = 50.0f;
    }

    // 3. DI CHUYỂN (Gọi hàm cha sau khi đã có vận tốc mới)
    GameObject::Update(dt, ctx);

    // 4. Va chạm (Ăn ngọc) và Dọn rác
    if (distance < 30.0f) {
        ctx.player.GainExp(m_expAmount);
        Destroy();
    }
    else if (m_y > ctx.screenHeight + 100.0f) {
        Destroy();
    }
}