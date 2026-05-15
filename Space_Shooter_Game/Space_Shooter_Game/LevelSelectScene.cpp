#include "LevelSelectScene.h"
#include "AssetManager.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "Level3Scene.h"
#include "Level4Scene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "GameConfig.h"
#include <DirectXColors.h>
#include <string>
#include "AudioManager.h"

constexpr float CARD_WIDTH = 260.0f;
constexpr float CARD_HEIGHT = 520.0f;
constexpr float CARD_SPACING = 40.0f;

LevelSelectScene::LevelSelectScene(Graphics& gfx)
    : m_gfx(gfx) {
    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");

    AssetManager& assets = AssetManager::GetInstance();
    auto cardTexture = assets.GetTexture(gfx, L"Assets/sheen__0011_Background.png");
    m_texCardBG = cardTexture;
    m_texOverlay = assets.GetTexture(gfx, L"Assets/WhitePoint.png");

    m_backButton = std::make_unique<Button>(0, 0, 300.0f, 200.0f);
    m_backButton->SetTextures(
        assets.GetTexture(gfx, L"Assets/Back_BTN.png"),
        assets.GetTexture(gfx, L"Assets/Back_BTN.png"),
        assets.GetTexture(gfx, L"Assets/Back_BTN.png")
    );

    m_backgroundTexture =
        assets.GetTexture(gfx, L"Assets/background.png");
    m_titleTexture = assets.GetTexture(gfx, L"Assets/Choose_Level_BTN.png");

    m_levelNames = {
        L"Level 1",
        L"Level 2",
        L"Level 3",
        L"Level 4"
    };
}

