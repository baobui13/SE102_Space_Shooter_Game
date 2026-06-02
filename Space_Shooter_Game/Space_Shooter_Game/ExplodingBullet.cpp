#include "ExplodingBullet.h"
#include "AnimationManager.h"
#include "GameContext.h"
#include <cmath>
#include "EntityManager.h"
#include "BaseEnemy.h"

ExplodingBullet::ExplodingBullet(Graphics& gfx,
                                 float startX,
                                 float startY,
                                 float targetX,
                                 float targetY,
                                 float speed,
                                 float sizeMultiplier,
                                 int damage)
    : GameObject(0.0f, 0.0f, 0.0f, 0.0f)
    , m_state(ExplodingBulletState::Flying)
    , m_speed(speed)
    , m_damage(damage)
    , m_sizeMultiplier(sizeMultiplier)
    , m_directionX(0.0f)
    , m_directionY(-1.0f)
    , m_flyDrawWidth(50.0f)
    , m_flyDrawHeight(50.0f)
    , m_explosionDrawWidth(700.0f)
    , m_explosionDrawHeight(700.0f) {
    (void)gfx;
    SetColliderName("exploding_bullet");

    AnimationManager::GetInstance().GetDisplaySize("exploding_bullet_fly", m_flyDrawWidth, m_flyDrawHeight);
    AnimationManager::GetInstance().GetDisplaySize("exploding_bullet_explosion", m_explosionDrawWidth, m_explosionDrawHeight);
    AnimationManager::GetInstance().PlayAnimation("exploding_bullet_fly", m_anim);

    m_width = m_flyDrawWidth * m_sizeMultiplier;
    m_height = m_flyDrawHeight * m_sizeMultiplier;
    m_x = startX - (m_width * 0.5f);
    m_y = startY - (m_height * 0.5f);

    float dx = targetX - (m_x + (m_width * 0.5f));
    float dy = targetY - (m_y + (m_height * 0.5f));
    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f) {
        m_directionX = dx / length;
        m_directionY = dy / length;
    }
}

void ExplodingBullet::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    if (m_state == ExplodingBulletState::Flying) {
        m_x += m_directionX * m_speed * dt;
        m_y += m_directionY * m_speed * dt;
        m_anim.Update(dt);

        bool triggeredExplosion = false;

        // 1. Kiểm tra nếu bay ra khỏi màn hình
        if (IsOutOfBounds(ctx.screenWidth, ctx.screenHeight)) {
            triggeredExplosion = true;
            if (m_x < 0.0f) m_x = 0.0f;
            if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
            if (m_y < 0.0f) m_y = 0.0f;
            if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;
        }
        else {
            // 2. Kiểm tra nếu va chạm với quái vật trên đường bay
            auto& allEntities = ctx.entityManager.GetEntities();
            for (auto& entity : allEntities) {
                if (!entity->IsActive()) continue;
                BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(entity.get());
                if (enemy && this->CheckCollision(*enemy)) {
                    triggeredExplosion = true;
                    break; // Chạm 1 con là đủ để nổ
                }
            }
        }

        // 3. Nếu kích hoạt nổ, gây sát thương diện rộng 1 LẦN duy nhất
        if (triggeredExplosion) {
            StartExplosion(); // Hàm này sẽ làm m_width, m_height phình to ra thành vùng nổ

            // Quét lại toàn bộ quái vật xem con nào nằm trong vùng nổ
            auto& allEntities = ctx.entityManager.GetEntities();
            for (auto& entity : allEntities) {
                if (!entity->IsActive()) continue;
                BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(entity.get());
                if (enemy && this->CheckCollision(*enemy)) {
                    enemy->TakeDamage(m_damage); // Gây sát thương nổ
                }
            }
        }
        return;
    }

    // Nếu đang ở trạng thái Exploding (chỉ chạy Animation nổ)
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
    SetColliderName("exploding_explosion");
    m_vx = 0.0f;
    m_vy = 0.0f;
    m_width = m_explosionDrawWidth * m_sizeMultiplier;
    m_height = m_explosionDrawHeight * m_sizeMultiplier;
    m_x = centerX - (m_width * 0.5f);
    m_y = centerY - (m_height * 0.5f);
    AnimationManager::GetInstance().PlayAnimation("exploding_bullet_explosion", m_anim);
}

bool ExplodingBullet::IsOutOfBounds(float screenW, float screenH) const {
    return (m_x < 0.0f || (m_x + m_width) > screenW || m_y < 0.0f || (m_y + m_height) > screenH);
}
