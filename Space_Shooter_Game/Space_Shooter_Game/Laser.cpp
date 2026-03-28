#include "Laser.h"
#include "GameContext.h"
#include "Player.h"
#include "AssetManager.h"
#include <DirectXMath.h>
#include <DirectXColors.h>

Laser::Laser(Graphics& gfx, float duration, int damage, float sizeMultiplier)
    : GameObject(0, 0, 0, 0), m_duration(duration), m_damage(damage), m_sizeMultiplier(sizeMultiplier) {
    m_texture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/lazer_1.png");
    
    // Tọa độ body (phần trụ)
    m_bodyRect.left = 37;
    m_bodyRect.top = 2;
    m_bodyRect.right = 48; 
    m_bodyRect.bottom = 17; 

    // Tọa độ impact (tỏa ra)
    m_impactRect.left = 2;
    m_impactRect.top = 1;
    m_impactRect.right = 33; 
    m_impactRect.bottom = 18; 
}

void Laser::Update(float dt, ::GameContext& ctx) {
    if (!m_isActive) return;

    m_duration -= dt;
    if (m_duration <= 0.0f) {
        Destroy();
        return;
    }

    // Bám theo người chơi: vị trí súng của người chơi (chính giữa trên cùng)
    m_x = ctx.player.GetX() + (ctx.player.GetWidth() / 2.0f);
    m_y = ctx.player.GetY() + 10.0f; // Thụt xuống 1 tí để khớp với nòng súng
}

void Laser::Render(Graphics& gfx) {
    if (!m_isActive) return;

    auto spriteBatch = gfx.GetSpriteBatch();
    
    // Bỏ qua nếu texture bị null
    if (!m_texture) return;

    // Khoảng cách từ đầu súng đến mép trên màn hình (Y = 0)
    float distance = m_y; 

    float bodyWidth = (float)(m_bodyRect.right - m_bodyRect.left);
    float bodyHeight = (float)(m_bodyRect.bottom - m_bodyRect.top);

    // Kéo chiều dài theo trục Y, độ dày theo trục X
    float scaleX = 5.0f * m_sizeMultiplier; // Độ dày tia sáng scale theo kích thước kỹ năng của Player
    float scaleY = distance / bodyHeight;
    
    DirectX::XMFLOAT2 scale(scaleX, scaleY);

    // Xoay 0 độ vì phần cắt ảnh đã nằm dọc sẵn
    float rotation = 0.0f; 

    // Origin của body: (ở giữa chiều ngang, nằm dưới cùng) để mọc thẳng từ nòng dâng lên
    DirectX::XMFLOAT2 bodyOrigin(bodyWidth / 2.0f, bodyHeight);
    DirectX::XMFLOAT2 position(m_x, m_y);

    // Vẽ phần body kéo dài
    spriteBatch->Draw(
        m_texture.Get(),
        position,
        &m_bodyRect,
        DirectX::Colors::White,
        rotation,
        bodyOrigin,
        scale,
        DirectX::SpriteEffects_None,
        0.0f
    );

    // Phần Impact (bùng nổ) ở mép màn hình
    float impactWidth = (float)(m_impactRect.right - m_impactRect.left);
    float impactHeight = (float)(m_impactRect.bottom - m_impactRect.top);
    
    // Origin của impact đặt ở giữa khối bùng nổ
    DirectX::XMFLOAT2 impactOrigin(impactWidth / 2.0f, impactHeight / 2.0f);
    DirectX::XMFLOAT2 impactPos(m_x, 0.0f); // Chạm viền

    spriteBatch->Draw(
        m_texture.Get(),
        impactPos,
        &m_impactRect,
        DirectX::Colors::White,
        rotation,
        impactOrigin,
        DirectX::XMFLOAT2(scaleX, scaleX), // Tỉ lệ bùng nổ tương đương độ dày thân
        DirectX::SpriteEffects_None,
        0.0f
    );
}
