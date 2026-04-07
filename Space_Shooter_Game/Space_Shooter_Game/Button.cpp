#include "Button.h"

namespace {
constexpr int ORIGIN_TOP_LEFT = 1;
constexpr int ORIGIN_BOTTOM_LEFT = 2;
constexpr int ORIGIN_BOTTOM_RIGHT = 3;
constexpr int ORIGIN_TOP_RIGHT = 4;
}

Button::Button(float x, float y, float width, float height, int origin)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_origin(ORIGIN_TOP_LEFT)
    , m_state(State::NORMAL) {
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

    if (mouseX >= bounds.left && mouseX <= bounds.right &&
        mouseY >= bounds.top && mouseY <= bounds.bottom) {
        m_state = isLeftClicked ? State::CLICKED : State::HOVER;
    } else {
        m_state = State::NORMAL;
    }
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
