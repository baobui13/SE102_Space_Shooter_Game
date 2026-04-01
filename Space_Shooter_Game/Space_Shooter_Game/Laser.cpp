#include "Laser.h"
#include "AssetManager.h"
#include "GameContext.h"
#include "Player.h"
#include <DirectXColors.h>
#include <DirectXMath.h>

namespace {
constexpr wchar_t LASER_TEXTURE_PATH[] = L"Assets/lazer_1.png";

constexpr LONG LASER_BODY_LEFT = 37;
constexpr LONG LASER_BODY_TOP = 2;
constexpr LONG LASER_BODY_RIGHT = 48;
constexpr LONG LASER_BODY_BOTTOM = 17;

constexpr LONG LASER_IMPACT_LEFT = 2;
constexpr LONG LASER_IMPACT_TOP = 1;
constexpr LONG LASER_IMPACT_RIGHT = 33;
constexpr LONG LASER_IMPACT_BOTTOM = 18;

constexpr float LASER_BASE_THICKNESS = 5.0f;
constexpr float LASER_PLAYER_Y_OFFSET = 10.0f;
}

Laser::Laser(Graphics& gfx, float duration, int damage, float sizeMultiplier)
    : GameObject(0.0f, 0.0f, 0.0f, 0.0f)
    , m_remainingDuration(duration)
    , m_damage(damage)
    , m_sizeMultiplier(sizeMultiplier)
    , m_texture(AssetManager::GetInstance().GetTexture(gfx, LASER_TEXTURE_PATH))
    , m_bodyRect{ LASER_BODY_LEFT, LASER_BODY_TOP, LASER_BODY_RIGHT, LASER_BODY_BOTTOM }
    , m_impactRect{ LASER_IMPACT_LEFT, LASER_IMPACT_TOP, LASER_IMPACT_RIGHT, LASER_IMPACT_BOTTOM } {
}

void Laser::Update(float dt, GameContext& ctx) {
    if (!m_isActive) {
        return;
    }

    m_remainingDuration -= dt;
    if (m_remainingDuration <= 0.0f) {
        Destroy();
        return;
    }

    m_x = ctx.player.GetX() + (ctx.player.GetWidth() * 0.5f);
    m_y = ctx.player.GetY() + LASER_PLAYER_Y_OFFSET;
}

void Laser::Render(Graphics& gfx) {
    if (!m_isActive || !m_texture) {
        return;
    }

    auto spriteBatch = gfx.GetSpriteBatch();

    float distance = m_y;
    float bodyWidth = static_cast<float>(m_bodyRect.right - m_bodyRect.left);
    float bodyHeight = static_cast<float>(m_bodyRect.bottom - m_bodyRect.top);
    float thickness = LASER_BASE_THICKNESS * m_sizeMultiplier;
    float stretchY = distance / bodyHeight;

    DirectX::XMFLOAT2 bodyScale(thickness, stretchY);
    DirectX::XMFLOAT2 bodyOrigin(bodyWidth * 0.5f, bodyHeight);
    DirectX::XMFLOAT2 bodyPosition(m_x, m_y);

    spriteBatch->Draw(
        m_texture.Get(),
        bodyPosition,
        &m_bodyRect,
        DirectX::Colors::White,
        0.0f,
        bodyOrigin,
        bodyScale,
        DirectX::SpriteEffects_None,
        0.0f
    );

    float impactWidth = static_cast<float>(m_impactRect.right - m_impactRect.left);
    float impactHeight = static_cast<float>(m_impactRect.bottom - m_impactRect.top);
    DirectX::XMFLOAT2 impactOrigin(impactWidth * 0.5f, impactHeight * 0.5f);
    DirectX::XMFLOAT2 impactPosition(m_x, 0.0f);
    DirectX::XMFLOAT2 impactScale(thickness, thickness);

    spriteBatch->Draw(
        m_texture.Get(),
        impactPosition,
        &m_impactRect,
        DirectX::Colors::White,
        0.0f,
        impactOrigin,
        impactScale,
        DirectX::SpriteEffects_None,
        0.0f
    );
}
