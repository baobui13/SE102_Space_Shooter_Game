#pragma once
#include "GameObject.h"

class ExpOrb : public GameObject {
private:
    int m_expAmount; // Lượng kinh nghiệm mà viên ngọc này chứa
    float m_magnetSpeed; // Tốc độ bay về phía Player khi bị hút
public:
    // Khởi tạo ngọc EXP tại tọa độ (x, y) với lượng EXP truyền vào
    ExpOrb(Graphics& gfx, float x, float y, int expValue);

    // Bắt buộc phải ghi đè hàm Update của GameObject
    void Update(float dt, ::GameContext& ctx) override;

    // Lấy lượng kinh nghiệm khi Player nhặt được
    int GetExpAmount() const { return m_expAmount; }
};