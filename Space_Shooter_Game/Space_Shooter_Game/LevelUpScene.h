#pragma once
#include "Scene.h"
#include "Player.h"
#include "UpgradeManager.h"
#include <SpriteFont.h>
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <d3d11.h>

class LevelUpScene : public Scene {
private:
    Graphics& m_gfx;
    Player& m_player;
    std::unique_ptr<DirectX::SpriteFont> m_font;
    std::unique_ptr<DirectX::SpriteFont> m_descFont;

    std::vector<UpgradeOption> m_currentChoices;

    // --- Texture nền thẻ trắng duy nhất ---
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texWhiteCardBG;

    // --- Cần có 1 texture trắng nhỏ 1x1 pixel để vẽ nền mờ ---
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texWhitePoint;

    // --- ĐỊNH NGHĨA TOẠ ĐỘ CẮT ẢNH (SOURCE RECTANGLES) ---
    // Vì chúng ta dùng nguyên một tệp ảnh làm nền, nên không cần cắt ảnh nữa.

    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;

    void GenerateChoices();

public:
    LevelUpScene(Graphics& gfx, Player& player);
    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;
};