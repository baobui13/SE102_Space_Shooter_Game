#pragma once
#include "Scene.h"
#include "Button.h"
#include <memory>

class PauseScene : public Scene {
public:
    PauseScene(Graphics& gfx);
    ~PauseScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

private:
    Graphics& m_gfx;
    std::unique_ptr<DirectX::SpriteFont> m_font;

    // Buttons
    std::unique_ptr<Button> m_continueButton;
    std::unique_ptr<Button> m_settingButton;
    std::unique_ptr<Button> m_menuButton;
    std::unique_ptr<Button> m_exitButton;
};