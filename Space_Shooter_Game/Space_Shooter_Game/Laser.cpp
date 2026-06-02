#include "Laser.h"
#include "AnimationManager.h"
#include "BaseEnemy.h"
#include "EntityManager.h"
#include "GameContext.h"
#include "Player.h"

namespace {
constexpr float LASER_PLAYER_Y_OFFSET = 10.0f;
}

Laser::Laser(Graphics& gfx, float duration, int damage, float sizeMultiplier)
    : GameObject(0.0f, 0.0f, 0.0f, 0.0f)
    , m_remainingDuration(duration)
    , m_damage(damage)
    , m_sizeMultiplier(sizeMultiplier) {
    (void)gfx;
    AnimationManager::GetInstance().PlayAnimation("laser_body", m_bodyAnim);

    float bodyDisplayWidth = 0.0f;
    float bodyDisplayHeight = 0.0f;
    AnimationManager::GetInstance().GetDisplaySize("laser_body", bodyDisplayWidth, bodyDisplayHeight);

    const AnimClip* bodyClip = m_bodyAnim.GetClip(
        AnimationManager::GetInstance().GetClipName("laser_body"));
    const float frameW = bodyClip ? static_cast<float>(bodyClip->frameWidth) : 16.0f;
    const float frameH = bodyClip ? static_cast<float>(bodyClip->frameHeight) : 16.0f;

    // Chiều rộng laser: displayWidth trong skills.json × sizeMultiplier (upgrade skill size).
    m_bodyDrawWidth = (bodyDisplayWidth > 0.0f ? bodyDisplayWidth : frameW) * m_sizeMultiplier;

    m_bodyUniformScale = m_bodyDrawWidth / frameW;
    m_bodySegmentHeight = (bodyDisplayHeight > 0.0f)
        ? bodyDisplayHeight * m_sizeMultiplier
        : frameH * m_bodyUniformScale;

    m_bodyFrameCount = bodyClip ? bodyClip->frameCount : 1;

    SetColliderName("laser_hitbox");
}

void Laser::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    m_remainingDuration -= dt;
    if (m_remainingDuration <= 0.0f) {
        Destroy();
        return;
    }

    m_x = ctx.player.GetCenterX();
    m_y = ctx.player.GetY() - LASER_PLAYER_Y_OFFSET;

    m_bodyAnim.Update(dt);

    const float beamHeight = m_y > 0.0f ? m_y : 0.0f;
    const Collider laserCollider = Collider::Rectangle(
        GetColliderName(),
        m_x - m_bodyDrawWidth * 0.5f,
        0.0f,
        m_bodyDrawWidth,
        beamHeight);

    auto& allEntities = ctx.entityManager.GetEntities();
    for (auto& entity : allEntities) {
        if (!entity->IsActive()) continue;

        BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(entity.get());
        if (enemy && enemy->CheckCollision(laserCollider)) {
            enemy->TakeDamage(m_damage * dt * 10.0f);
        }
    }
}

void Laser::Render(Graphics& gfx) {
    if (!m_isActive) {
        return;
    }

    const float beamBottom = m_y > 0.0f ? m_y : 0.0f;
    if (beamBottom <= 0.0f) {
        return;
    }

    const float drawX = m_x - (m_bodyDrawWidth * 0.5f);
    const float segmentH = m_bodySegmentHeight > 0.0f ? m_bodySegmentHeight : 16.0f;
    const int frameCount = m_bodyFrameCount > 0 ? m_bodyFrameCount : 1;
    const int currentFrame = m_bodyAnim.GetCurrentFrameIndex();

    // Ghép các mảnh từ player lên đỉnh; mỗi mảnh dùng frame animation lệch pha để tạo hiệu ứng chạy dọc.
    int segmentIndex = 0;
    float segmentY = beamBottom - segmentH;
    if (segmentY < 0.0f) {
        segmentY = 0.0f;
    }
    while (segmentY < beamBottom) {
        const int frameIndex = (currentFrame + segmentIndex) % frameCount;
        m_bodyAnim.RenderFrameAtIndex(gfx, frameIndex, drawX, segmentY, m_bodyUniformScale);
        if (segmentY <= 0.0f) {
            break;
        }
        segmentY -= segmentH;
        ++segmentIndex;
    }
}
