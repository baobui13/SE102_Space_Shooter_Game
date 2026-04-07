#pragma once
#include "Graphics.h"
#include "InputManager.h"
#include "Player.h"
#include "Bullet.h"
#include "ExpOrb.h"
#include "Scene.h"
#include <memory>
#include <SpriteFont.h>
#include "ProgressBar.h"

#include "EntityManager.h"
#include "BulletPool.h"

class GameplayScene : public Scene {
private:
    Graphics& m_gfx;
    std::unique_ptr<Player> m_player;
    EntityManager m_entityManager;
    BulletPool m_bulletPool;

    std::unique_ptr<DirectX::SpriteFont> m_font;
    std::unique_ptr<ProgressBar> m_hpBar;
public:
    GameplayScene(Graphics& gfx);
    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;
};