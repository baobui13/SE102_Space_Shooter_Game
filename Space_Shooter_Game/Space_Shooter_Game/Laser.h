#pragma once
#include "GameObject.h"
#include "Graphics.h"
#include "GameContext.h"
#include <wrl.h>
#include <d3d11.h>

class Laser : public GameObject {
private:
    float m_duration;
    int m_damage;
    float m_sizeMultiplier;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    
    // Tọa độ cắt ảnh lazer_1.png theo thiết kế người dùng
    RECT m_bodyRect;
    RECT m_impactRect;

public:
    Laser(Graphics& gfx, float duration, int damage, float sizeMultiplier = 1.0f);

    void Update(float dt, ::GameContext& ctx) override;
    void Render(Graphics& gfx) override;
};
