#include "GameplayScene.h"
#include "MenuScene.h"
#include "SceneManager.h"

GameplayScene::GameplayScene(Graphics& gfx) : m_gfx(gfx) {
    // Đặt phi thuyền ở giữa màn hình (400, 300)
    m_player = std::make_unique<Player>(gfx, 400.0f - 32.0f, 300.0f - 32.0f);
}

void GameplayScene::Update(float dt, InputManager& input, SceneManager& manager) {
    // Cập nhật logic di chuyển của Player
    m_player->Update(dt, input);

    // KHI BẤM NÚT ESCAPE -> QUAY LẠI MENU
    if (input.IsKeyDown(VK_ESCAPE)) {
        manager.ChangeScene(std::make_unique<MenuScene>(m_gfx));
    }
}

void GameplayScene::Render(Graphics& gfx) {
    // 1. Xóa màn hình với màu không gian sâu (xanh lam đậm)
    gfx.ClearBuffer(0.02f, 0.02f, 0.1f);

    auto spriteBatch = gfx.GetSpriteBatch();

    // 2. Bắt đầu vẽ
    spriteBatch->Begin();

    // 3. Gọi Player tự vẽ nó (Sẽ tự động gọi GameObject::Render)
    m_player->Render(gfx);

    // 4. Kết thúc vẽ
    spriteBatch->End();
}