#include "LaserSkill.h"
#include "GameContext.h"
#include "EntityManager.h"
#include "Laser.h"
#include "Player.h"

namespace {
constexpr char LASER_KEY = '1';
constexpr float LASER_COOLDOWN = 10.0f;
constexpr int LASER_MAX_LEVEL = 5;
constexpr int LASER_LEVEL_DAMAGE_BONUS = 20;
}

LaserSkill::LaserSkill()
    : Skill("Laser Beam",
            "Fires a massive laser beam from your ship.",
            SkillType::ACTIVE,
            LASER_COOLDOWN,
            LASER_MAX_LEVEL)
    , m_baseDuration(5.0f)
    , m_baseDamage(50) {
}

bool LaserSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed(LASER_KEY);
}

void LaserSkill::Activate(GameContext& ctx) {
    int damage = m_baseDamage
        + ((m_level - 1) * LASER_LEVEL_DAMAGE_BONUS)
        + static_cast<int>(ctx.player.GetAttackDamage() * 1.5f);

    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();

    auto laser = std::make_unique<Laser>(ctx.gfx, m_baseDuration, damage, sizeMultiplier);
    ctx.entityManager.AddEntity(std::move(laser));
}

void LaserSkill::OnLevelUp() {
    m_baseDamage += LASER_LEVEL_DAMAGE_BONUS;
}
