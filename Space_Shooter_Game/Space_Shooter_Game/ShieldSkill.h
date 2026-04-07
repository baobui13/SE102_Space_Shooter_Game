#pragma once
#include "Skill.h"

class ShieldSkill : public Skill {
private:
    float m_baseDuration;

public:
    ShieldSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
