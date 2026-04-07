#include "ExplodingBulletSkill.h"
#include "ExplodingBullet.h"
#include "GameContext.h"
#include "EntityManager.h"
#include "Player.h"
#include <memory>

namespace {
constexpr char EXPLODING_BULLET_KEY = '2';
constexpr float EXPLODING_BULLET_COOLDOWN = 3.0f;
constexpr int EXPLODING_BULLET_MAX_LEVEL = 5;
constexpr float EXPLODING_BULLET_LEVEL_SPEED_BONUS = 20.0f;
constexpr int EXPLODING_BULLET_LEVEL_DAMAGE_BONUS = 20;
constexpr int EXPLODING_BULLET_LEVEL_UP_DAMAGE_BONUS = 25;
}

ExplodingBulletSkill::ExplodingBulletSkill()
    : Skill("Explosive Bullet",
            "Fires a slow bullet toward your cursor. Explodes on impact!",
            SkillType::ACTIVE,
            EXPLODING_BULLET_COOLDOWN,
            EXPLODING_BULLET_MAX_LEVEL)
    , m_baseSpeed(160.0f)
    , m_baseDamage(50) {
}

bool ExplodingBulletSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed(EXPLODING_BULLET_KEY);
}

void ExplodingBulletSkill::Activate(GameContext& ctx) {
    float startX = ctx.player.GetX() + (ctx.player.GetWidth() * 0.5f);
    float startY = ctx.player.GetY();

    float targetX = static_cast<float>(ctx.input.GetMouseX());
    float targetY = static_cast<float>(ctx.input.GetMouseY());

    float speed = m_baseSpeed + ((m_level - 1) * EXPLODING_BULLET_LEVEL_SPEED_BONUS);
    int damage = m_baseDamage
        + ((m_level - 1) * EXPLODING_BULLET_LEVEL_DAMAGE_BONUS)
        + static_cast<int>(ctx.player.GetAttackDamage() * 1.2f);

    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();

    auto bullet = std::make_unique<ExplodingBullet>(
        ctx.gfx,
        startX,
        startY,
        targetX,
        targetY,
        speed,
        sizeMultiplier,
        damage
    );
    ctx.entityManager.AddEntity(std::move(bullet));
}

void ExplodingBulletSkill::OnLevelUp() {
    m_baseDamage += EXPLODING_BULLET_LEVEL_UP_DAMAGE_BONUS;
    m_baseSpeed += EXPLODING_BULLET_LEVEL_SPEED_BONUS;
}
