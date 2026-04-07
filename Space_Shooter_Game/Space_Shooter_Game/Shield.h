#pragma once
#include "GameObject.h"

class Shield : public GameObject {
private:
    float m_remainingDuration;

public:
    Shield(Graphics& gfx, float duration, float sizeMultiplier = 1.0f);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;
};
