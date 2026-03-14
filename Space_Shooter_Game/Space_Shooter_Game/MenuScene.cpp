#include "MenuScene.h"
#include "Graphics.h"

MenuScene::MenuScene(Graphics& gfx) {
    m_playButton = std::make_unique<Button>(200.0f, 400.0f, 400.0f, 100.0f);
    // Nạp 3 trạng thái ảnh cho nút PLAY (nếu bạn chỉ có 1 ảnh, hãy chép tên nó cho cả 3 tham số)
    m_playButton->SetTextures(
        gfx.LoadTexture(L"Assets/Start_BTN.png"),
        gfx.LoadTexture(L"Assets/Start_BTN.png"),
        gfx.LoadTexture(L"Assets/Start_BTN.png")
    );

    m_exitButton = std::make_unique<Button>(200.0f, 600.0f, 400.0f, 100.0f);
    m_exitButton->SetTextures(
        gfx.LoadTexture(L"Assets/Exit_BTN.png"),
        gfx.LoadTexture(L"Assets/Exit_BTN.png"),
        gfx.LoadTexture(L"Assets/Exit_BTN.png")
    );
}

void MenuScene::Update(float dt) {
    // Tương lai: Lấy tọa độ chuột và trạng thái click từ InputManager
    float mouseX = 0; // Tạm thời để 0
    float mouseY = 0;
    bool isClicked = false;

    // Cập nhật trạng thái của các nút
    m_playButton->Update(mouseX, mouseY, isClicked);
    m_exitButton->Update(mouseX, mouseY, isClicked);

    if (m_playButton->IsClicked()) {
        // Tương lai: Gọi SceneManager chuyển sang GameplayScene
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
    spriteBatch->Begin();

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