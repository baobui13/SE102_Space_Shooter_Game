#include "ExplodingBulletSkill.h"
#include "ExplodingBullet.h"
#include "GameContext.h"
#include "EntityManager.h"
#include "Player.h"
#include <memory>

ExplodingBulletSkill::ExplodingBulletSkill()
    : Skill("Explosive Bullet",
            "Fires a slow bullet toward your cursor. Explodes on impact!",
            SkillType::ACTIVE,
            3.0f,   // Cooldown (giây)
            5)      // Max level
    , m_speed(160.0f)
    , m_damage(50)
{
}

// Kích hoạt bằng phím '2'
bool ExplodingBulletSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed('2');
}

void ExplodingBulletSkill::Activate(GameContext& ctx) {
    // Xuất phát từ tâm player
    float startX = ctx.player.GetX() + (ctx.player.GetWidth() * 0.5f);
    float startY = ctx.player.GetY();

    // Tọa độ chuột (đã được InputManager chuyển sang virtual space)
    float targetX = (float)ctx.input.GetMouseX();
    float targetY = (float)ctx.input.GetMouseY();

    // Tốc độ scale theo level: +20px/s mỗi cấp
    float speed = m_speed + (m_level - 1) * 20.0f;

    // Sát thương: Scale theo Level + Scale theo Attack Damage của Player
    int damage = m_damage + (m_level - 1) * 20 + (int)(ctx.player.GetAttackDamage() * 1.2f);

    // Kích thước: Từ Player
    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();

    auto bullet = std::make_unique<ExplodingBullet>(
        ctx.gfx, startX, startY, targetX, targetY, speed, sizeMultiplier, damage
    );
    ctx.entityManager.AddEntity(std::move(bullet));
}

void ExplodingBulletSkill::OnLevelUp() {
    m_damage += 25;     // Thêm 25 sát thương mỗi cấp
    m_speed  += 20.0f;  // Đạn nhanh hơn 20px/s mỗi cấp
}
