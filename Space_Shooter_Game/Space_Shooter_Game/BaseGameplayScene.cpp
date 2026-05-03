#include "AudioManager.h"
#include "BaseGameplayScene.h"
#include "AssetManager.h"
#include "ExplodingBulletSkill.h"
#include "GameConfig.h"
#include "GameContext.h"
#include "LaserSkill.h"
#include "LevelUpScene.h"
#include "MenuScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "ShieldSkill.h"
#include <DirectXColors.h>
#include <string>

BaseGameplayScene::BaseGameplayScene(Graphics& gfx, int levelIndex)
    : m_gfx(gfx)
    , m_bulletPool(gfx)
    , m_levelIndex(levelIndex)
    , m_backgroundColor{ 0.02f, 0.02f, 0.1f } {
    AudioManager::GetInstance().PlayMusic(AudioIds::GameplayMusic);

    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/GoodTimingRg.spritefont");

    m_hpBar = std::make_unique<ProgressBar>(20.0f, 20.0f, 500.0f, 30.0f, 2);
    m_hpBar->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/BarV1_Bar.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/BarV1_ProgressBar.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/BarV1_ProgressBarBorder.png")
    );

    auto buttonTexture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/sheen__0011_Background.png");
    m_pauseButton = std::make_unique<Button>(VIRTUAL_WIDTH - 90.0f, 20.0f, 70.0f, 70.0f, 1, L"Pause", m_font.get(), Button::TextAlignment::CENTER, 1.0f);
    m_pauseButton->SetTextures(buttonTexture, buttonTexture, buttonTexture);

    InitializePlayer();
}

void BaseGameplayScene::SetBackgroundColor(const DirectX::XMFLOAT3& color) {
    m_backgroundColor = color;
    m_backgroundTexture.Reset();
}

void BaseGameplayScene::SetBackgroundTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) {
    m_backgroundTexture = texture;
}

void BaseGameplayScene::InitializePlayer() {
    m_player = std::make_unique<Player>(m_gfx, VIRTUAL_WIDTH / 2.0f - 32.0f, VIRTUAL_HEIGHT - 200.0f);
    m_player->AddSkill(std::make_unique<LaserSkill>());
    m_player->AddSkill(std::make_unique<ExplodingBulletSkill>());
    m_player->AddSkill(std::make_unique<ShieldSkill>());
}

void BaseGameplayScene::InitializeLevel() {
    // Default implementation - empty. Subclasses override to add enemies
}

void BaseGameplayScene::HandleLevelInput(InputManager& input, SceneManager& manager) {
    // Default implementation - can be overridden in subclasses
    if (input.IsKeyPressed('E')) {
        m_player->GainExp(50);
    }

    if (input.IsKeyPressed('R')) {
        m_player->TakeDamage(20);
    }
}

void BaseGameplayScene::RenderLevelElements(DirectX::SpriteBatch* spriteBatch) {
    // Default implementation - empty. Subclasses override to render level-specific elements
}

void BaseGameplayScene::Update(float dt, InputManager& input, SceneManager& manager) {
    GameContext ctx(
        m_gfx,
        input,
        AssetManager::GetInstance(),
        m_bulletPool,
        m_entityManager,
        *m_player,
        VIRTUAL_WIDTH,
        VIRTUAL_HEIGHT
    );

    m_player->Update(dt, ctx);
    m_entityManager.UpdateAll(dt, ctx);
    m_bulletPool.Update(dt, ctx);

    float mouseX = (float)input.GetMouseX();
    float mouseY = (float)input.GetMouseY();
    bool isClicked = input.IsLeftMouseClicked();
    if (m_pauseButton) {
        m_pauseButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
        if (m_pauseButton->IsClicked()) {
            manager.PushScene(std::make_unique<PauseScene>(m_gfx));
        }
    }

    if (input.IsKeyPressed(VK_ESCAPE)) {
        manager.PushScene(std::make_unique<PauseScene>(m_gfx));
    }

    HandleLevelInput(input, manager);

    if (m_player->GetUpgradePoints() > 0 && input.IsKeyPressed('U')) {
        AudioManager::GetInstance().PlayUiEffect(AudioIds::UiOpenLevelUp);
        manager.PushScene(std::make_unique<LevelUpScene>(m_gfx, *m_player));
    }
}

void BaseGameplayScene::Render(Graphics& gfx) {
    if (m_backgroundTexture) {
        gfx.ClearBuffer(0.0f, 0.0f, 0.0f);
    }
    else {
        gfx.ClearBuffer(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z);
    }

    auto spriteBatch = gfx.GetSpriteBatch();

    spriteBatch->Begin(
        DirectX::SpriteSortMode_Deferred,
        gfx.GetStates()->NonPremultiplied(),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        gfx.GetScaleMatrix()
    );

    if (m_backgroundTexture) {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };
        spriteBatch->Draw(m_backgroundTexture.Get(), bgRect, DirectX::Colors::White);
    }

    m_entityManager.RenderAll(gfx);
    m_bulletPool.Render(gfx);
    m_player->Render(gfx);

    RenderLevelElements(spriteBatch);

    std::wstring uiLeft =
        L"--- PLAYER STATS ---\n"
        L"HP: " + std::to_wstring(m_player->GetHp()) + L" / " + std::to_wstring(m_player->GetMaxHp()) + L"\n" +
        L"LEVEL: " + std::to_wstring(m_player->GetLevel()) + L"\n" +
        L"EXP: " + std::to_wstring(m_player->GetCurrentExp()) + L" / " + std::to_wstring(m_player->GetExpToNextLevel()) + L"\n" +
        L"SPEED: " + std::to_wstring((int)m_player->GetSpeed()) + L"\n" +
        L"MAGNET: " + std::to_wstring((int)m_player->GetMagnetRange());

    std::wstring uiRight =
        L"--- COMBAT STATS ---\n"
        L"DAMAGE: " + std::to_wstring(m_player->GetAttackDamage()) + L"\n" +
        L"ATK SPD: " + std::to_wstring((int)(m_player->GetAttackSpeed() * 10.0f) / 10) + L" /s\n" +
        L"RANGE: " + std::to_wstring((int)m_player->GetAttackRange()) + L"\n" +
        L"CD MULT: " + std::to_wstring((int)(m_player->GetCooldownMultiplier() * 100)) + L"%\n" +
        L"SKILL SIZE: " + std::to_wstring((int)(m_player->GetSkillSizeMultiplier() * 100)) + L"%";

    m_font->DrawString(spriteBatch, uiLeft.c_str(), DirectX::XMFLOAT2(10.0f, 10.0f), DirectX::Colors::Yellow);
    m_font->DrawString(spriteBatch, uiRight.c_str(), DirectX::XMFLOAT2(200.0f, 10.0f), DirectX::Colors::LightGreen);

    if (m_player->GetUpgradePoints() > 0) {
        std::wstring notify =
            L"Ban co " + std::to_wstring(m_player->GetUpgradePoints()) + L" luot nang cap! Bam 'U' de dung.";
        m_font->DrawString(spriteBatch, notify.c_str(), DirectX::XMFLOAT2(10.0f, 200.0f), DirectX::Colors::Cyan);
    }

    // Draw HP Bar
    float hpPercent = (float)m_player->GetHp() / m_player->GetMaxHp();
    m_hpBar->Render(spriteBatch, hpPercent, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    if (m_pauseButton) {
        m_pauseButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, DirectX::Colors::White, DirectX::Colors::Black, 1.0f);
    }

    spriteBatch->End();
}
