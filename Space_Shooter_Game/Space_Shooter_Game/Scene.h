#pragma once
#include "AssetManager.h"

// Forward declaration: Báo cho C++ biết có tồn tại class Graphics để dùng làm tham số
class Graphics;
class InputManager;

class Scene {
public:
    virtual ~Scene() = default;

    // Hàm cập nhật logic (di chuyển, va chạm...), dt là Delta Time (thời gian giữa 2 khung hình)
    virtual void Update(float dt, InputManager& input) = 0;

    // Hàm vẽ đồ họa lên màn hình
    virtual void Render(Graphics& gfx) = 0;
};