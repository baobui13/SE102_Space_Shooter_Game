#pragma once
#include "AssetManager.h"

class Graphics;
class InputManager;
class SceneManager;

class Scene {
public:
    virtual ~Scene() = default;

    // Hàm cập nhật logic (di chuyển, va chạm...), dt là Delta Time (thời gian giữa 2 khung hình)
    virtual void Update(float dt, InputManager& input, SceneManager& manager) = 0;

    // Hàm vẽ đồ họa lên màn hình
    virtual void Render(Graphics& gfx) = 0;
};