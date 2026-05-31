#pragma once
#include "Skill.h"

class ShieldSkill : public Skill {
private:
    float m_baseDuration;
    char m_activationKey;
    float m_durationLevelBonus;

public:
    ShieldSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
