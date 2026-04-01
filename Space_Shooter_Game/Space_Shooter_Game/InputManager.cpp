#include "InputManager.h"

bool InputManager::IsKeyDown(unsigned int keycode) const {
    if (keycode >= 256) {
        return false;
    }
    return m_keys[keycode];
}

bool InputManager::IsKeyPressed(unsigned int keycode) const {
    if (keycode >= 256) {
        return false;
    }
    return m_keys[keycode] && !m_prevKeys[keycode];
}

int InputManager::GetMouseX() const { return m_mouseX; }
int InputManager::GetMouseY() const { return m_mouseY; }

bool InputManager::IsLeftMouseDown() const { return m_isLeftMouseDown; }

bool InputManager::IsLeftMouseClicked() const {
    return m_isLeftMouseDown && !m_prevLeftMouseDown;
}

bool InputManager::IsRightMouseDown() const { return m_isRightMouseDown; }

bool InputManager::IsRightMouseClicked() const {
    return m_isRightMouseDown && !m_prevRightMouseDown;
}

void InputManager::Update() {
    for (int i = 0; i < 256; ++i) {
        m_prevKeys[i] = m_keys[i];
    }

    m_prevLeftMouseDown = m_isLeftMouseDown;
    m_prevRightMouseDown = m_isRightMouseDown;
}

void InputManager::OnKeyDown(unsigned int keycode) {
    if (keycode < 256) {
        m_keys[keycode] = true;
    }
}

void InputManager::OnKeyUp(unsigned int keycode) {
    if (keycode < 256) {
        m_keys[keycode] = false;
    }
}

void InputManager::OnMouseMove(int x, int y) {
    m_mouseX = static_cast<int>(x / m_scaleX);
    m_mouseY = static_cast<int>(y / m_scaleY);
}

void InputManager::OnLeftMouseDown() { m_isLeftMouseDown = true; }
void InputManager::OnLeftMouseUp() { m_isLeftMouseDown = false; }
void InputManager::OnRightMouseDown() { m_isRightMouseDown = true; }
void InputManager::OnRightMouseUp() { m_isRightMouseDown = false; }
