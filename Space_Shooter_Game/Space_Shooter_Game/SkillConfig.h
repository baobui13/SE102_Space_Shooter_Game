#pragma once

#include <string>

struct SkillConfigValues {
    std::string name;
    std::string description;
    char activationKey = '1';
    float cooldown = 1.0f;
    int maxLevel = 5;
    float baseDuration = 0.0f;
    float durationLevelBonus = 0.0f;
    float baseSpeed = 0.0f;
    float speedLevelBonus = 0.0f;
    int baseDamage = 0;
    int damageLevelBonus = 0;
    int levelUpDamageBonus = 0;
    float playerDamageScale = 0.0f;
};

SkillConfigValues GetSkillConfig(const std::string& id);
