#include "Shield.h"
#include "AssetManager.h"
#include "GameContext.h"
#include "Player.h"

namespace {
constexpr wchar_t SHIELD_TEXTURE_PATH[] = L"Assets/Shield_1.png";
constexpr int SHIELD_FRAME_W = 64;
constexpr int SHIELD_FRAME_H = 64;
constexpr int SHIELD_FRAME_COUNT = 12;
constexpr float SHIELD_FRAME_DURATION = 0.06f;
constexpr float SHIELD_DRAW_SIZE = 140.0f;
}

Shield::Shield(Graphics& gfx, float duration, float sizeMultiplier)
    : GameObject(0.0f, 0.0f, SHIELD_DRAW_SIZE * sizeMultiplier, SHIELD_DRAW_SIZE * sizeMultiplier)
    , m_remainingDuration(duration) {
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, SHIELD_TEXTURE_PATH));
    m_anim.AddClip(
        "Active",
        0,
        0,
        SHIELD_FRAME_W,
        SHIELD_FRAME_H,
        SHIELD_FRAME_COUNT,
        SHIELD_FRAME_COUNT,
        SHIELD_FRAME_DURATION,
        true
    );
    m_anim.Play("Active");
}

void Shield::Update(float dt, GameContext& ctx) {
    if (!m_isActive) {
        return;
    }

    m_remainingDuration -= dt;
    if (m_remainingDuration <= 0.0f) {
        ctx.player.SetShielded(false);
        Destroy();
        return;
    }

    m_x = ctx.player.GetX() + (ctx.player.GetWidth() * 0.5f) - (m_width * 0.5f);
    m_y = ctx.player.GetY() + (ctx.player.GetHeight() * 0.5f) - (m_height * 0.5f);
    ctx.player.SetShielded(true);
    m_anim.Update(dt);
}

void Shield::Render(Graphics& gfx) {
    if (!m_isActive) {
        return;
    }

    m_anim.Render(gfx, m_x, m_y, m_width, m_height);
}
