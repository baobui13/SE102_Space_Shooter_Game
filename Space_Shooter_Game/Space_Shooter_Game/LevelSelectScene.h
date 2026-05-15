#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "Button.h"
#include "Graphics.h"
#include "InputManager.h"
#include <SpriteFont.h>
#include <vector>
#include <wrl/client.h>
#include <d3d11.h>
#include <array>

class LevelSelectScene : public Scene {
public:
    LevelSelectScene(Graphics& gfx);
    ~LevelSelectScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

private:
    Graphics& m_gfx;
    std::unique_ptr<DirectX::SpriteFont> m_font;
    std::unique_ptr<Button> m_backButton;
    std::array<bool, 4> m_wasHovered = { false, false, false, false };
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleTexture;
    DirectX::XMFLOAT2 m_titlePos;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texCardBG;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texOverlay;

    std::vector<std::wstring> m_levelNames;
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;

    float m_backgroundOffsetY = 0.0f;
    float m_backgroundScrollSpeed = 40.0f;
};
