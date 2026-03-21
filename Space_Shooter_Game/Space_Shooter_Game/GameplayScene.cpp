#include "GameplayScene.h"
#include "MenuScene.h"
#include "SceneManager.h"

GameplayScene::GameplayScene(Graphics& gfx) : m_gfx(gfx) {
    // Đặt phi thuyền ở giữa màn hình (400, 300)
    m_player = std::make_unique<Player>(gfx, 400.0f - 32.0f, 300.0f - 32.0f);
}

void GameplayScene::Update(float dt, InputManager& input, SceneManager& manager) {
    // 1. Truyền thêm danh sách đạn và m_gfx vào cho Player
    m_player->Update(dt, input, m_bullets, m_gfx);

    // 2. Cập nhật vị trí cho tất cả các viên đạn
    for (auto& bullet : m_bullets) {
        bullet->Update(dt);
    }

    // 3. DỌN RÁC: Xóa các viên đạn đã bị đánh dấu m_isActive = false (bay khỏi màn hình)
    // Cú pháp này gọi là "Erase-Remove Idiom", chuẩn mực của C++ để xóa phần tử trong vector
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const std::unique_ptr<Bullet>& b) { return !b->IsActive(); }),
        m_bullets.end()
    );

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
    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied());

    // 3. Vẽ
    for (auto& bullet : m_bullets) {
        bullet->Render(gfx);
    }

    m_player->Render(gfx);

    // 4. Kết thúc vẽ
    spriteBatch->End();
}