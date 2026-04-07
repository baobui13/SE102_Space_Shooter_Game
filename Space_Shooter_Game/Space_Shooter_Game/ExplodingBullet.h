#pragma once
#include "GameObject.h"

enum class ExplodingBulletState {
    Flying,
    Exploding,
};

class ExplodingBullet : public GameObject {
private:
    ExplodingBulletState m_state;
    float m_speed;
    int m_damage;
    float m_directionX;
    float m_directionY;

public:
    ExplodingBullet(Graphics& gfx,
                    float startX,
                    float startY,
                    float targetX,
                    float targetY,
                    float speed = 160.0f,
                    float sizeMultiplier = 1.0f,
                    int damage = 50);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;

private:
    void StartExplosion();
    bool IsOutOfBounds(float screenW, float screenH) const;
};
