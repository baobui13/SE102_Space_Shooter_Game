#pragma once
#include "GameObject.h"
#include "InputManager.h"
#include "Bullet.h"
#include <vector>
#include <memory>

class Player : public GameObject {
private:
    // --- CHỈ SỐ CƠ BẢN ---
    float m_speed;         // Tốc độ di chuyển
    int m_hp;              // Máu hiện tại
    int m_maxHp;           // Máu tối đa

    // --- CHỈ SỐ TẤN CÔNG ---
    int m_attackDamage;    // Sát thương mỗi đòn đánh
    float m_attackSpeed;   // Số đòn đánh tung ra trong 1 giây (Attacks per second)
    float m_attackTimer;   // Bộ đếm ngược thời gian chờ đến đòn đánh tiếp theo
    float m_attackRange;

    // --- CHỈ SỐ CẤP ĐỘ ---
    int m_level;           // Cấp độ hiện tại
    int m_exp;             // Kinh nghiệm hiện tại
    int m_expToNextLevel;  // Kinh nghiệm cần để lên cấp tiếp theo

public:
    Player(Graphics& gfx, float startX, float startY);

    // Override hàm thuần ảo của GameObject
    void Update(float dt) override;

    // Ta viết một hàm Update riêng nhận thêm InputManager để bắt phím
    void Update(float dt, InputManager& input, std::vector<std::unique_ptr<Bullet>>& bullets, Graphics& gfx);

    // --- CÁC HÀM XỬ LÝ LOGIC CHỈ SỐ ---
    void TakeDamage(int damage);
    void GainExp(int amount);



    // Player sẽ tự động dùng hàm Render() được kế thừa từ GameObject.

private:
    // Hàm nội bộ, chỉ gọi tự động khi exp >= expToNextLevel
    void LevelUp();
};