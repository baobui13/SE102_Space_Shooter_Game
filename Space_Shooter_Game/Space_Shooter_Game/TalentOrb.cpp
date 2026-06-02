#include "TalentOrb.h"
#include "AnimationManager.h"
#include "GameContext.h"
#include "OrbConfig.h"
#include "Player.h"

#include <cmath>

TalentOrb::TalentOrb(Graphics& gfx, float x, float y, int pointValue)
    : GameObject(x, y, OrbConfig::GetTalentOrb().width, OrbConfig::GetTalentOrb().height)
    , m_pointValue(pointValue)
    , m_magnetSpeed(OrbConfig::GetTalentOrb().magnetSpeed)
    , m_fallSpeed(OrbConfig::GetTalentOrb().fallSpeed)
    , m_collectColliderName(OrbConfig::GetTalentOrb().collectColliderName)
{
    (void)gfx;
    const OrbDefinition& config = OrbConfig::GetTalentOrb();
    SetColliderName(config.colliderName);
    m_vx = 0.0f;
    m_vy = m_fallSpeed;

    AnimationManager::GetInstance().PlayAnimation(config.animationId, m_anim);
}

void TalentOrb::Update(float dt, ::GameContext& ctx) {
    const float playerCenterX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    const float playerCenterY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
    const float orbCenterX = m_x + m_width / 2.0f;
    const float orbCenterY = m_y + m_height / 2.0f;

    const float dx = playerCenterX - orbCenterX;
    const float dy = playerCenterY - orbCenterY;
    const float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < ctx.player.GetMagnetRange() && distance > 0.0f) {
        m_vx = (dx / distance) * m_magnetSpeed;
        m_vy = (dy / distance) * m_magnetSpeed;
    }
    else {
        m_vx = 0.0f;
        m_vy = m_fallSpeed;
    }

    GameObject::Update(dt, ctx);

    const Collider collectCollider = ColliderRegistry::GetInstance().CreateColliderAt(
        m_collectColliderName,
        GetCenterX(),
        GetCenterY());

    if (collectCollider.Intersects(ctx.player.GetCollider())) {
        ctx.player.AddUpgradePoints(m_pointValue);
        Destroy();
    }
    else if (m_y > ctx.screenHeight + 100.0f) {
        Destroy();
    }
}
