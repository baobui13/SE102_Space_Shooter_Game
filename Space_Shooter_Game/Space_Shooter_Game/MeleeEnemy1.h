#pragma once
#include "BaseEnemy.h"

class MeleeEnemy1 : public BaseEnemy {
public:
    MeleeEnemy1(float x, float y, float width, float height,
                float health, float moveSpeed, float attackPower, float attackRange);

    void Update(float dt, GameContext& ctx) override;
    void Attack(GameObject* target) override;
    bool IsPlayerInRange(GameObject* player) const;
};
