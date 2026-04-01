#include "ExplodingBullet.h"
#include "AssetManager.h"
#include "GameContext.h"
#include <cmath>

namespace {
constexpr wchar_t EXPLODING_BULLET_TEXTURE_PATH[] = L"Assets/ExplodingBullet_1.png";

constexpr int BULLET_FRAME_X = 10;
constexpr int BULLET_FRAME_Y = 10;
constexpr int BULLET_FRAME_W = 20;
constexpr int BULLET_FRAME_H = 20;
constexpr int BULLET_FRAME_COUNT = 2;
constexpr int BULLET_FRAME_SPACING_X = 20;
constexpr float BULLET_DRAW_W = 50.0f;
constexpr float BULLET_DRAW_H = 50.0f;
constexpr float BULLET_FRAME_DURATION = 0.1f;

constexpr int EXPLOSION_FRAME_X = 80;
constexpr int EXPLOSION_FRAME_Y = 0;
constexpr int EXPLOSION_FRAME_W = 58;
constexpr int EXPLOSION_FRAME_H = 58;
constexpr int EXPLOSION_FRAME_COUNT = 5;
constexpr float EXPLOSION_DRAW_W = 700.0f;
constexpr float EXPLOSION_DRAW_H = 700.0f;
constexpr float EXPLOSION_FRAME_DURATION = 0.1f;
}

ExplodingBullet::ExplodingBullet(Graphics& gfx,
                                 float startX,
                                 float startY,
                                 float targetX,
                                 float targetY,
                                 float speed,
                                 float sizeMultiplier,
                                 int damage)
    : GameObject(startX - (BULLET_DRAW_W * sizeMultiplier * 0.5f),
                 startY - (BULLET_DRAW_H * sizeMultiplier * 0.5f),
                 BULLET_DRAW_W * sizeMultiplier,
                 BULLET_DRAW_H * sizeMultiplier)
    , m_state(ExplodingBulletState::Flying)
    , m_speed(speed)
    , m_damage(damage)
    , m_directionX(0.0f)
    , m_directionY(-1.0f) {
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, EXPLODING_BULLET_TEXTURE_PATH));
    m_anim.AddClip(
        "Bullet",
        BULLET_FRAME_X,
        BULLET_FRAME_Y,
        BULLET_FRAME_W,
        BULLET_FRAME_H,
        BULLET_FRAME_COUNT,
        BULLET_FRAME_COUNT,
        BULLET_FRAME_DURATION,
        true,
        BULLET_FRAME_SPACING_X
    );
    m_anim.AddClip(
        "Explosion",
        EXPLOSION_FRAME_X,
        EXPLOSION_FRAME_Y,
        EXPLOSION_FRAME_W,
        EXPLOSION_FRAME_H,
        EXPLOSION_FRAME_COUNT,
        EXPLOSION_FRAME_COUNT,
        EXPLOSION_FRAME_DURATION,
        false
    );
    m_anim.Play("Bullet");

    float dx = targetX - (m_x + (m_width * 0.5f));
    float dy = targetY - (m_y + (m_height * 0.5f));
    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f) {
        m_directionX = dx / length;
        m_directionY = dy / length;
    }
}

void ExplodingBullet::Update(float dt, GameContext& ctx) {
    if (!m_isActive) {
        return;
    }

    if (m_state == ExplodingBulletState::Flying) {
        m_x += m_directionX * m_speed * dt;
        m_y += m_directionY * m_speed * dt;
        m_anim.Update(dt);

        if (IsOutOfBounds(ctx.screenWidth, ctx.screenHeight)) {
            if (m_x < 0.0f) m_x = 0.0f;
            if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
            if (m_y < 0.0f) m_y = 0.0f;
            if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;
            StartExplosion();
        }
        return;
    }

    m_anim.Update(dt);
    if (m_anim.IsFinished()) {
        Destroy();
    }
}

void ExplodingBullet::Render(Graphics& gfx) {
    if (!m_isActive) {
        return;
    }

    m_anim.Render(gfx, m_x, m_y, m_width, m_height);
}

void ExplodingBullet::StartExplosion() {
    float centerX = m_x + (m_width * 0.5f);
    float centerY = m_y + (m_height * 0.5f);

    m_state = ExplodingBulletState::Exploding;
    m_vx = 0.0f;
    m_vy = 0.0f;
    m_width = EXPLOSION_DRAW_W;
    m_height = EXPLOSION_DRAW_H;
    m_x = centerX - (m_width * 0.5f);
    m_y = centerY - (m_height * 0.5f);
    m_anim.Play("Explosion");
}

bool ExplodingBullet::IsOutOfBounds(float screenW, float screenH) const {
    return (m_x < 0.0f || (m_x + m_width) > screenW || m_y < 0.0f || (m_y + m_height) > screenH);
}
