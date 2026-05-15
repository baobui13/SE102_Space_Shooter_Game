#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Graphics.h"
#include <string>

class ScrollingBackground {
public:
    ScrollingBackground(
        Graphics& gfx,
        const std::wstring& texturePath,
        float scrollSpeed = 40.0f
    );

    void Update(float dt);
    void Render(Graphics& gfx);

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    float m_offsetY = 0.0f;
    float m_scrollSpeed;
};
