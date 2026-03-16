#pragma once
#include "Window.h"
#include <windowsx.h>

// Xử lý các thông điệp từ hệ điều hành Windows
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    
    Window* pThis = nullptr;

    // Khi cửa sổ vừa được tạo, Windows gửi thông điệp WM_NCCREATE đầu tiên
    if (uMsg == WM_NCCREATE) {
        // Trích xuất con trỏ 'this' truyền vào CreateWindowEx
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Window*>(pCreate->lpCreateParams);

        // Lưu con trỏ pThis vào bộ nhớ dữ liệu người dùng của cửa sổ này
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
		// Với các thông điệp khác (như phím, chuột), dùng con trỏ pThis đã lưu để gọi hàm xử lý của object Window
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    // Nếu đã lấy được con trỏ của Object Window, gọi hàm xử lý KHÔNG TĨNH
    if (pThis) {
        return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



// Đây là hàm KHÔNG TĨNH, có thể truy cập m_inputManager bình thường
LRESULT Window::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

        // --- XỬ LÝ BÀN PHÍM ---
    case WM_KEYDOWN:
        m_inputManager.OnKeyDown(static_cast<unsigned int>(wParam));
        break;

    case WM_KEYUP:
        m_inputManager.OnKeyUp(static_cast<unsigned int>(wParam));
        break;

        // --- XỬ LÝ CHUỘT ---
    case WM_MOUSEMOVE:
        m_inputManager.OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_LBUTTONDOWN:
        m_inputManager.OnLeftMouseDown();
        break;

    case WM_LBUTTONUP:
        m_inputManager.OnLeftMouseUp();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



Window::Window(const std::wstring& title, int width, int height)
    : m_title(title), m_width(width), m_height(height), m_hInstance(GetModuleHandle(nullptr)), m_hwnd(nullptr) {

    const wchar_t* CLASS_NAME = L"SpaceRoguelikeWindowClass";

    // Cấu hình lớp cửa sổ
    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.lpfnWndProc = WindowProc;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Màu nền cửa sổ mặc định

    RegisterClass(&wndClass);

    // Tính toán kích thước cửa sổ sao cho phần hiển thị (Client Area) đúng với width, height
    RECT wr = { 0, 0, m_width, m_height };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    // Tạo cửa sổ
    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        m_title.c_str(),
        WS_OVERLAPPEDWINDOW, // Kiểu cửa sổ có thanh tiêu đề và nút đóng/thu nhỏ
        CW_USEDEFAULT, CW_USEDEFAULT, // Vị trí xuất hiện trên màn hình
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        m_hInstance,
        this
    );

    // Hiển thị cửa sổ lên màn hình
    if (m_hwnd) {
        ShowWindow(m_hwnd, SW_SHOW);
    }
}

Window::~Window() {
    const wchar_t* CLASS_NAME = L"SpaceRoguelikeWindowClass";
    UnregisterClass(CLASS_NAME, m_hInstance);
}

// Lấy thông điệp từ hàng đợi và gửi đi xử lý
bool Window::ProcessMessages() {
    MSG msg = {};
    // Dùng PeekMessage thay vì GetMessage để không chặn Game Loop
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false; // Trả về false nếu người dùng bấm dấu X đóng cửa sổ
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true; // Game vẫn tiếp tục
}