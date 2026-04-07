#include "ProgressBar.h"

namespace {
constexpr int ORIGIN_TOP_LEFT = 1;
constexpr int ORIGIN_BOTTOM_LEFT = 2;
constexpr int ORIGIN_BOTTOM_RIGHT = 3;
constexpr int ORIGIN_TOP_RIGHT = 4;
}

ProgressBar::ProgressBar(float x, float y, float width, float height, int origin)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_origin(ORIGIN_TOP_LEFT) {
    SetOrigin(origin);
}

void ProgressBar::SetTextures(
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> background,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> progress,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> border) {
    m_texBackground = background;
    m_texProgress = progress;
    m_texBorder = border;
}

void ProgressBar::SetOrigin(int origin) {
    switch (origin) {
    case ORIGIN_TOP_LEFT:
    case ORIGIN_BOTTOM_LEFT:
    case ORIGIN_BOTTOM_RIGHT:
    case ORIGIN_TOP_RIGHT:
        m_origin = origin;
        break;
    default:
        m_origin = ORIGIN_TOP_LEFT;
        break;
    }
}

RECT ProgressBar::BuildDestinationRect(float screenWidth, float screenHeight) const {
    float left = m_x;
    float top = m_y;

    switch (m_origin) {
    case ORIGIN_BOTTOM_LEFT:
        top = screenHeight - m_y - m_height;
        break;
    case ORIGIN_BOTTOM_RIGHT:
        left = screenWidth - m_x - m_width;
        top = screenHeight - m_y - m_height;
        break;
    case ORIGIN_TOP_RIGHT:
        left = screenWidth - m_x - m_width;
        break;
    default:
        break;
    }

    RECT destRect = {
        static_cast<LONG>(left),
        static_cast<LONG>(top),
        static_cast<LONG>(left + m_width),
        static_cast<LONG>(top + m_height)
    };
    return destRect;
}

void ProgressBar::Render(DirectX::SpriteBatch* spriteBatch, float percentage, float screenWidth, float screenHeight) {
    if (!spriteBatch) {
        return;
    }

    if (percentage < 0.0f) percentage = 0.0f;
    if (percentage > 1.0f) percentage = 1.0f;

    const RECT destRect = BuildDestinationRect(screenWidth, screenHeight);

    if (m_texBackground) {
        spriteBatch->Draw(m_texBackground.Get(), destRect);
    }

    if (m_texProgress) {
        RECT progressRect = {
            destRect.left,
            destRect.top,
            static_cast<LONG>(destRect.left + (m_width * percentage)),
            destRect.bottom
        };
        spriteBatch->Draw(m_texProgress.Get(), progressRect);
    }

    if (m_texBorder) {
        spriteBatch->Draw(m_texBorder.Get(), destRect);
    }
}
