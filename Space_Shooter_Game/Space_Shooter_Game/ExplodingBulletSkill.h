#pragma once
#include "Skill.h"

class ExplodingBulletSkill : public Skill {
private:
    float m_baseSpeed;
    int m_baseDamage;
    char m_activationKey;
    float m_speedLevelBonus;
    int m_damageLevelBonus;
    int m_levelUpDamageBonus;
    float m_playerDamageScale;

public:
    ExplodingBulletSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