void LevelSelectScene::Update(float dt, InputManager& input, SceneManager& manager) {
    m_mouseX = (float)input.GetMouseX();
    m_mouseY = (float)input.GetMouseY();

    if (m_backButton) {
        m_backButton->Update(m_mouseX, m_mouseY, input.IsLeftMouseClicked(), VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
        if (m_backButton->IsClicked()) {
            manager.ChangeScene(std::make_unique<MenuScene>(m_gfx));
            return;
        }
    }

    if (input.IsKeyPressed(VK_ESCAPE)) {
        manager.ChangeScene(std::make_unique<MenuScene>(m_gfx));
        return;
    }

    float totalWidth = (4 * CARD_WIDTH) + (3 * CARD_SPACING);
    float startX = (VIRTUAL_WIDTH - totalWidth) / 2.0f;
    float cardY = (VIRTUAL_HEIGHT - CARD_HEIGHT) / 2.0f;

    for (int i = 0; i < 4; ++i) {

        float cardX = startX + i * (CARD_WIDTH + CARD_SPACING);

        bool isHovered =
            m_mouseX >= cardX &&
            m_mouseX <= cardX + CARD_WIDTH &&
            m_mouseY >= cardY &&
            m_mouseY <= cardY + CARD_HEIGHT;

        // Hover sound
        if (isHovered && !m_wasHovered[i]) {
            AudioManager::GetInstance().PlayUiEffect(AudioIds::UiHoverLevel);
        }

        // Lưu trạng thái hover
        m_wasHovered[i] = isHovered;

        // Click vào level
        if (isHovered && input.IsLeftMouseClicked()) {

            switch (i) {
            case 0:
                manager.ChangeScene(std::make_unique<Level1Scene>(m_gfx));
                break;

            case 1:
                manager.ChangeScene(std::make_unique<Level2Scene>(m_gfx));
                break;

            case 2:
                manager.ChangeScene(std::make_unique<Level3Scene>(m_gfx));
                break;

            case 3:
                manager.ChangeScene(std::make_unique<Level4Scene>(m_gfx));
                break;
            }

            return;
        }
    }
    m_backgroundOffsetY += m_backgroundScrollSpeed * dt;

    if (m_backgroundOffsetY >= VIRTUAL_HEIGHT) {
        m_backgroundOffsetY = 0.0f;
    }
}

void LevelSelectScene::Render(Graphics& gfx) {
    gfx.ClearBuffer(0.0f, 0.0f, 0.0f);
    auto spriteBatch = gfx.GetSpriteBatch();

    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, gfx.GetScaleMatrix());

    if (m_backgroundTexture) {

        LONG y = (LONG)m_backgroundOffsetY;

        RECT rect1 = {
            0,
            y - (LONG)VIRTUAL_HEIGHT,
            (LONG)VIRTUAL_WIDTH,
            y
        };

        RECT rect2 = {
            0,
            y,
            (LONG)VIRTUAL_WIDTH,
            y + (LONG)VIRTUAL_HEIGHT
        };

        spriteBatch->Draw(
            m_backgroundTexture.Get(),
            rect1,
            nullptr,
            DirectX::Colors::White
        );

        spriteBatch->Draw(
            m_backgroundTexture.Get(),
            rect2,
            nullptr,
            DirectX::Colors::White
        );
    }

    if (m_texOverlay) {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };
        spriteBatch->Draw(m_texOverlay.Get(), bgRect, DirectX::Colors::Black * 0.45f);
    }

    // ====== CĂN GIỮA UI ======
    const float titleW = 700.0f;
    const float titleH = 300.0f;
    const float gap = 1.0f;

    // tổng chiều cao
    float totalHeight = titleH + gap + CARD_HEIGHT;

    // căn giữa dọc
    float startY = (VIRTUAL_HEIGHT - totalHeight) / 2;

    // vị trí title
    m_titlePos = {
        (VIRTUAL_WIDTH - titleW) / 2,
        startY
    };

    // card nằm dưới title
    float cardY = startY + titleH + gap;

    // căn giữa ngang
    float totalWidth = (4 * CARD_WIDTH) + (3 * CARD_SPACING);
    float startX = (VIRTUAL_WIDTH - totalWidth) / 2.0f;

    // ====== VẼ TITLE ======
    RECT titleRect = {
        (LONG)m_titlePos.x,
        (LONG)m_titlePos.y,
        (LONG)(m_titlePos.x + titleW),
        (LONG)(m_titlePos.y + titleH)
    };

    if (m_titleTexture) {
        spriteBatch->Draw(m_titleTexture.Get(), titleRect);
    }

    for (int i = 0; i < 4; ++i) {
        float cardX = startX + i * (CARD_WIDTH + CARD_SPACING);
        RECT destCard = { (LONG)cardX, (LONG)cardY, (LONG)(cardX + CARD_WIDTH), (LONG)(cardY + CARD_HEIGHT) };

        bool isHovered = (m_mouseX >= cardX && m_mouseX <= cardX + CARD_WIDTH &&
            m_mouseY >= cardY && m_mouseY <= cardY + CARD_HEIGHT);

        DirectX::XMVECTOR tint = isHovered ? DirectX::Colors::LightGray : DirectX::Colors::White;
        if (m_texCardBG) {
            spriteBatch->Draw(m_texCardBG.Get(), destCard, tint);
        }

        // Draw level label at bottom of card
        std::wstring levelName = m_levelNames[i];
        DirectX::XMFLOAT2 textPos(cardX + CARD_WIDTH / 2.0f, cardY + CARD_HEIGHT - 60.0f);
        m_font->DrawString(spriteBatch, levelName.c_str(), textPos, DirectX::Colors::Black,
            0.0f, DirectX::XMFLOAT2(0.5f, 0.5f), 1.1f);

        // Draw level number above name for clarity
        std::wstring numberText = std::to_wstring(i + 1);
        m_font->DrawString(spriteBatch, numberText.c_str(), DirectX::XMFLOAT2(cardX + CARD_WIDTH / 2.0f, cardY + 140.0f), DirectX::Colors::Yellow,
            0.0f, DirectX::XMFLOAT2(0.5f, 0.5f), 2.4f);
    }

    if (m_backButton) {
        m_backButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, DirectX::Colors::White, DirectX::Colors::White);
    }

    spriteBatch->End();
}
