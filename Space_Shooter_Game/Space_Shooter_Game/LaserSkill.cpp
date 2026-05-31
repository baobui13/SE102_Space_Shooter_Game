#include "LaserSkill.h"
#include "GameContext.h"
#include "EntityManager.h"
#include "Laser.h"
#include "Player.h"
#include "AudioManager.h"
#include "SkillConfig.h"

LaserSkill::LaserSkill()
    : Skill(GetSkillConfig("laser").name,
            GetSkillConfig("laser").description,
            SkillType::ACTIVE,
            GetSkillConfig("laser").cooldown,
            GetSkillConfig("laser").maxLevel)
    , m_baseDuration(GetSkillConfig("laser").baseDuration)
    , m_baseDamage(GetSkillConfig("laser").baseDamage)
    , m_activationKey(GetSkillConfig("laser").activationKey)
    , m_damageLevelBonus(GetSkillConfig("laser").damageLevelBonus)
    , m_playerDamageScale(GetSkillConfig("laser").playerDamageScale) {
}

bool LaserSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed(m_activationKey);
}

void LaserSkill::Activate(GameContext& ctx) {
    int damage = m_baseDamage
        + ((m_level - 1) * m_damageLevelBonus)
        + static_cast<int>(ctx.player.GetAttackDamage() * m_playerDamageScale);

    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();

    auto laser = std::make_unique<Laser>(ctx.gfx, m_baseDuration, damage, sizeMultiplier);
    ctx.entityManager.AddEntity(std::move(laser));

    // Phát âm thanh laser
    AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerLaser);
}

void LaserSkill::OnLevelUp() {
    m_baseDamage += GetSkillConfig("laser").levelUpDamageBonus;
}
