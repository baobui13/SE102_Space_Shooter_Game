#include "Button.h"
#include "AudioManager.h"
#include <DirectXColors.h>

namespace {
constexpr int ORIGIN_TOP_LEFT = 1;
constexpr int ORIGIN_BOTTOM_LEFT = 2;
constexpr int ORIGIN_BOTTOM_RIGHT = 3;
constexpr int ORIGIN_TOP_RIGHT = 4;
}

Button::Button(
    float x,
    float y,
    float width,
    float height,
    int origin,
    const std::wstring& title,
    DirectX::SpriteFont* font,
    TextAlignment textAlignment,
    float textScale)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_origin(ORIGIN_TOP_LEFT)
    , m_state(State::NORMAL)
    , m_title(title)
    , m_font(font)
    , m_textAlignment(textAlignment)
    , m_textScale(textScale)
    , m_hoverSoundId(AudioIds::UiHover)
    , m_clickSoundId(AudioIds::UiClick)
    , m_wasHovered(false) {
    SetOrigin(origin);
}

void Button::SetTextures(
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texNormal,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texHover,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texClicked) {
    m_texNormal = texNormal;
    m_texHover = texHover;
    m_texClicked = texClicked;
}

void Button::SetSoundEffects(const std::string& hoverSoundId, const std::string& clickSoundId) {
    m_hoverSoundId = hoverSoundId;
    m_clickSoundId = clickSoundId;
}

void Button::SetOrigin(int origin) {
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

RECT Button::GetDestinationRect(float screenWidth, float screenHeight) const {
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

    RECT rect = {
        static_cast<LONG>(left),
        static_cast<LONG>(top),
        static_cast<LONG>(left + m_width),
        static_cast<LONG>(top + m_height)
    };
    return rect;
}

void Button::Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth, float screenHeight) {
    const RECT bounds = GetDestinationRect(screenWidth, screenHeight);
    const bool isHovered =
        mouseX >= bounds.left && mouseX <= bounds.right &&
        mouseY >= bounds.top && mouseY <= bounds.bottom;

    if (isHovered && !m_wasHovered && !m_hoverSoundId.empty()) {
        AudioManager::GetInstance().PlayUiEffect(m_hoverSoundId);
    }

    if (isHovered && isLeftClicked && !m_clickSoundId.empty()) {
        AudioManager::GetInstance().PlayUiEffect(m_clickSoundId);
    }

    if (isHovered) {
        m_state = isLeftClicked ? State::CLICKED : State::HOVER;
    } else {
        m_state = State::NORMAL;
    }

    m_wasHovered = isHovered;
}

DirectX::XMFLOAT2 Button::CalculateTextPosition(const RECT& rect, float textWidth, float textHeight, float padding) const {
    float x = static_cast<float>(rect.left) + padding;
    float y = static_cast<float>(rect.top) + padding;

    switch (m_textAlignment) {
    case TextAlignment::TOP_LEFT:
        break;
    case TextAlignment::TOP_CENTER:
        x = static_cast<float>(rect.left + rect.right) * 0.5f - (textWidth * 0.5f);
        break;
    case TextAlignment::TOP_RIGHT:
        x = static_cast<float>(rect.right) - textWidth - padding;
        break;
    case TextAlignment::MIDDLE_LEFT:
        y = static_cast<float>(rect.top + rect.bottom) * 0.5f - (textHeight * 0.5f);
        break;
    case TextAlignment::CENTER:
        x = static_cast<float>(rect.left + rect.right) * 0.5f - (textWidth * 0.5f);
        y = static_cast<float>(rect.top + rect.bottom) * 0.5f - (textHeight * 0.5f);
        break;
    case TextAlignment::MIDDLE_RIGHT:
        x = static_cast<float>(rect.right) - textWidth - padding;
        y = static_cast<float>(rect.top + rect.bottom) * 0.5f - (textHeight * 0.5f);
        break;
    case TextAlignment::BOTTOM_LEFT:
        y = static_cast<float>(rect.bottom) - textHeight - padding;
        break;
    case TextAlignment::BOTTOM_CENTER:
        x = static_cast<float>(rect.left + rect.right) * 0.5f - (textWidth * 0.5f);
        y = static_cast<float>(rect.bottom) - textHeight - padding;
        break;
    case TextAlignment::BOTTOM_RIGHT:
        x = static_cast<float>(rect.right) - textWidth - padding;
        y = static_cast<float>(rect.bottom) - textHeight - padding;
        break;
    }

    return DirectX::XMFLOAT2(x, y);
}

void Button::Render(
    DirectX::SpriteBatch* spriteBatch,
    float screenWidth,
    float screenHeight,
    DirectX::FXMVECTOR tint,
    DirectX::FXMVECTOR textColor,
    float textScaleOverride,
    float padding) const {
    if (!spriteBatch) {
        return;
    }

    const RECT rect = GetDestinationRect(screenWidth, screenHeight);
    if (auto texture = GetCurrentTexture()) {
        spriteBatch->Draw(texture, rect, tint);
    }

    if (!m_font || m_title.empty()) {
        return;
    }

    const float textScale = textScaleOverride > 0.0f ? textScaleOverride : m_textScale;
    DirectX::XMVECTOR measured = m_font->MeasureString(m_title.c_str());
    DirectX::XMFLOAT2 textSize;
    DirectX::XMStoreFloat2(&textSize, measured);
    textSize.x *= textScale;
    textSize.y *= textScale;

    const DirectX::XMFLOAT2 textPos = CalculateTextPosition(rect, textSize.x, textSize.y, padding);
    m_font->DrawString(spriteBatch, m_title.c_str(), textPos, textColor, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), textScale);
}

bool Button::IsClicked() const {
    return m_state == State::CLICKED;
}

ID3D11ShaderResourceView* Button::GetCurrentTexture() const {
    switch (m_state) {
    case State::HOVER:
        return m_texHover ? m_texHover.Get() : m_texNormal.Get();
    case State::CLICKED:
        return m_texClicked ? m_texClicked.Get() : m_texNormal.Get();
    case State::NORMAL:
    default:
        return m_texNormal.Get();
    }
}
