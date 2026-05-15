#pragma once
#include "Scene.h"
#include "Button.h"
#include "Scene.h"
#include <memory>
#include <vector>

class MenuScene : public Scene {
public:
    MenuScene(Graphics& gfx);
    ~MenuScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

private:
    Graphics& m_gfx;

    // Danh sách các nút trên Menu
    std::unique_ptr<Button> m_playButton;
    std::unique_ptr<Button> m_exitButton;
    std::unique_ptr<Button> m_volumeButton;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;

    float m_backgroundOffsetY = 0.0f;
    float m_backgroundScrollSpeed = 40.0f;
};