#include "GameplayScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include <string>
#include <DirectXColors.h>

GameplayScene::GameplayScene(Graphics& gfx) : m_gfx(gfx) {
    // Đặt phi thuyền ở gần cạnh dưới màn hình (400, 800)
    m_player = std::make_unique<Player>(gfx, 400.0f - 32.0f, 800.0f - 32.0f);

    // KHỞI TẠO FONT CHỮ
    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");

    // [THÊM MỚI]: Sinh ngọc ở các vị trí cố định để test
    // 4 viên ở 4 góc màn hình (Mỗi viên 10 EXP)
    m_expOrbs.push_back(std::make_unique<ExpOrb>(gfx, 100.0f, 100.0f, 10));
    m_expOrbs.push_back(std::make_unique<ExpOrb>(gfx, 700.0f, 100.0f, 10));
    m_expOrbs.push_back(std::make_unique<ExpOrb>(gfx, 100.0f, 500.0f, 10));
    m_expOrbs.push_back(std::make_unique<ExpOrb>(gfx, 700.0f, 500.0f, 10));

    // 1 viên to đùng ở giữa cạnh trên (50 EXP)
    m_expOrbs.push_back(std::make_unique<ExpOrb>(gfx, 400.0f, 100.0f, 50));
}

void GameplayScene::Update(float dt, InputManager& input, SceneManager& manager) {
    // Truyền thêm danh sách đạn và m_gfx vào cho Player
    m_player->Update(dt, input, m_bullets, m_gfx);

    // Cập nhật vị trí cho tất cả các viên đạn
    for (auto& bullet : m_bullets) {
        bullet->Update(dt);
    }

    // Tính tọa độ tâm của Player
    float playerCenterX = m_player->GetX() + m_player->GetWidth() / 2.0f;
    float playerCenterY = m_player->GetY() + m_player->GetHeight() / 2.0f;

    // Cập nhật và kiểm tra va chạm Ngọc EXP
    for (auto& orb : m_expOrbs) {

        orb->Update(dt, playerCenterX, playerCenterY, m_player->GetMagnetRange());

        float dx = playerCenterX - (orb->GetX() + orb->GetWidth() / 2.0f);
        float dy = playerCenterY - (orb->GetY() + orb->GetHeight() / 2.0f);
        float distToEat = std::sqrt(dx * dx + dy * dy);

        // Khoảng cách nhặt (Ăn ngọc) vẫn để cứng 30.0f hoặc bạn có thể chuyển nó vào Player sau nếu muốn
        if (orb->IsActive() && distToEat < 30.0f) {
            m_player->GainExp(orb->GetExpAmount());
            orb->Destroy();
        }
    }

    // DỌN RÁC: Xóa các viên đạn đã bị đánh dấu m_isActive = false (bay khỏi màn hình)
    // Cú pháp này gọi là "Erase-Remove Idiom", chuẩn mực của C++ để xóa phần tử trong vector
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const std::unique_ptr<Bullet>& b) { return !b->IsActive(); }),
        m_bullets.end()
    );


    // Cập nhật tất cả ngọc EXP
    for (auto& orb : m_expOrbs) {
        orb->Update(dt);

        // KIỂM TRA VA CHẠM VỚI PLAYER
        if (orb->IsActive() && m_player->CheckCollision(*orb)) {
            // Cộng EXP cho Player (Giả sử Player của bạn có hàm GainExp)
            m_player->GainExp(orb->GetExpAmount());

            // Hủy viên ngọc sau khi bị ăn
            orb->Destroy();
        }
    }

    // Xóa các viên ngọc đã bị ăn hoặc trôi khỏi màn hình (m_isActive == false)
    m_expOrbs.erase(std::remove_if(m_expOrbs.begin(), m_expOrbs.end(),
        [](const std::unique_ptr<ExpOrb>& orb) { return !orb->IsActive(); }),
        m_expOrbs.end());

    // KHI BẤM NÚT ESCAPE -> QUAY LẠI MENU
    if (input.IsKeyDown(VK_ESCAPE)) {
        manager.ChangeScene(std::make_unique<MenuScene>(m_gfx));
    }

    // TEST: Bấm phím E để hack 50 EXP
    if (input.IsKeyPressed('E')) {
        m_player->GainExp(50);
        OutputDebugString(L"[DEBUG] Cong 50 EXP!\n");
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

    for (auto& orb : m_expOrbs) {
        orb->Render(gfx);
    }

    m_player->Render(gfx);

    std::wstring uiText =
        L"LEVEL: " + std::to_wstring(m_player->GetLevel()) + L"\n" +
        L"EXP: " + std::to_wstring(m_player->GetCurrentExp()) + L" / " + std::to_wstring(m_player->GetExpToNextLevel()) + L"\n\n" +
        L"SAT THUONG: " + std::to_wstring(m_player->GetAttackDamage()) + L"\n" +
        L"TOC DO BAN: " + std::to_wstring((int)m_player->GetAttackSpeed()) + L" /s\n" +
        L"TAM BAN: " + std::to_wstring((int)m_player->GetAttackRange());

    // Vẽ chữ lên màn hình
    m_font->DrawString(spriteBatch, uiText.c_str(), DirectX::XMFLOAT2(10.0f, 10.0f), DirectX::Colors::Yellow);

    // 4. Kết thúc vẽ
    spriteBatch->End();
}