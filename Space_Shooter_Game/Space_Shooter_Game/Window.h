#pragma once
#pragma once
#include <Windows.h>
#include <string>

class Window {
public:
    // Hàm khởi tạo: truyền vào tiêu đề, chiều rộng và chiều cao cửa sổ
    Window(const std::wstring& title, int width, int height);
    ~Window();

    // Hàm xử lý các sự kiện của Windows (chuột, phím, đóng cửa sổ...)
    bool ProcessMessages();

    // Lấy handle của cửa sổ để sau này truyền cho DirectX
    HWND GetHWND() const { return m_hwnd; }

private:
    // Hàm callback xử lý thông điệp của Windows
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    HINSTANCE m_hInstance;
    std::wstring m_title;
    int m_width;
    int m_height;
};