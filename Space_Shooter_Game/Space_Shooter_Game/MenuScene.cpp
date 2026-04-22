#include "AudioManager.h"
#include "MenuScene.h"
#include "GameplayScene.h"
#include "SceneManager.h"
#include "Graphics.h"
#include "InputManager.h"

MenuScene::MenuScene(Graphics& gfx) : m_gfx(gfx) {
    AudioManager::GetInstance().PlayMusic(AudioIds::MenuMusic);

    m_playButton = std::make_unique<Button>(200.0f, 400.0f, 400.0f, 100.0f);
    m_playButton->SetTextures(
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Start_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Start_BTN.png"),
        AssetManager::GetInstance().GetTexture(gfx, L"Assets/Start_BTN.png")
    );

    m_exitButton = std::make_unique<Button>(200.0f, 600.0f, 400.0f, 100.0f);
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
    m_playButton->Update(mouseX, mouseY, isClicked);
    m_exitButton->Update(mouseX, mouseY, isClicked);

    if (m_playButton->IsClicked()) {
        manager.ChangeScene(std::make_unique<GameplayScene>(m_gfx));
    }

	if (m_exitButton->IsClicked()) {
        PostQuitMessage(0);
    }
}

void MenuScene::Render(Graphics& gfx) {
    // Tô màu nền
    gfx.ClearBuffer(0.2f, 0.1f, 0.4f);

    // Lấy cọ vẽ ra
    auto spriteBatch = gfx.GetSpriteBatch();

    // Bắt đầu vẽ 2D
    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, gfx.GetScaleMatrix());

    // Vẽ nút PLAY
    if (auto tex = m_playButton->GetCurrentTexture()) {
        // Tạo một hình chữ nhật quy định vị trí và kích thước vẽ trên màn hình
        RECT destRect = {
            (LONG)m_playButton->GetX(),
            (LONG)m_playButton->GetY(),
            (LONG)(m_playButton->GetX() + m_playButton->GetWidth()),
            (LONG)(m_playButton->GetY() + m_playButton->GetHeight())
        };
        spriteBatch->Draw(tex, destRect);
    }

    // Vẽ nút EXIT
    if (auto tex = m_exitButton->GetCurrentTexture()) {
        RECT destRect = {
            (LONG)m_exitButton->GetX(),
            (LONG)m_exitButton->GetY(),
            (LONG)(m_exitButton->GetX() + m_exitButton->GetWidth()),
            (LONG)(m_exitButton->GetY() + m_exitButton->GetHeight())
        };
        spriteBatch->Draw(tex, destRect);
    }

    // Kết thúc vẽ 2D
    spriteBatch->End();
}
