#pragma once
#include "Scene.h"
#include "Button.h"
#include "TalentTree.h"
#include "Player.h"
#include <memory>

class TalentTreeScene : public Scene {
public:
    TalentTreeScene(Graphics& gfx, Player* player);
    ~TalentTreeScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

private:
    Graphics& m_gfx;
    Player* m_player;
    
    std::unique_ptr<Button> m_backButton;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;
};
