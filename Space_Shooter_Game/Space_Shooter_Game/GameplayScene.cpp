#include "GameplayScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "GameConfig.h"
#include <string>
#include <DirectXColors.h>
#include "LevelUpScene.h"

GameplayScene::GameplayScene(Graphics& gfx) 
    : m_gfx(gfx), m_bulletPool(gfx) {
    // Đặt phi thuyền ở giữa cạnh dưới màn hình
    m_player = std::make_unique<Player>(gfx, VIRTUAL_WIDTH / 2.0f - 32.0f, VIRTUAL_HEIGHT - 200.0f);

    // KHỞI TẠO FONT CHỮ
    m_font = std::make_unique<DirectX::SpriteFont>(gfx.GetDevice().Get(), L"Assets/Arial.spritefont");

    // Sinh ngọc ở các vị trí cố định thông qua EntityManager
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(gfx, VIRTUAL_WIDTH * 0.125f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(gfx, VIRTUAL_WIDTH * 0.875f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(gfx, VIRTUAL_WIDTH * 0.125f, VIRTUAL_HEIGHT * 0.5f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(gfx, VIRTUAL_WIDTH * 0.875f, VIRTUAL_HEIGHT * 0.5f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.1f, 50));
}

void GameplayScene::Update(float dt, InputManager& input, SceneManager& manager) {
    GameContext ctx(m_gfx, input, AssetManager::GetInstance(), m_bulletPool, m_entityManager, *m_player, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // 1. Cập nhật Player
    m_player->Update(dt, ctx);

    // 2. Cập nhật các Entity (Ngọc, Quái...)
    m_entityManager.UpdateAll(dt, ctx);

    // 3. Cập nhật Đạn từ Pool
    m_bulletPool.Update(dt, ctx);

    // 4. Quay lại Menu
    if (input.IsKeyDown(VK_ESCAPE)) {
        manager.ChangeScene(std::make_unique<MenuScene>(m_gfx));
    }

    if (input.IsKeyPressed('E')) {
        m_player->GainExp(50);
    }

    // Mở hòm nâng cấp nếu có lượt và bấm phím U
    if (m_player->GetUpgradePoints() > 0 && input.IsKeyPressed('U')) {
        manager.PushScene(std::make_unique<LevelUpScene>(m_gfx, *m_player));
    }
}

void GameplayScene::Render(Graphics& gfx) {
    gfx.ClearBuffer(0.02f, 0.02f, 0.1f);
    auto spriteBatch = gfx.GetSpriteBatch();

    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, gfx.GetScaleMatrix());

    // Vẽ tất cả bằng EntityManager và BulletPool
    m_entityManager.RenderAll(gfx);
    m_bulletPool.Render(gfx);
    m_player->Render(gfx);

    std::wstring uiText =
        L"LEVEL: " + std::to_wstring(m_player->GetLevel()) + L"\n" +
        L"EXP: " + std::to_wstring(m_player->GetCurrentExp()) + L" / " + std::to_wstring(m_player->GetExpToNextLevel()) + L"\n" +
        L"DAMAGE: " + std::to_wstring(m_player->GetAttackDamage()) + L"\n" +
        L"FPS: " + std::to_wstring((int)(1.0f / 0.016f)); // Giả định 60fps để test

    m_font->DrawString(spriteBatch, uiText.c_str(), DirectX::XMFLOAT2(10.0f, 10.0f), DirectX::Colors::Yellow);

    // Cảnh báo có lượt nâng cấp
    if (m_player->GetUpgradePoints() > 0) {
        std::wstring notify = L"Ban co " + std::to_wstring(m_player->GetUpgradePoints()) + L" luot nang cap! Bam 'U' de dung.";
        m_font->DrawString(spriteBatch, notify.c_str(), DirectX::XMFLOAT2(10.0f, 200.0f), DirectX::Colors::Cyan);
    }

    spriteBatch->End();
}
