#include "GameObject.h"
#include "GameContext.h"

GameObject::GameObject(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_vx(0.0f), m_vy(0.0f), m_isActive(true) {
}

void GameObject::Update(float dt, ::GameContext& ctx) {
    // 1. Logic mặc định: Tự động di chuyển dựa theo vận tốc
    // Nhờ có 2 dòng này, class Đạn (Bullet) gần như không cần viết lại hàm Update!
    m_x += m_vx * dt;
    m_y += m_vy * dt;

    // 2. Tự động cập nhật khung hình của hoạt ảnh
    m_anim.Update(dt);
}

void GameObject::Render(Graphics& gfx) {
    // Nếu object đã chết hoặc không có ảnh thì không cần vẽ
    if (!m_isActive) return;

    auto spriteBatch = gfx.GetSpriteBatch();

    // Gọi SpriteAnimation tự vẽ chính nó lên tọa độ của Object này
    m_anim.Render(gfx, m_x, m_y, m_width, m_height);
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