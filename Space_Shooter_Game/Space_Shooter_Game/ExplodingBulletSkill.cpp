#include "ExplodingBulletSkill.h"
#include "ExplodingBullet.h"
#include "GameContext.h"
#include "EntityManager.h"
#include "Player.h"
#include "SkillConfig.h"
#include <memory>

ExplodingBulletSkill::ExplodingBulletSkill()
    : Skill(GetSkillConfig("exploding_bullet").name,
            GetSkillConfig("exploding_bullet").description,
            SkillType::ACTIVE,
            GetSkillConfig("exploding_bullet").cooldown,
            GetSkillConfig("exploding_bullet").maxLevel)
    , m_baseSpeed(GetSkillConfig("exploding_bullet").baseSpeed)
    , m_baseDamage(GetSkillConfig("exploding_bullet").baseDamage)
    , m_activationKey(GetSkillConfig("exploding_bullet").activationKey)
    , m_speedLevelBonus(GetSkillConfig("exploding_bullet").speedLevelBonus)
    , m_damageLevelBonus(GetSkillConfig("exploding_bullet").damageLevelBonus)
    , m_levelUpDamageBonus(GetSkillConfig("exploding_bullet").levelUpDamageBonus)
    , m_playerDamageScale(GetSkillConfig("exploding_bullet").playerDamageScale) {
}

bool ExplodingBulletSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed(m_activationKey);
}

void ExplodingBulletSkill::Activate(GameContext& ctx) {
    float startX = ctx.player.GetX() + (ctx.player.GetWidth() * 0.5f);
    float startY = ctx.player.GetY();

    float targetX = static_cast<float>(ctx.input.GetMouseX());
    float targetY = static_cast<float>(ctx.input.GetMouseY());

    float speed = m_baseSpeed + ((m_level - 1) * m_speedLevelBonus);
    int damage = m_baseDamage
        + ((m_level - 1) * m_damageLevelBonus)
        + static_cast<int>(ctx.player.GetAttackDamage() * m_playerDamageScale);

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
    m_baseDamage += m_levelUpDamageBonus;
    m_baseSpeed += m_speedLevelBonus;
}
