#include "ShieldSkill.h"
#include "EntityManager.h"
#include "GameContext.h"
#include "Player.h"
#include "Shield.h"
#include "AudioManager.h"
#include "SkillConfig.h"
#include <memory>

ShieldSkill::ShieldSkill()
    : Skill(GetSkillConfig("shield").name,
            GetSkillConfig("shield").description,
            SkillType::ACTIVE,
            GetSkillConfig("shield").cooldown,
            GetSkillConfig("shield").maxLevel)
    , m_baseDuration(GetSkillConfig("shield").baseDuration)
    , m_activationKey(GetSkillConfig("shield").activationKey)
    , m_durationLevelBonus(GetSkillConfig("shield").durationLevelBonus) {
}

bool ShieldSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed(m_activationKey) && !ctx.player.IsShielded();
}

void ShieldSkill::Activate(GameContext& ctx) {
    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();
    auto shield = std::make_unique<Shield>(ctx.gfx, m_baseDuration, sizeMultiplier);
    ctx.entityManager.AddEntity(std::move(shield));
    AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerShield);
}

void ShieldSkill::OnLevelUp() {
    m_baseDuration += m_durationLevelBonus;
}
