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
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texCardBG;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texOverlay;

    std::vector<std::wstring> m_levelNames;
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;
};
