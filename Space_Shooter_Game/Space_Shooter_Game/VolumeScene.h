#pragma once

#include "Scene.h"
#include "Button.h"
#include <memory>

class VolumeScene : public Scene
{
public:
    VolumeScene(Graphics& gfx);
    ~VolumeScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

private:
    Graphics& m_gfx;

    std::unique_ptr<DirectX::SpriteFont> m_font;

    // BGM
    std::unique_ptr<Button> m_bgmMinusButton;
    std::unique_ptr<Button> m_bgmPlusButton;

    // SFX
    std::unique_ptr<Button> m_sfxMinusButton;
    std::unique_ptr<Button> m_sfxPlusButton;

    // Back
    std::unique_ptr<Button> m_backButton;
    float m_backgroundOffsetY = 0.0f;
    float m_backgroundScrollSpeed = 40.0f;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texOverlay;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texBgmLabel;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texSfxLabel;
};