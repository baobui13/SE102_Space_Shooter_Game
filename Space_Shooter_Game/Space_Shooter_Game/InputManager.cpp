#include "InputManager.h"

// --- LẤY TRẠNG THÁI CHO SCENE ---

bool InputManager::IsKeyDown(unsigned int keycode) const {
    if (keycode >= 256) return false;
    return m_keys[keycode];
}

bool InputManager::IsKeyPressed(unsigned int keycode) const {
    if (keycode >= 256) return false;
    // Phím đang được nhấn ở frame hiện tại VÀ không được nhấn ở frame trước
    return m_keys[keycode] && !m_prevKeys[keycode];
}

int InputManager::GetMouseX() const { return m_mouseX; }
int InputManager::GetMouseY() const { return m_mouseY; }

bool InputManager::IsLeftMouseDown() const { return m_isLeftMouseDown; }

bool InputManager::IsLeftMouseClicked() const {
    return m_isLeftMouseDown && !m_prevLeftMouseDown;
}

// --- CẬP NHẬT Ở CUỐI FRAME ---

void InputManager::Update() {
    // Lưu lại trạng thái của frame hiện tại để làm "quá khứ" cho frame tiếp theo
    for (int i = 0; i < 256; ++i) {
        m_prevKeys[i] = m_keys[i];
    }
    m_prevLeftMouseDown = m_isLeftMouseDown;
}

// --- NHẬN TÍN HIỆU TỪ WINDOWS ---

void InputManager::OnKeyDown(unsigned int keycode) {
    if (keycode < 256) m_keys[keycode] = true;
}

void InputManager::OnKeyUp(unsigned int keycode) {
    if (keycode < 256) m_keys[keycode] = false;
}

void InputManager::OnMouseMove(int x, int y) {
    m_mouseX = (int)(x / m_scaleX);
    m_mouseY = (int)(y / m_scaleY);
}

void InputManager::OnLeftMouseDown() { m_isLeftMouseDown = true; }
void InputManager::OnLeftMouseUp() { m_isLeftMouseDown = false; }