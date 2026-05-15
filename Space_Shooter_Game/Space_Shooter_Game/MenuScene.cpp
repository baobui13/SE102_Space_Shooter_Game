#include "AudioManager.h"
#include "AssetManager.h"
#include "LevelSelectScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "Graphics.h"
#include "InputManager.h"
#include "GameConfig.h"

MenuScene::MenuScene(Graphics& gfx) : m_gfx(gfx) {
    AudioManager::GetInstance().PlayMusic(AudioIds::MenuMusic);

    m_backgroundTexture =
        AssetManager::GetInstance().GetTexture(
            gfx,
            L"Assets/background.png"
        );

    float buttonWidth = 800.0f;
    float buttonHeight = 300.0f;
    float gap = 20.0f;

    float centerX = (VIRTUAL_WIDTH - buttonWidth) / 2;
    float totalHeight = 3 * buttonHeight + 2 * gap;
    float startY = (VIRTUAL_HEIGHT - totalHeight) / 2;

    m_playButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight);
    m_playButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Start_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Start_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Start_BTN.png")
    );

    m_volumeButton = std::make_unique<Button>(centerX, startY + buttonHeight + gap, buttonWidth, buttonHeight);
    m_volumeButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Volume_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Volume_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Volume_BTN.png")
    );

    m_exitButton = std::make_unique<Button>(centerX, startY + 2 * (buttonHeight + gap), buttonWidth, buttonHeight);
    m_exitButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Exit_BTN.png")
    );
}

void MenuScene::Update(float dt, InputManager& input, SceneManager& manager) {
    // Lấy dữ liệu chuột thật từ InputManager
    float mouseX = (float)input.GetMouseX();
    float mouseY = (float)input.GetMouseY();
    bool isClicked = input.IsLeftMouseClicked();

    // Cập nhật trạng thái của các nút
    m_playButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    m_volumeButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    m_exitButton->Update(mouseX, mouseY, isClicked, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    if (m_playButton->IsClicked()) {
        manager.ChangeScene(std::make_unique<LevelSelectScene>(m_gfx));
    }

    static bool isMuted = false;

    if (m_volumeButton->IsClicked()) {
        isMuted = !isMuted;

        if (isMuted) {
            AudioManager::GetInstance().StopMusic();
        }
        else {
            AudioManager::GetInstance().PlayMusic(AudioIds::MenuMusic);
        }
    }

	if (m_exitButton->IsClicked()) {
        PostQuitMessage(0);
    }
    m_backgroundOffsetY += m_backgroundScrollSpeed * dt;

    if (m_backgroundOffsetY >= VIRTUAL_HEIGHT) {
        m_backgroundOffsetY = 0.0f;
    }
}

void MenuScene::Render(Graphics& gfx) {
    // Tô màu nền
    gfx.ClearBuffer(0.0f, 0.0f, 0.0f);

    // Lấy cọ vẽ ra
    auto spriteBatch = gfx.GetSpriteBatch();

    // Bắt đầu vẽ 2D
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

    // Vẽ nút PLAY
    m_playButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    m_volumeButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    m_exitButton->Render(spriteBatch, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // Kết thúc vẽ 2D
    spriteBatch->End();
}
