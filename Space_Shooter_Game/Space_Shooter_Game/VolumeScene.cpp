#include "VolumeScene.h"

#include "AudioManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameConfig.h"

#include <algorithm>

VolumeScene::VolumeScene(Graphics& gfx)
    : m_gfx(gfx)
{
    auto& assets = AssetManager::GetInstance();

    auto texMinus = assets.GetTexture(gfx, L"Assets/Minus_BTN.png");
    auto texPlus = assets.GetTexture(gfx, L"Assets/Plus_BTN.png");
    auto texBack = assets.GetTexture(gfx, L"Assets/Back_BTN.png");

    m_texBgmLabel = assets.GetTexture(gfx, L"Assets/Bgm_BTN.png");
    m_texSfxLabel = assets.GetTexture(gfx, L"Assets/GM_BTN.png");

    m_font = std::make_unique<DirectX::SpriteFont>(
        gfx.GetDevice().Get(),
        L"Assets/Arial.spritefont"
    );

    m_backgroundTexture = assets.GetTexture(gfx, L"Assets/background.png");
    m_texOverlay = assets.GetTexture(gfx, L"Assets/WhitePoint.png");

    float btnW = 120.0f;
    float btnH = 100.0f;

    // ===== CĂN GIỮA UI =====
    float centerX = VIRTUAL_WIDTH / 2.0f;

    float labelW = 300.0f;
    float labelH = 120.0f;

    float rowSpacing = 250.0f;
    float startY = VIRTUAL_HEIGHT / 2.0f - rowSpacing / 2.0f;

    float spacing = 220.0f; // khoảng cách từ center ra 2 bên   
    float minusX = centerX - spacing - btnW / 2;
    float plusX = centerX + spacing - btnW / 2;

    float bgmY = startY;
    float sfxY = startY + rowSpacing;

    // ===== BGM =====
    m_bgmMinusButton = std::make_unique<Button>(minusX, bgmY, btnW, btnH);
    m_bgmMinusButton->SetTextures(texMinus, texMinus);

    m_bgmPlusButton = std::make_unique<Button>(plusX, bgmY, btnW, btnH);
    m_bgmPlusButton->SetTextures(texPlus, texPlus);

    // ===== SFX =====
    m_sfxMinusButton = std::make_unique<Button>(minusX, sfxY, btnW, btnH);
    m_sfxMinusButton->SetTextures(texMinus, texMinus);

    m_sfxPlusButton = std::make_unique<Button>(plusX, sfxY, btnW, btnH);
    m_sfxPlusButton->SetTextures(texPlus, texPlus);

    // ===== BACK =====
    m_backButton = std::make_unique<Button>(
        centerX - 200,
        sfxY + 250,
        400,
        140
    );
    m_backButton->SetTextures(texBack, texBack);
}

void VolumeScene::Update(float dt,
    InputManager& input,
    SceneManager& manager)
{
    float mouseX = (float)input.GetMouseX();
    float mouseY = (float)input.GetMouseY();
    bool click = input.IsLeftMouseClicked();

    m_bgmMinusButton->Update(mouseX, mouseY, click, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_bgmPlusButton->Update(mouseX, mouseY, click, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_sfxMinusButton->Update(mouseX, mouseY, click, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_sfxPlusButton->Update(mouseX, mouseY, click, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_backButton->Update(mouseX, mouseY, click, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    auto& audio = AudioManager::GetInstance();

    if (m_bgmMinusButton->IsClicked()) {
        float v = audio.GetBackgroundMusicVolume() - 0.1f;
        audio.SetBackgroundMusicVolume(std::clamp(v, 0.0f, 1.0f));
    }

    if (m_bgmPlusButton->IsClicked()) {
        float v = audio.GetBackgroundMusicVolume() + 0.1f;
        audio.SetBackgroundMusicVolume(std::clamp(v, 0.0f, 1.0f));
    }

    if (m_sfxMinusButton->IsClicked()) {
        float v = audio.GetSoundEffectVolume() - 0.1f;
        audio.SetSoundEffectVolume(std::clamp(v, 0.0f, 1.0f));
    }

    if (m_sfxPlusButton->IsClicked()) {
        float v = audio.GetSoundEffectVolume() + 0.1f;
        audio.SetSoundEffectVolume(std::clamp(v, 0.0f, 1.0f));
    }

    if (m_backButton->IsClicked()) {
        manager.PopScene();
    }

    // ===== SCROLL BACKGROUND =====
    m_backgroundOffsetY += m_backgroundScrollSpeed * dt;

    if (m_backgroundOffsetY >= VIRTUAL_HEIGHT) {
        m_backgroundOffsetY = 0.0f;
    }
}

void VolumeScene::Render(Graphics& gfx)
{
    gfx.ClearBuffer(0.0f, 0.0f, 0.0f);

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

    // ===== BACKGROUND SCROLL =====
    if (m_backgroundTexture)
    {
        LONG y = (LONG)m_backgroundOffsetY;

        RECT rect1 = { 0, y - (LONG)VIRTUAL_HEIGHT, (LONG)VIRTUAL_WIDTH, y };
        RECT rect2 = { 0, y, (LONG)VIRTUAL_WIDTH, y + (LONG)VIRTUAL_HEIGHT };

        spriteBatch->Draw(m_backgroundTexture.Get(), rect1);
        spriteBatch->Draw(m_backgroundTexture.Get(), rect2);
    }

    // ===== OVERLAY =====
    if (m_texOverlay)
    {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };
        spriteBatch->Draw(m_texOverlay.Get(), bgRect, DirectX::Colors::Black * 0.45f);
    }

    float centerX = VIRTUAL_WIDTH / 2.0f;
    float rowSpacing = 250.0f;
    float startY = VIRTUAL_HEIGHT / 2.0f - rowSpacing / 2.0f;

    float bgmY = startY;
    float sfxY = startY + rowSpacing;

    // ===== LABEL =====
    if (m_texBgmLabel)
    {
        RECT rect = { (LONG)(centerX - 160), (LONG)(bgmY - 180), (LONG)(centerX + 160), (LONG)(bgmY) };
        spriteBatch->Draw(m_texBgmLabel.Get(), rect);
    }

    if (m_texSfxLabel)
    {
        RECT rect = { (LONG)(centerX - 150), (LONG)(sfxY - 160), (LONG)(centerX + 150), (LONG)(sfxY) };
        spriteBatch->Draw(m_texSfxLabel.Get(), rect);
    }

    auto& audio = AudioManager::GetInstance();

    // ===== % TEXT =====
    std::wstring bgmPercent = std::to_wstring((int)(audio.GetBackgroundMusicVolume() * 100)) + L"%";
    std::wstring sfxPercent = std::to_wstring((int)(audio.GetSoundEffectVolume() * 100)) + L"%";

    m_font->DrawString(spriteBatch, bgmPercent.c_str(),
        DirectX::XMFLOAT2(centerX - 40, bgmY + 20),
        DirectX::Colors::White, 0, { 0,0 }, 1.3f);

    m_font->DrawString(spriteBatch, sfxPercent.c_str(),
        DirectX::XMFLOAT2(centerX - 40, sfxY + 20),
        DirectX::Colors::White, 0, { 0,0 }, 1.3f);

    // ===== BUTTONS =====
    m_bgmMinusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_bgmPlusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_sfxMinusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_sfxPlusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_backButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    spriteBatch->End();
}