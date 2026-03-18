#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h> // Thư viện chứa ComPtr giúp tự động dọn dẹp bộ nhớ DirectX
#include <CommonStates.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <memory>

// Tự động link thư viện d3d11
#pragma comment(lib, "d3d11.lib")

class Graphics {
public:
    // Khởi tạo DirectX gắn vào cửa sổ (HWND)
    Graphics(HWND hwnd);
    ~Graphics() = default;

    // Xóa màn hình cũ và tô màu nền mới (R, G, B)
    void ClearBuffer(float red, float green, float blue);

    // Đẩy hình ảnh đã vẽ lên màn hình
    void EndFrame();

    // Đọc file ảnh từ ổ cứng
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(const wchar_t* filePath);

    // Lấy cọ vẽ SpriteBatch ra vẽ
    DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }

    DirectX::CommonStates* GetStates() { return m_states.get(); }

private:
    // Các thành phần cốt lõi của DirectX 11
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;                 // Quản lý bộ nhớ GPU, tạo tài nguyên (texture, buffer...)
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;            // Quản lý việc lật trang màn hình (tránh hiện tượng xé hình)
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;         // Ra lệnh cho GPU vẽ hình
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView;   // Nơi mà GPU sẽ vẽ lên (màn hình)

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;            // SpriteBatch giúp vẽ hình 2D dễ dàng hơn
    std::unique_ptr<DirectX::CommonStates> m_states;
};