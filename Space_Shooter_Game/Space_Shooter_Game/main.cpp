#include "AudioManager.h"
#include "Window.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameConfig.h"
#include <chrono>
#include <objbase.h>

// Hàm WinMain là điểm bắt đầu của một ứng dụng Windows Desktop (thay cho int main() ở C++ Console)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);    // Khởi tạo COM (Component Object Model) để sử dụng DirectXTK

	// Khởi tạo cửa sổ game với kích thước chuẩn từ GameConfig
    Window gameWindow(L"Space Roguelike Game", (int)VIRTUAL_WIDTH, (int)VIRTUAL_HEIGHT);

    // Khởi tạo DirectX gắn vào handle (HWND) của cửa sổ vừa tạo
    Graphics gfx(gameWindow.GetHWND());

    AudioManager& audio = AudioManager::GetInstance();
    audio.Initialize();

    // Truyền tỉ lệ scale thực tế vào InputManager để tính tọa độ chuột chính xác
    gameWindow.GetInput().SetScale(gfx.GetScaleX(), gfx.GetScaleY());

    // Khởi tạo SceneManager
    SceneManager sceneManager;

    // Tạo MenuScene và nạp nó vào SceneManager ngay khi mở game
    sceneManager.ChangeScene(std::make_unique<MenuScene>(gfx));

    // Biến điều khiển Vòng lặp Game (Game Loop)
    bool isRunning = true;

    // Khởi tạo biến đếm thời gian
    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = tp1;

    // GAME LOOP: Vòng lặp này sẽ chạy liên tục nhiều khung hình trên giây (FPS)
    while (isRunning) {
        // Xử lý thông điệp Windows (Nếu trả về false nghĩa là người dùng đã đóng cửa sổ)
        if (!gameWindow.ProcessMessages()) {
            isRunning = false;
        }

        // ---------------------------------------------------------
        // TẠI ĐÂY: Sẽ là nơi cập nhật logic game (Update) 
        // và vẽ đồ họa DirectX (Render)
        // ---------------------------------------------------------


        // Tính Delta Time (thời gian trôi qua giữa 2 khung hình, tính bằng giây)
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float dt = elapsedTime.count();

        audio.Update();

        // --- UPDATE (Cập nhật logic) ---

        // Truyền InputManager vào hàm Update của SceneManager
        sceneManager.Update(dt, gameWindow.GetInput());

        // --- RENDER (Vẽ đồ họa) ---
        // SceneManager sẽ tự động gọi lệnh ClearBuffer của Scene hiện tại
        sceneManager.Render(gfx);

        gfx.EndFrame();

        gameWindow.GetInput().Update();
    }

    audio.Shutdown();
    CoUninitialize();
    return 0;
}
