#include "SceneManager.h"
#include "InputManager.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene) {
    // std::unique_ptr sẽ tự động hủy Scene cũ (nếu có) và thay bằng Scene mới
    m_currentScene = std::move(newScene);
}

void SceneManager::Update(float dt, InputManager& input) {
    if (m_currentScene) {
        m_currentScene->Update(dt, input);
    }
}

void SceneManager::Render(Graphics& gfx) {
    if (m_currentScene) {
        m_currentScene->Render(gfx);
    }
}