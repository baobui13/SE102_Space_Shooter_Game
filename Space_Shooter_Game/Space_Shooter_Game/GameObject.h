#pragma once
#include "Graphics.h"
#include <wrl/client.h>
#include <d3d11.h>
#include "SpriteAnimation.h"

struct GameContext; // Forward declaration toàn cục

class GameObject {
protected:
    // Dùng protected thay vì private để các class con (Player, Enemy) có thể trực tiếp sửa đổi

    // Tọa độ và kích thước
    float m_x, m_y;
    float m_width, m_height;

    // Vận tốc di chuyển (Px/giây)
    float m_vx, m_vy;

    // Trạng thái tồn tại (Dùng để báo cho GameplayScene biết để xóa object này đi khi nó chết)
    bool m_isActive;

    // Hình ảnh hiển thị (Texture)
    SpriteAnimation m_anim;

public:
    GameObject(float x, float y, float width, float height);

    // Virtual destructor rất quan trọng trong đa hình để tránh rò rỉ bộ nhớ
    virtual ~GameObject() = default;


    // --- CÁC HÀM ẢO ---
    virtual void Update(float dt, ::GameContext& ctx);
    virtual void Render(Graphics& gfx);

    // Thuật toán kiểm tra va chạm (AABB)
    bool CheckCollision(const GameObject& other) const;

    // --- CÁC HÀM GETTER / SETTER ---
    bool IsActive() const { return m_isActive; }
    void Destroy() { m_isActive = false; } // Đánh dấu là đã chết để chờ xóa

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    void SetVelocity(float vx, float vy) { m_vx = vx; m_vy = vy; }
    SpriteAnimation& GetAnim() { return m_anim; }
};