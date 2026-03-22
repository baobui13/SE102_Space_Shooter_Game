#pragma once
#include "Scene.h"
#include "Player.h"
#include "Upgrade.h"
#include <SpriteFont.h>
#include <vector>
#include <memory>

class LevelUpScene : public Scene {
private:
    Graphics& m_gfx;
    Player& m_player;
    std::unique_ptr<DirectX::SpriteFont> m_font;

    std::vector<UpgradeOption> m_allUpgrades;
    std::vector<UpgradeOption> m_currentChoices;

    void GenerateChoices(); // Hàm bốc 3 thẻ

public:
    LevelUpScene(Graphics& gfx, Player& player);
    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;
};