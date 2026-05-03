#include "PauseScene.h"
#include "AssetManager.h"
#include "GameConfig.h"
#include "Graphics.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include <windows.h>

PauseScene::PauseScene(Graphics& gfx) : m_gfx(gfx) {
    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");

    // Load button texture
    auto buttonTexture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/sheen__0011_Background.png");

    // Buttons positioned vertically on the left side
    float buttonWidth = 300.0f;
    float buttonHeight = 80.0f;
    float startX = 100.0f;
    float startY = 300.0f;
    float spacing = 100.0f;

    m_continueButton = std::make_unique<Button>(
        startX, startY, buttonWidth, buttonHeight, 1,
        L"Continue", m_font.get(), Button::TextAlignment::CENTER, 1.0f
    );
    m_continueButton->SetTextures(buttonTexture, buttonTexture, buttonTexture);

    m_settingButton = std::make_unique<Button>(
        startX, startY + spacing, buttonWidth, buttonHeight, 1,
        L"Setting", m_font.get(), Button::TextAlignment::CENTER, 1.0f
    );
    m_settingButton->SetTextures(buttonTexture, buttonTexture, buttonTexture);

    m_menuButton = std::make_unique<Button>(
        startX, startY + 2 * spacing, buttonWidth, buttonHeight, 1,
        L"Menu", m_font.get(), Button::TextAlignment::CENTER, 1.0f
    );
    m_menuButton->SetTextures(buttonTexture, buttonTexture, buttonTexture);

    m_exitButton = std::make_unique<Button>(
        startX, startY + 3 * spacing, buttonWidth, buttonHeight, 1,
        L"Exit", m_font.get(), Button::TextAlignment::CENTER, 1.0f
    );
    m_exitButton->SetTextures(buttonTexture, buttonTexture, buttonTexture);
}

void PauseScene::Update(float dt, InputManager& input, SceneManager& manager) {
    float mouseX = (float)input.GetMouseX();
    float mouseY = (float)input.GetMouseY();
    bool isClicked = input.IsLeftMouseClicked();

    m_continueButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_settingButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_menuButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_exitButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    if (m_continueButton->IsClicked()) {
        manager.PopScene(); // Resume gameplay
    }

    if (m_settingButton->IsClicked()) {
        // TODO: Implement settings scene
    }

    if (m_menuButton->IsClicked()) {
        manager.ChangeScene(std::make_unique<MenuScene>(m_gfx));
    }

    if (m_exitButton->IsClicked()) {
        PostQuitMessage(0);
    }
}

void PauseScene::Render(Graphics& gfx) {
    // Do not clear buffer to allow gameplay scene to show underneath
    // Optionally, render a semi-transparent overlay

    auto spriteBatch = gfx.GetSpriteBatch();
    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, gfx.GetScaleMatrix());

    // Render a semi-transparent black rectangle over the screen
    DirectX::XMFLOAT4 color(0.0f, 0.0f, 0.0f, 0.5f); // Semi-transparent black
    // But SpriteBatch doesn't support alpha directly, perhaps use a texture or skip

    // For simplicity, just render buttons without overlay
    m_continueButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, DirectX::Colors::White, DirectX::Colors::Black, 1.0f);
    m_settingButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, DirectX::Colors::White, DirectX::Colors::Black, 1.0f);
    m_menuButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, DirectX::Colors::White, DirectX::Colors::Black, 1.0f);
    m_exitButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, DirectX::Colors::White, DirectX::Colors::Black, 1.0f);

    spriteBatch->End();
}