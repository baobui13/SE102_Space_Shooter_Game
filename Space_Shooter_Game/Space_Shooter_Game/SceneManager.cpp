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
    // Nếu danh sách không rỗng thì gỡ màn hình trên cùng đi
    if (!m_scenes.empty()) {
        m_scenes.pop_back();
    }
}

void SceneManager::Update(float dt, InputManager& input) {
	// Cập nhật cho màn hình trên cùng, pause màn hình dưới nếu có
    if (!m_scenes.empty()) {
        m_scenes.back()->Update(dt, input, *this);
    }

    // Nếu có Scene mới đang chờ được nạp, thay thế an toàn tại cuối vòng lặp
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