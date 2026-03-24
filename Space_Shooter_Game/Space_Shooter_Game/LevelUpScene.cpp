#include "LevelUpScene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "GameConfig.h"

// Kích thước vẽ lên màn hình
const float CARD_WIDTH = 450.0f;
const float CARD_HEIGHT = 650.0f;
const float CARD_SPACING = 50.0f;

LevelUpScene::LevelUpScene(Graphics& gfx, Player& player)
    : m_gfx(gfx), m_player(player)
{
    // Tạm thời dùng Font Arial mặc định
    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");
    m_descFont = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");

    // Nạp Texture nền thẻ trắng
    AssetManager& assets = AssetManager::GetInstance();
    m_texWhiteCardBG = assets.GetTexture(gfx, L"Assets/sheen__0011_Background.png");

    // Nạp texture trắng 1x1 để vẽ nền mờ
    m_texWhitePoint = assets.GetTexture(gfx, L"Assets/WhitePoint.png");

    GenerateChoices();
}

void LevelUpScene::GenerateChoices() {
    m_currentChoices = UpgradeManager::GetInstance().GetRandomUpgrades(3);
}

void LevelUpScene::Update(float dt, InputManager& input, SceneManager& manager) {
    int choice = -1;

    // 1. Lấy tọa độ chuột từ InputManager
    m_mouseX = (float)input.GetMouseX();
    m_mouseY = (float)input.GetMouseY();

    // 2. Tính toán lại vị trí các thẻ (giống hệt trong Render)
    float totalWidth = (3 * CARD_WIDTH) + (2 * CARD_SPACING);
    float startX = (VIRTUAL_WIDTH - totalWidth) / 2.0f;
    float startY = (VIRTUAL_HEIGHT - CARD_HEIGHT) / 2.0f + 50.0f;

    // 3. Xử lý khi click chuột trái (VK_LBUTTON là mã phím chuột trái mặc định của Windows)
    if (input.IsLeftMouseClicked()) {
        for (size_t i = 0; i < m_currentChoices.size(); ++i) {
            float cardX = startX + i * (CARD_WIDTH + CARD_SPACING);
            float cardY = startY;

            if (m_mouseX >= cardX && m_mouseX <= cardX + CARD_WIDTH &&
                m_mouseY >= cardY && m_mouseY <= cardY + CARD_HEIGHT) {
                choice = (int)i; // Đã tìm thấy thẻ được click!
                break;
            }
        }
    }

    // 4. Vẫn giữ lại tính năng chọn bằng phím cứng (Dành cho ai thích dùng phím)
    if (input.IsKeyPressed('1')) choice = 0;
    if (input.IsKeyPressed('2')) choice = 1;
    if (input.IsKeyPressed('3')) choice = 2;

    // 5. Áp dụng kỹ năng nếu đã chọn
    if (choice >= 0 && choice < m_currentChoices.size()) {
        m_currentChoices[choice].applyEffect(m_player);
        m_player.UseUpgradePoint();

        if (m_player.GetUpgradePoints() > 0) {
            GenerateChoices(); // Rút thẻ mới nếu vẫn còn lượt
        }
        else {
            manager.PopScene(); // Thoát màn hình
        }
    }
}

void LevelUpScene::Render(Graphics& gfx) {
    auto spriteBatch = gfx.GetSpriteBatch();

    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, gfx.GetScaleMatrix());

    // Nền mờ
    if (m_texWhitePoint) {
        RECT bgRect = { 0, 0, (LONG)VIRTUAL_WIDTH, (LONG)VIRTUAL_HEIGHT };
        spriteBatch->Draw(m_texWhitePoint.Get(), bgRect, DirectX::Colors::Black * 0.75f);
    }

    std::wstring title = L"LEVEL UP!";
    m_font->DrawString(spriteBatch, title.c_str(), DirectX::XMFLOAT2(VIRTUAL_WIDTH / 2.0f - 100.0f, 80.0f), DirectX::Colors::Gold);

    // Tính toán vị trí căn giữa
    float totalWidth = (3 * CARD_WIDTH) + (2 * CARD_SPACING);
    float startX = (VIRTUAL_WIDTH - totalWidth) / 2.0f;
    float startY = (VIRTUAL_HEIGHT - CARD_HEIGHT) / 2.0f + 50.0f;

    for (size_t i = 0; i < m_currentChoices.size(); ++i) {
        float cardX = startX + i * (CARD_WIDTH + CARD_SPACING);
        float cardY = startY;

        RECT destCard = { (LONG)cardX, (LONG)cardY, (LONG)(cardX + CARD_WIDTH), (LONG)(cardY + CARD_HEIGHT) };

        // --- TÍNH NĂNG HOVER: KIỂM TRA XEM CHUỘT CÓ ĐANG CHỈ VÀO THẺ NÀY KHÔNG ---
        bool isHovered = (m_mouseX >= cardX && m_mouseX <= cardX + CARD_WIDTH &&
            m_mouseY >= cardY && m_mouseY <= cardY + CARD_HEIGHT);

        // Nếu di chuột vào, thẻ sẽ sáng lên/đổi màu chút xíu. Nếu không, giữ nguyên màu Trắng.
        DirectX::XMVECTOR cardTint = isHovered ? DirectX::Colors::LightGray : DirectX::Colors::White;

        if (m_texWhiteCardBG) {
            spriteBatch->Draw(m_texWhiteCardBG.Get(), destCard, nullptr, cardTint);
        }

        std::wstring typeText(m_currentChoices[i].type.begin(), m_currentChoices[i].type.end());
        m_descFont->DrawString(spriteBatch, typeText.c_str(), DirectX::XMFLOAT2(cardX + 40.0f, cardY + 40.0f), DirectX::Colors::Black, 0.0f, DirectX::XMFLOAT2(0, 0), 1.2f);

        std::wstring nameText(m_currentChoices[i].name.begin(), m_currentChoices[i].name.end());
        m_font->DrawString(spriteBatch, nameText.c_str(), DirectX::XMFLOAT2(cardX + 40.0f, cardY + 110.0f), DirectX::Colors::Black);

        std::wstring descText(m_currentChoices[i].description.begin(), m_currentChoices[i].description.end());
        m_descFont->DrawString(spriteBatch, descText.c_str(), DirectX::XMFLOAT2(cardX + 40.0f, cardY + 200.0f), DirectX::Colors::DarkSlateGray);

        // Đổi text SELECT thành màu nổi bật hơn nếu đang Hover
        std::wstring hint = L"[" + std::to_wstring(i + 1) + L"] SELECT";
        DirectX::XMVECTOR hintColor = isHovered ? DirectX::Colors::Red : DirectX::Colors::Crimson;

        m_descFont->DrawString(spriteBatch, hint.c_str(), DirectX::XMFLOAT2(cardX + CARD_WIDTH / 2.0f - 60.0f, cardY + CARD_HEIGHT - 60.0f), hintColor, 0.0f, DirectX::XMFLOAT2(0, 0), 1.3f);
    }

    spriteBatch->End();
}