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
};