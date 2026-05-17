#pragma once
#include "GameObject.h"

enum class CircleGroundAttackState {
    Warning,
    Exploding
};

// Vụ nổ vòng tròn tại vị trí cố định: chờ warningDuration rồi gây sát thương + animation
class CircleGroundAttack : public GameObject {
public:
    CircleGroundAttack(Graphics& gfx,
                       float centerX,
                       float centerY,
                       float radius,
                       int damage,
                       float warningDuration);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;

private:
    void ApplyDamage(GameContext& ctx);
    bool IsPlayerInsideCircle(GameContext& ctx) const;

    float m_centerX;
    float m_centerY;
    float m_radius;
    int m_damage;
    float m_warningDuration;
    float m_elapsed = 0.0f;
    CircleGroundAttackState m_state = CircleGroundAttackState::Warning;
    bool m_damageApplied = false;
};
