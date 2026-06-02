#pragma once

#include "GameObject.h"
#include <string>

class TalentOrb : public GameObject {
public:
    TalentOrb(Graphics& gfx, float x, float y, int pointValue = 1);

    void Update(float dt, ::GameContext& ctx) override;

    int GetPointValue() const { return m_pointValue; }

private:
    int m_pointValue = 1;
    float m_magnetSpeed = 500.0f;
    float m_fallSpeed = 40.0f;
    std::string m_collectColliderName;
};
