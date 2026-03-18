#pragma once
#include "GameObject.h"
#include "InputManager.h"

class Player : public GameObject {
private:
    float m_speed; // Tốc độ bay của phi thuyền

public:
    Player(Graphics& gfx, float startX, float startY);

    // Override hàm thuần ảo của GameObject
    void Update(float dt) override;

    // Ta viết một hàm Update riêng nhận thêm InputManager để bắt phím
    void Update(float dt, InputManager& input);

    // Player sẽ tự động dùng hàm Render() được kế thừa từ GameObject.
};