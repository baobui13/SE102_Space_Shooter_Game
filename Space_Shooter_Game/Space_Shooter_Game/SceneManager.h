#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

class SceneManager {
public:
    SceneManager() = default;
    ~SceneManager() = default;

    void PushScene(std::unique_ptr<Scene> scene); // Thêm scene đè lên
    void PopScene();                              // Gỡ bỏ scene trên cùng
    void ChangeScene(std::unique_ptr<Scene> scene); // Xóa sạch danh sách và nạp scene mới

    void Update(float dt, InputManager& input);
    void Render(Graphics& gfx);

private:
    std::vector<std::unique_ptr<Scene>> m_scenes; // Danh sách các màn hình
    std::unique_ptr<Scene> m_nextScene;           // Scene đang chờ nạp
    bool m_requestPop = false;
};