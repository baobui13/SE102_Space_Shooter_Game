#include "SettingScene.h"

#include "AudioManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameConfig.h"

#include <algorithm>

SettingScene::SettingScene(Graphics& gfx)
    : m_gfx(gfx)
{
    auto& assets = AssetManager::GetInstance();

    auto texMinus = assets.GetTexture(gfx, L"Assets/Minus_BTN.png");
    auto texPlus = assets.GetTexture(gfx, L"Assets/Plus_BTN.png");
    auto texBack = assets.GetTexture(gfx, L"Assets/Back_BTN.png");

    m_font = std::make_unique<DirectX::SpriteFont>(
        gfx.GetDevice().Get(),
        L"Assets/Arial.spritefont"
    );

    // ❗ chỉ background tĩnh (KHÔNG SCROLL)
    m_backgroundTexture = assets.GetTexture(gfx, L"Assets/background.png");
    m_texOverlay = assets.GetTexture(gfx, L"Assets/WhitePoint.png");

    float btnW = 150.0f;
    float btnH = 120.0f;

    // ===== BGM =====
    m_bgmMinusButton = std::make_unique<Button>(500, 300, btnW, btnH);
    m_bgmMinusButton->SetTextures(texMinus, texMinus);

    m_bgmPlusButton = std::make_unique<Button>(850, 300, btnW, btnH);
    m_bgmPlusButton->SetTextures(texPlus, texPlus);

    // ===== SFX =====
    m_sfxMinusButton = std::make_unique<Button>(500, 600, btnW, btnH);
    m_sfxMinusButton->SetTextures(texMinus, texMinus);

    m_sfxPlusButton = std::make_unique<Button>(850, 600, btnW, btnH);
    m_sfxPlusButton->SetTextures(texPlus, texPlus);

    // ===== BACK =====
    m_backButton = std::make_unique<Button>(700, 900, 400, 160);
    m_backButton->SetTextures(texBack, texBack);
}

void SettingScene::Update(float dt,
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

    // ===== BGM =====
    if (m_bgmMinusButton->IsClicked())
    {
        float v = audio.GetBackgroundMusicVolume();
        v -= 0.1f;
        audio.SetBackgroundMusicVolume(std::clamp(v, 0.0f, 1.0f));
    }

    if (m_bgmPlusButton->IsClicked())
    {
        float v = audio.GetBackgroundMusicVolume();
        v += 0.1f;
        audio.SetBackgroundMusicVolume(std::clamp(v, 0.0f, 1.0f));
    }

    // ===== SFX (bao gồm UI luôn) =====
    if (m_sfxMinusButton->IsClicked())
    {
        float v = audio.GetSoundEffectVolume();
        v -= 0.1f;

        v = std::clamp(v, 0.0f, 1.0f);

        audio.SetSoundEffectVolume(v);
    }

    if (m_sfxPlusButton->IsClicked())
    {
        float v = audio.GetSoundEffectVolume();
        v += 0.1f;

        v = std::clamp(v, 0.0f, 1.0f);

        audio.SetSoundEffectVolume(v);

    }

    if (m_backButton->IsClicked())
    {
        manager.PopScene();
    }
}

void SettingScene::Render(Graphics& gfx)
{

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

    // ===== BACKGROUND (TĨNH) =====
   // ===== OVERLAY GIỐNG PAUSE =====
    if (m_texOverlay)
    {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };

        spriteBatch->Draw(
            m_texOverlay.Get(),
            bgRect,
            DirectX::Colors::Black * 0.5f
        );
    }

    // ===== OVERLAY =====
    if (m_texOverlay)
    {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };

        spriteBatch->Draw(
            m_texOverlay.Get(),
            bgRect,
            DirectX::Colors::Black * 0.45f
        );
    }

    auto& audio = AudioManager::GetInstance();

    std::wstring bgmText =
        L"Background Music: " +
        std::to_wstring((int)(audio.GetBackgroundMusicVolume() * 100)) +
        L"%";

    std::wstring sfxText =
        L"Game Music: " +
        std::to_wstring((int)(audio.GetSoundEffectVolume() * 100)) +
        L"%";

    m_font->DrawString(spriteBatch, bgmText.c_str(), DirectX::XMFLOAT2(550, 220));
    m_font->DrawString(spriteBatch, sfxText.c_str(), DirectX::XMFLOAT2(550, 520));

    m_bgmMinusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_bgmPlusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    m_sfxMinusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    m_sfxPlusButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    m_backButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    spriteBatch->End();
}