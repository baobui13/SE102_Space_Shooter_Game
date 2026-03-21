#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    int m_damage; // Lượng sát thương viên đạn mang theo

public:
    // Cần truyền vào: Điểm bắt đầu (startX, Y), Điểm bắn tới (targetX, Y), Tốc độ bay, Sát thương
    Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage);

    // Override lại Update để xử lý việc bay ra khỏi màn hình
    void Update(float dt) override;

    // Hàm để quái vật (Enemy) gọi khi bị trúng đạn
    int GetDamage() const { return m_damage; }
};