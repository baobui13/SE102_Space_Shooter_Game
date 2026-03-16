#pragma once
#include "Graphics.h"
#include <wrl/client.h>
#include <d3d11.h>

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
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

public:
    GameObject(float x, float y, float width, float height);

    // Virtual destructor rất quan trọng trong đa hình để tránh rò rỉ bộ nhớ
    virtual ~GameObject() = default;

    // Hàm thuần ảo (Pure virtual): Bắt buộc mọi class con BẮT BUỘC phải tự viết logic Update riêng
    virtual void Update(float dt) = 0;

    // Hàm ảo (Virtual): Cung cấp sẵn logic vẽ mặc định, class con CÓ THỂ dùng hoặc ghi đè lại
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

    void SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }
};