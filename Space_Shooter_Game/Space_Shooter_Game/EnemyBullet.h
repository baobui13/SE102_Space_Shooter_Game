#pragma once
#include "GameObject.h"

class EnemyBullet : public GameObject {
private:
    int m_damage;              // Lượng sát thương viên đạn mang theo
    float m_maxDistance;       // Khoảng cách tối đa đạn có thể bay
	float m_distanceTraveled; // Khoảng cách đạn ĐÃ bay được
	DirectX::XMFLOAT3 m_position; // Vị trí hiện tại của đạn
    DirectX::XMFLOAT3 m_direction;  // Hướng di chuyển của đạn
	float m_speed;             // Tốc độ di chuyển của đạn
    bool m_isActive;           // Kiểm tra trạng thái của đạn (đang hoạt động hay không)

public:
    // Constructor cho EnemyBullet
    EnemyBullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance);

    // Cập nhật trạng thái của EnemyBullet (vị trí, khoảng cách bay)
    void Update(float dt, ::GameContext& ctx);

    // Hàm để vẽ đạn
    void Render(Graphics& gfx) override;

    // Kiểm tra va chạm với các đối tượng (ví dụ: Player hoặc các đối tượng khác)
    bool CheckCollision(const GameObject& target);

    // Lấy giá trị sát thương của đạn
    int GetDamage() const { return m_damage; }

    // Kiểm tra xem đạn có còn hoạt động không
    bool IsActive() const { return m_isActive; }

    // Đặt lại trạng thái của đạn (khi không còn sử dụng)
    void Deactivate() { m_isActive = false; }
};