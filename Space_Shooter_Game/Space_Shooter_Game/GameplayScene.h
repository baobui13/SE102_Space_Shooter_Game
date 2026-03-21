#pragma once
#include "Graphics.h"
#include "InputManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Scene.h"
#include <memory>

class GameplayScene : public Scene {
private:
    Graphics& m_gfx;
    std::unique_ptr<Player> m_player;

    std::vector<std::unique_ptr<Bullet>> m_bullets;
public:
    GameplayScene(Graphics& gfx);
    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx);
};