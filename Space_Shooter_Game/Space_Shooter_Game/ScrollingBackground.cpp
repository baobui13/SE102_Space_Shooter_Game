#include "ScrollingBackground.h"
#include "AssetManager.h"
#include "GameConfig.h"
#include <DirectXColors.h>

ScrollingBackground::ScrollingBackground(
    Graphics& gfx,
    const std::wstring& texturePath,
    float scrollSpeed
)
    : m_scrollSpeed(scrollSpeed)
{
    m_texture = AssetManager::GetInstance().GetTexture(gfx, texturePath);
}

void ScrollingBackground::Update(float dt) {
    m_offsetY += m_scrollSpeed * dt;

    if (m_offsetY >= VIRTUAL_HEIGHT) {
        m_offsetY = 0.0f;
    }
}

void ScrollingBackground::Render(Graphics& gfx) {
    auto spriteBatch = gfx.GetSpriteBatch();

    LONG y = (LONG)m_offsetY;

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

    spriteBatch->Draw(m_texture.Get(), rect1, nullptr, DirectX::Colors::White);
    spriteBatch->Draw(m_texture.Get(), rect2, nullptr, DirectX::Colors::White);
}