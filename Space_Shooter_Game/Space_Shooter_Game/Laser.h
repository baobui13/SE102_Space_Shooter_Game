#pragma once
#include "GameObject.h"
#include "SpriteAnimation.h"

class Laser : public GameObject {
private:
    float m_remainingDuration;
    int m_damage;
    float m_sizeMultiplier;
    SpriteAnimation m_bodyAnim;
    float m_bodyDrawWidth = 0.0f;
    float m_bodySegmentHeight = 0.0f;
    float m_bodyUniformScale = 1.0f;
    int m_bodyFrameCount = 1;

public:
    Laser(Graphics& gfx, float duration, int damage, float sizeMultiplier = 1.0f);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;
};
