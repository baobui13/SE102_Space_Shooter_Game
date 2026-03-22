#pragma once
#include "Graphics.h"
#include "InputManager.h"
#include "Player.h"
#include "Bullet.h"
#include "ExpOrb.h"
#include "Scene.h"
#include <memory>
#include <SpriteFont.h>

class GameplayScene : public Scene {
private:
    Graphics& m_gfx;
    std::unique_ptr<Player> m_player;

    std::vector<std::unique_ptr<Bullet>> m_bullets;
    std::unique_ptr<DirectX::SpriteFont> m_font;
    std::vector<std::unique_ptr<ExpOrb>> m_expOrbs;
public:
    GameplayScene(Graphics& gfx);
    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx);
};