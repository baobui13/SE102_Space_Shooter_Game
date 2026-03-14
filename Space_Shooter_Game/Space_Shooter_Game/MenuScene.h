#pragma once
#include "Scene.h"
#include "Button.h"
#include <memory>
#include <vector>

class MenuScene : public Scene {
public:
    MenuScene(Graphics& gfx);
    ~MenuScene() = default;

    void Update(float dt) override;
    void Render(Graphics& gfx) override;

private:
    // Danh sách các nút trên Menu
    std::unique_ptr<Button> m_playButton;
    std::unique_ptr<Button> m_exitButton;
};