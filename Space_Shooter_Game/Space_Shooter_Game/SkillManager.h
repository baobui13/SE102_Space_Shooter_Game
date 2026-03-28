#pragma once
#include <vector>
#include <memory>
#include "Skill.h"

struct GameContext;

class SkillManager {
private:
    std::vector<std::unique_ptr<Skill>> m_skills;

public:
    SkillManager();
    ~SkillManager() = default;

    void AddSkill(std::unique_ptr<Skill> skill);

    void Update(float dt, GameContext& ctx);
};