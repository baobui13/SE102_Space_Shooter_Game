#pragma once
#include "Skill.h"

class LaserSkill : public Skill {
private:
    float m_baseDuration;
    int m_baseDamage;

public:
    LaserSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
