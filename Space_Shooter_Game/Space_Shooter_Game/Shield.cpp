#include "Shield.h"
#include "AnimationManager.h"
#include "GameContext.h"
#include "Player.h"

Shield::Shield(Graphics& gfx, float duration, float sizeMultiplier)
    : GameObject(0.0f, 0.0f, 140.0f * sizeMultiplier, 140.0f * sizeMultiplier)
    , m_remainingDuration(duration) {
    (void)gfx;
    float baseW = 140.0f;
    float baseH = 140.0f;
    AnimationManager::GetInstance().GetDisplaySize("shield_active", baseW, baseH);
    m_width = baseW * sizeMultiplier;
    m_height = baseH * sizeMultiplier;
    AnimationManager::GetInstance().PlayAnimation("shield_active", m_anim);
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
