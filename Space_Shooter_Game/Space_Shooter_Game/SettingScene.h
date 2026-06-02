#pragma once

#include "Scene.h"
#include "Button.h"
#include <memory>

class SettingScene : public Scene
{
public:
    SettingScene(Graphics& gfx);
    ~SettingScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

private:
    Graphics& m_gfx;

    std::unique_ptr<DirectX::SpriteFont> m_font;

    std::unique_ptr<Button> m_bgmMinusButton;
    std::unique_ptr<Button> m_bgmPlusButton;

    std::unique_ptr<Button> m_sfxMinusButton;
    std::unique_ptr<Button> m_sfxPlusButton;

    std::unique_ptr<Button> m_backButton;

    // ❌ KHÔNG CẦN scroll nữa
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texOverlay;
}; 
