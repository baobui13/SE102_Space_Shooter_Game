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
    int m_currentExp;      // Kinh nghiệm hiện tại
    int m_expToNextLevel;  // Kinh nghiệm cần để lên cấp tiếp theo

    float m_magnetRange;

public:
    Player(Graphics& gfx, float startX, float startY);

    // Override hàm ảo của GameObject
    void Update(float dt, ::GameContext& ctx) override;

	// Hàm xử lý khi bị trúng đạn
    void TakeDamage(int damage);

    // Các hàm xử lý kinh nghiệm
    void GainExp(int amount);
    void LevelUp();

    // Getter nếu sau này cần vẽ lên UI
    int GetLevel() const { return m_level; }
    int GetCurrentExp() const { return m_currentExp; }
    int GetExpToNextLevel() const { return m_expToNextLevel; }

    int GetAttackDamage() const { return m_attackDamage; }
    float GetAttackSpeed() const { return m_attackSpeed; }
    float GetAttackRange() const { return m_attackRange; }

    float GetMagnetRange() const { return m_magnetRange; }

    // Player sẽ tự động dùng hàm Render() được kế thừa từ GameObject.
};