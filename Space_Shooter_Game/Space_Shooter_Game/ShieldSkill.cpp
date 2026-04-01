#include "ShieldSkill.h"
#include "EntityManager.h"
#include "GameContext.h"
#include "Player.h"
#include "Shield.h"
#include <memory>

namespace {
constexpr char SHIELD_KEY = '3';
constexpr float SHIELD_COOLDOWN = 12.0f;
constexpr int SHIELD_MAX_LEVEL = 5;
constexpr float SHIELD_LEVEL_DURATION_BONUS = 0.75f;
}

ShieldSkill::ShieldSkill()
    : Skill("Shield",
            "Creates a shield around your ship and blocks incoming damage.",
            SkillType::ACTIVE,
            SHIELD_COOLDOWN,
            SHIELD_MAX_LEVEL)
    , m_baseDuration(3.0f) {
}

bool ShieldSkill::CanActivate(GameContext& ctx) {
    return ctx.input.IsKeyPressed(SHIELD_KEY) && !ctx.player.IsShielded();
}

void ShieldSkill::Activate(GameContext& ctx) {
    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();
    auto shield = std::make_unique<Shield>(ctx.gfx, m_baseDuration, sizeMultiplier);
    ctx.entityManager.AddEntity(std::move(shield));
}

void ShieldSkill::OnLevelUp() {
    m_baseDuration += SHIELD_LEVEL_DURATION_BONUS;
}
