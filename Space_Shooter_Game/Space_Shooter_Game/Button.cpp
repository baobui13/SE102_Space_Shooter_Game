#include "Button.h"

Button::Button(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height), m_state(State::NORMAL) {
}

void Button::SetTextures(
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texNormal,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texHover,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texClicked)
{
    m_texNormal = texNormal;
    m_texHover = texHover;
    m_texClicked = texClicked;
}

void Button::Update(float mouseX, float mouseY, bool isLeftClicked) {
    // Thuật toán kiểm tra va chạm AABB
    if (mouseX >= m_x && mouseX <= m_x + m_width &&
        mouseY >= m_y && mouseY <= m_y + m_height) {

        if (isLeftClicked) {
            m_state = State::CLICKED;
        }
        else {
            m_state = State::HOVER;
        }
    }
    else {
        m_state = State::NORMAL;
    }
}

bool Button::IsClicked() const {
    return m_state == State::CLICKED;
}

// Trả về ảnh tương ứng với trạng thái để lớp Graphics đem đi vẽ
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