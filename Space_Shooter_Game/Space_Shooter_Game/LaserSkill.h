#pragma once
#include "Skill.h"
#include "Laser.h"

class LaserSkill : public Skill {
private:
    float m_duration;
    int m_baseDamage;

public:
    LaserSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
