#pragma once

class InputManager {
private:
    bool m_keys[256] = { false };
    bool m_prevKeys[256] = { false };

    int m_mouseX = 0;
    int m_mouseY = 0;
    bool m_isLeftMouseDown = false;
    bool m_prevLeftMouseDown = false;
    bool m_isRightMouseDown = false;
    bool m_prevRightMouseDown = false;

    float m_scaleX = 1.0f;
    float m_scaleY = 1.0f;

public:
    InputManager() = default;

    void SetScale(float sx, float sy) { m_scaleX = sx; m_scaleY = sy; }

    bool IsKeyDown(unsigned int keycode) const;
    bool IsKeyPressed(unsigned int keycode) const;

    int GetMouseX() const;
    int GetMouseY() const;

    bool IsLeftMouseDown() const;
    bool IsLeftMouseClicked() const;
    bool IsRightMouseDown() const;
    bool IsRightMouseClicked() const;

    void Update();

    void OnKeyDown(unsigned int keycode);
    void OnKeyUp(unsigned int keycode);
    void OnMouseMove(int x, int y);
    void OnLeftMouseDown();
    void OnLeftMouseUp();
    void OnRightMouseDown();
    void OnRightMouseUp();
};
