#include "GameObject.h"

GameObject::GameObject(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_vx(0.0f), m_vy(0.0f), m_isActive(true) {
}

void GameObject::Render(Graphics& gfx) {
    // Nếu object đã chết hoặc không có ảnh thì không cần vẽ
    if (!m_isActive || !m_texture) return;

    auto spriteBatch = gfx.GetSpriteBatch();

    // Tạo khung chữ nhật để vẽ dựa trên tọa độ và kích thước thực thể
    RECT destRect = {
        (LONG)m_x,
        (LONG)m_y,
        (LONG)(m_x + m_width),
        (LONG)(m_y + m_height)
    };

    spriteBatch->Draw(m_texture.Get(), destRect);
}

// Xử lý va chạm chuẩn cho game 2D (Axis-Aligned Bounding Box - AABB)
bool GameObject::CheckCollision(const GameObject& other) const {
    // Nếu 1 trong 2 đã chết thì không tính va chạm
    if (!m_isActive || !other.IsActive()) return false;

    // Kiểm tra xem 2 hình chữ nhật có đè lên nhau không
    return (m_x < other.m_x + other.m_width &&
        m_x + m_width > other.m_x &&
        m_y < other.m_y + other.m_height &&
        m_y + m_height > other.m_y);
}