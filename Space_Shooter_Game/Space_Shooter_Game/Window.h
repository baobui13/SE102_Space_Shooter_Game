#pragma once
#pragma once
#include <Windows.h>
#include <string>
#include "InputManager.h"

class Window {
public:
    // Hàm khởi tạo: truyền vào tiêu đề, chiều rộng và chiều cao cửa sổ
    Window(const std::wstring& title, int width, int height);
    ~Window();

    // Hàm xử lý các sự kiện của Windows (chuột, phím, đóng cửa sổ...)
    bool ProcessMessages();

    // Lấy handle của cửa sổ để sau này truyền cho DirectX
    HWND GetHWND() const { return m_hwnd; }

    // Hàm để truyền InputManager cho SceneManager
    InputManager& GetInput() { return m_inputManager; }

private:

    HWND m_hwnd;
    HINSTANCE m_hInstance;
    std::wstring m_title;
    int m_width;
    int m_height;

    InputManager m_inputManager; // Window sẽ chứa InputManager

    // Hàm xử lý tín hiệu tĩnh bắt buộc của Win32 API
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Hàm xử lý tín hiệu thực sự của object
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};