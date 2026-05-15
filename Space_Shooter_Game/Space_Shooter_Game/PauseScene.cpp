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

    float buttonWidth = 520.0f;
    float buttonHeight = 220.0f;

    // spacing tự động theo chiều cao nút
    float spacing = buttonHeight * 0.25f; // 25% chiều cao nút

    // Tổng chiều cao UI
    float totalHeight = 4 * buttonHeight + 3 * spacing;

    // Nếu bị tràn màn hình → scale xuống
    if (totalHeight > VIRTUAL_HEIGHT * 0.9f) {
        float scale = (VIRTUAL_HEIGHT * 0.9f) / totalHeight;
        buttonHeight *= scale;
        spacing *= scale;
        buttonWidth *= scale;
        totalHeight = 4 * buttonHeight + 3 * spacing;
    }

    // Căn giữa
    float startY = (VIRTUAL_HEIGHT - totalHeight) / 2.0f;
    float startX = (VIRTUAL_WIDTH - buttonWidth) / 2.0f;

    m_continueButton = std::make_unique<Button>(
        startX, startY, buttonWidth, buttonHeight
    );

    m_continueButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Continue_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Continue_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Continue_BTN.png")
    );

    m_settingButton = std::make_unique<Button>(
        startX, startY + (buttonHeight + spacing), buttonWidth, buttonHeight
    );

    m_settingButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Setting_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Setting_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Setting_BTN.png")
    );

    m_menuButton = std::make_unique<Button>(
        startX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight
    );

    m_menuButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Menu_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Menu_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Menu_BTN.png")
    );

    m_exitButton = std::make_unique<Button>(
        startX, startY + 3 * (buttonHeight + spacing), buttonWidth, buttonHeight
    );

    m_exitButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_Scene_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_Scene_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_Scene_BTN.png")
    );
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
    m_continueButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_settingButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_menuButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_exitButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    spriteBatch->End();
}