#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    int m_damage; // Lượng sát thương viên đạn mang theo
    float m_maxDistance;      // Khoảng cách tối đa đạn có thể bay
    float m_distanceTraveled; // Khoảng cách đạn ĐÃ bay được
public:
    // Constructor mặc định cho pooling
    Bullet(Graphics& gfx);
    Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance);

    // Dùng để tái sử dụng viên đạn từ Pool
    void ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance);

    // Override lại Update để xử lý việc bay ra khỏi màn hình
    void Update(float dt, ::GameContext& ctx) override;

    // Hàm để quái vật (Enemy) gọi khi bị trúng đạn
    int GetDamage() const { return m_damage; }
};