#include "SceneManager.h"
#include "InputManager.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene) {
    // Lưu màn hình mới vào hàng chờ, sẽ nạp vào cuối vòng lặp Update
    m_nextScene = std::move(newScene);
}

void SceneManager::PushScene(std::unique_ptr<Scene> scene) {
    // Đẩy màn hình mới lên nóc danh sách
    m_scenes.push_back(std::move(scene));
}

void SceneManager::PopScene() {
    // Request a pop instead of deleting immediately
    m_requestPop = true;
}

void SceneManager::Update(float dt, InputManager& input) {
    if (!m_scenes.empty()) {
        m_scenes.back()->Update(dt, input, *this);
    }

    // Process pending pop after Update returns
    if (m_requestPop) {
        if (!m_scenes.empty()) m_scenes.pop_back();
        m_requestPop = false;
    }

    if (m_nextScene) {
        m_scenes.clear();
        m_scenes.push_back(std::move(m_nextScene));
    }
}

void SceneManager::Render(Graphics& gfx) {
    // Vẽ từ dưới cùng lên trên cùng (Scene sau sẽ tự động đè lên Scene trước)
    for (auto& scene : m_scenes) {
        scene->Render(gfx);
    }
}