#include "SceneManager.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene) {
    // std::unique_ptr sẽ tự động hủy Scene cũ (nếu có) và thay bằng Scene mới
    m_currentScene = std::move(newScene);
}

void SceneManager::Update(float dt) {
    if (m_currentScene) {
        m_currentScene->Update(dt);
    }
}

void SceneManager::Render(Graphics& gfx) {
    if (m_currentScene) {
        m_currentScene->Render(gfx);
    }
}