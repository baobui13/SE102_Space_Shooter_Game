#pragma once
#include "GameObject.h"

class Laser : public GameObject {
private:
    float m_remainingDuration;
    int m_damage;
    float m_sizeMultiplier;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    RECT m_bodyRect;
    RECT m_impactRect;

public:
    Laser(Graphics& gfx, float duration, int damage, float sizeMultiplier = 1.0f);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;
};
