#include "LevelUpScene.h"
#include "SceneManager.h"
#include "UpgradeManager.h"
#include "InputManager.h"

// Định nghĩa các hằng số kích thước (Dựa trên màn hình 1920x1080)
const float CARD_WIDTH = 450.0f;  // Chiều rộng 1 lá bài
const float CARD_HEIGHT = 700.0f; // Chiều cao 1 lá bài
const float CARD_SPACING = 60.0f; // Khoảng cách giữa các bài

LevelUpScene::LevelUpScene(Graphics& gfx, Player& player)
    : m_gfx(gfx), m_player(player)
{
    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");

    // Bốc 3 thẻ khi vừa mở Scene
    GenerateChoices();
}

void LevelUpScene::GenerateChoices() {
    // Xin UpgradeManager 3 thẻ ngẫu nhiên
    m_currentChoices = UpgradeManager::GetInstance().GetRandomUpgrades(3);
}

void LevelUpScene::Update(float dt, InputManager& input, SceneManager& manager) {
    int choice = -1;
    if (input.IsKeyPressed('1')) choice = 0;
    if (input.IsKeyPressed('2')) choice = 1;
    if (input.IsKeyPressed('3')) choice = 2;

    if (choice >= 0 && choice < m_currentChoices.size()) {
        // Áp dụng hiệu ứng
        m_currentChoices[choice].applyEffect(m_player);
        m_player.UseUpgradePoint(); // Trừ 1 lượt

        // Kiểm tra xem còn lượt không?
        if (m_player.GetUpgradePoints() > 0) {
            GenerateChoices(); // Bốc 3 thẻ mới cho lượt tiếp theo
        }
        else {
            manager.PopScene(); // Hết lượt -> Gỡ Scene này ra, GameplayScene ở dưới sẽ chạy tiếp!
        }
    }
}

void LevelUpScene::Render(Graphics& gfx) {
    auto spriteBatch = gfx.GetSpriteBatch();
    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied());

    // Vẽ tiêu đề
    std::wstring title = L"--- CHON NANG CAP --- (Con " + std::to_wstring(m_player.GetUpgradePoints()) + L" luot)";
    m_font->DrawString(spriteBatch, title.c_str(), DirectX::XMFLOAT2(250.0f, 200.0f), DirectX::Colors::Yellow);

    // Vẽ 3 lựa chọn
    for (size_t i = 0; i < m_currentChoices.size(); ++i) {
        // Chuyển string sang wstring để vẽ
        std::wstring name(m_currentChoices[i].name.begin(), m_currentChoices[i].name.end());
        std::wstring text = L"Phim " + std::to_wstring(i + 1) + L": " + name;
        m_font->DrawString(spriteBatch, text.c_str(), DirectX::XMFLOAT2(250.0f, 300.0f + i * 50.0f), DirectX::Colors::White);
    }

    spriteBatch->End();
}