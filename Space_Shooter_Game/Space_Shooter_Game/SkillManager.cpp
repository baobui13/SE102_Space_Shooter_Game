#include "SkillManager.h"
#include "GameContext.h"
#include "Player.h"

SkillManager::SkillManager() {
}

void SkillManager::AddSkill(std::unique_ptr<Skill> skill) {
    m_skills.push_back(std::move(skill));
}

void SkillManager::Update(float dt, GameContext& ctx) {
    for (auto& skill : m_skills) {
        skill->Update(dt);
        if (skill->GetType() == SkillType::ACTIVE && skill->IsReady()) {
            skill->Activate(ctx);
            skill->ResetCooldown();
        }
    }
}