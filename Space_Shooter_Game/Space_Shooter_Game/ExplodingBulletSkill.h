#pragma once
#include "Skill.h"

class ExplodingBulletSkill : public Skill {
private:
    float m_baseSpeed;
    int m_baseDamage;

public:
    ExplodingBulletSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
