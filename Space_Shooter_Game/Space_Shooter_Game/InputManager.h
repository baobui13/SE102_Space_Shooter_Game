#pragma once

class InputManager {
private:
    // Mảng 256 phần tử đại diện cho 256 mã phím (Virtual-Key Codes) của Windows
    bool m_keys[256] = { false };
    bool m_prevKeys[256] = { false }; // Lưu trạng thái phím ở frame trước

    // Tọa độ và trạng thái chuột
    int m_mouseX = 0;
    int m_mouseY = 0;
    bool m_isLeftMouseDown = false;
    bool m_prevLeftMouseDown = false; // Lưu trạng thái chuột ở frame trước

public:
    InputManager() = default;

    // --- CÁC HÀM CHO SCENE SỬ DỤNG LẤY THÔNG TIN ---
    // Kiểm tra xem phím có đang ĐƯỢC GIỮ không
    bool IsKeyDown(unsigned int keycode) const;

    // Kiểm tra xem phím VỪA ĐƯỢC NHẤN ở frame này không
    bool IsKeyPressed(unsigned int keycode) const;

    int GetMouseX() const;
    int GetMouseY() const;

    bool IsLeftMouseDown() const; // Chuột đang bị đè
    bool IsLeftMouseClicked() const; // Chuột vừa được click

    // --- HÀM CẬP NHẬT TRẠNG THÁI ---
    // Hàm này BẮT BUỘC phải gọi ở cuối mỗi vòng lặp game
    void Update();

    // --- CÁC HÀM NHẬN TÍN HIỆU TỪ WINDOWS (WNDPROC) ---
    void OnKeyDown(unsigned int keycode);
    void OnKeyUp(unsigned int keycode);
    void OnMouseMove(int x, int y);
    void OnLeftMouseDown();
    void OnLeftMouseUp();
};