#pragma once
#include "Scene.h"
#include <memory> // Thư viện dùng std::unique_ptr để quản lý bộ nhớ tự động

class SceneManager {
public:
    SceneManager() = default;
    ~SceneManager() = default;

    // Hàm dùng để chuyển đổi màn hình
    void ChangeScene(std::unique_ptr<Scene> newScene);

    // Gọi Update và Render của màn hình hiện tại
    void Update(float dt, InputManager& input);
    void Render(Graphics& gfx);

private:
    std::unique_ptr<Scene> m_currentScene; // Con trỏ lưu trữ màn hình đang hoạt động
};