#include "TalentTreeScene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "GameConfig.h"

TalentTreeScene::TalentTreeScene(Graphics& gfx, Player* player)
    : m_gfx(gfx), m_player(player)
{
    // Ensure player pointer is valid, we can assert or just check in methods
    
    // Create a Back button at top left
    m_backButton = std::make_unique<Button>(50.0f, 50.0f, 200.0f, 80.0f);
    m_backButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_BTN.png")
    );
    // Ideally use a different texture for back button if it exists, or exit works for now

    m_backgroundTexture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/background.png");
    
    // Initialize TalentTree textures
    // TalentTree is now initialized in BaseGameplayScene::InitializePlayer
}

void TalentTreeScene::Update(float dt, InputManager& input, SceneManager& manager) {
    float mouseX = (float)input.GetMouseX();
    float mouseY = (float)input.GetMouseY();
    bool isClicked = input.IsLeftMouseClicked();

    m_backButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    if (m_backButton->IsClicked()) {
        manager.PopScene();
        return;
    }

    if (m_player) {
        m_player->GetTalentTree().Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, *m_player);
    }
}

void TalentTreeScene::Render(Graphics& gfx) {
    gfx.ClearBuffer(0.1f, 0.1f, 0.1f);
    auto spriteBatch = gfx.GetSpriteBatch();
    
    // Start drawing
    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, gfx.GetScaleMatrix());
    
    if (m_backgroundTexture) {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };
        spriteBatch->Draw(m_backgroundTexture.Get(), bgRect, DirectX::Colors::DarkGray); // Dim the background
    }

    if (m_player) {
        // Load font locally for drawing text (could be cached)
        static std::unique_ptr<DirectX::SpriteFont> font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");
        
        m_player->GetTalentTree().Render(gfx, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, font.get());
        
        // Draw Upgrade Points
        std::wstring pointsText = L"Upgrade Points: " + std::to_wstring(m_player->GetUpgradePoints());
        font->DrawString(spriteBatch, pointsText.c_str(), DirectX::XMFLOAT2(VIRTUAL_WIDTH - 400.0f, 50.0f), DirectX::Colors::Yellow);
        
        // Draw Tooltip
        const TalentNode* hoveredNode = m_player->GetTalentTree().GetHoveredNode();
        if (hoveredNode) {
            std::wstring desc = hoveredNode->GetDescription();
            std::wstring status = hoveredNode->IsUnlocked() ? L"(Unlocked)" : L"(Locked)";
            // Draw description at bottom left
            font->DrawString(spriteBatch, desc.c_str(), DirectX::XMFLOAT2(50.0f, VIRTUAL_HEIGHT - 100.0f), DirectX::Colors::White);
            font->DrawString(spriteBatch, status.c_str(), DirectX::XMFLOAT2(50.0f, VIRTUAL_HEIGHT - 60.0f), hoveredNode->IsUnlocked() ? DirectX::Colors::Green : DirectX::Colors::Red);
        }
    }
    
    m_backButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    
    spriteBatch->End();
}
