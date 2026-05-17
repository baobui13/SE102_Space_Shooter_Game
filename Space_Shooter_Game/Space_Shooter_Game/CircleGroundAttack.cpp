#include "CircleGroundAttack.h"
#include "AssetManager.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

namespace {
constexpr wchar_t EXPLOSION_TEXTURE_PATH[] =
    L"Assets/Bullets/Green Effect Bullet Impact Explosion 32x32.png";

constexpr int EXPLOSION_FRAME_X = 512;
constexpr int EXPLOSION_FRAME_Y = 384;
constexpr int EXPLOSION_FRAME_W = 32;
constexpr int EXPLOSION_FRAME_H = 32;
constexpr int EXPLOSION_FRAME_COUNT = 4;
constexpr int EXPLOSION_COLUMNS = 4;
constexpr float EXPLOSION_FRAME_DURATION = 0.08f;
constexpr float EXPLOSION_DISPLAY_SCALE = 4.0f;
}

CircleGroundAttack::CircleGroundAttack(Graphics& gfx,
                                       float centerX,
                                       float centerY,
                                       float radius,
                                       int damage,
                                       float warningDuration)
    : GameObject(
        centerX - radius,
        centerY - radius,
        radius * 2.0f,
        radius * 2.0f),
    m_centerX(centerX),
    m_centerY(centerY),
    m_radius(radius),
    m_damage(damage),
    m_warningDuration(warningDuration)
{
    const float drawSize = EXPLOSION_FRAME_W * EXPLOSION_DISPLAY_SCALE;
    m_width = drawSize;
    m_height = drawSize;
    m_x = centerX - drawSize * 0.5f;
    m_y = centerY - drawSize * 0.5f;

    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, EXPLOSION_TEXTURE_PATH));
    m_anim.AddClip(
        "explosion",
        EXPLOSION_FRAME_X,
        EXPLOSION_FRAME_Y,
        EXPLOSION_FRAME_W,
        EXPLOSION_FRAME_H,
        EXPLOSION_FRAME_COUNT,
        EXPLOSION_COLUMNS,
        EXPLOSION_FRAME_DURATION,
        false,
        0,
        0
    );
}

void CircleGroundAttack::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    m_elapsed += dt;

    if (m_state == CircleGroundAttackState::Warning) {
        if (m_elapsed >= m_warningDuration) {
            m_state = CircleGroundAttackState::Exploding;
            m_x = m_centerX - m_width * 0.5f;
            m_y = m_centerY - m_height * 0.5f;
            m_anim.Play("explosion");
            ApplyDamage(ctx);
        }
        return;
    }

    m_anim.Update(dt);
    if (m_anim.IsFinished()) {
        Destroy();
    }
}

void CircleGroundAttack::Render(Graphics& gfx) {
    if (!m_isActive || m_state != CircleGroundAttackState::Exploding) {
        return;
    }

    m_anim.Render(gfx, m_x, m_y, m_width, m_height);
}

bool CircleGroundAttack::IsPlayerInsideCircle(GameContext& ctx) const {
    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() * 0.5f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() * 0.5f;
    float dx = playerCX - m_centerX;
    float dy = playerCY - m_centerY;
    return (dx * dx + dy * dy) <= (m_radius * m_radius);
}

void CircleGroundAttack::ApplyDamage(GameContext& ctx) {
    if (m_damageApplied) return;

    m_damageApplied = true;
    if (IsPlayerInsideCircle(ctx)) {
        ctx.player.TakeDamage(m_damage);
    }
}
