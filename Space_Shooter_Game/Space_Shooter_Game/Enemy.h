#pragma once
#include "GameObject.h"
#include "EnemyBullet.h"  // Bao gồm EnemyBullet.h để sử dụng EnemyBullet trong lớp Enemy
#include <vector>

class Enemy : public GameObject {
private:
    int m_health;  // Máu của kẻ địch
    float m_speed; // Tốc độ di chuyển của kẻ địch
    bool m_isAlive; // Kiểm tra trạng thái sống của kẻ địch
	DirectX::XMFLOAT3 m_position; // Vị trí hiện tại của kẻ địch
    std::vector<EnemyBullet> m_bullets;  // Danh sách đạn của kẻ địch
    float m_shootCooldown;  // Thời gian cooldown giữa các lần bắn
    float m_timeSinceLastShot;  // Thời gian kể từ lần bắn trước

public:
    // Constructor cho Enemy
    Enemy(Graphics& gfx, float startX, float startY, int health, float speed);

    void Render(Graphics& gfx) override;

    // Cập nhật trạng thái của Enemy (di chuyển, bắn đạn, kiểm tra va chạm)
    void Update(float dt, ::GameContext& ctx) override;

    // Kiểm tra va chạm giữa kẻ địch và các đối tượng khác
    bool CheckCollision(const GameObject& target);

    // Hàm để bắn đạn
    void Shoot(float targetX, float targetY, float speed, int damage, float maxDistance);

    // Hàm kiểm tra hp và trạng thái sống của kẻ địch
    bool IsAlive() const { return m_isAlive; }

    // Lấy hp của kẻ địch
    int GetHealth() const { return m_health; }

    // Giảm hp khi bị trúng đạn
    void TakeDamage(int damage);
};