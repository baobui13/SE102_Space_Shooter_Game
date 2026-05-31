#pragma once

struct PlayerConfigValues {
    float size = 64.0f;
    float speed = 300.0f;
    int hp = 100;
    int attackDamage = 10;
    float attackSpeed = 3.0f;
    float attackRange = 250.0f;
    int startLevel = 1;
    int startExp = 0;
    int expToNextLevel = 100;
    float expGrowthMultiplier = 1.3f;
    float magnetRange = 100.0f;
    float cooldownMultiplier = 1.0f;
    float skillSizeMultiplier = 1.0f;
    float dashSpeed = 900.0f;
    float dashDuration = 0.18f;
    int dashCharges = 2;
    float dashRechargeTime = 1.5f;
    float bulletOffset = 8.0f;
    float bulletSpeed = 500.0f;
    float shootRecoilOffset = 10.0f;
    float shootRecoilRecoverSpeed = 80.0f;
    float invulDuration = 1.0f;
};

const PlayerConfigValues& GetPlayerConfig();
