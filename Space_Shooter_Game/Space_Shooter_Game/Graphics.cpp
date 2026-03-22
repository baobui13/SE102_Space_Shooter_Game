#include "Graphics.h"
#include <WICTextureLoader.h>
#include <stdexcept>

Graphics::Graphics(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;  // Tự động lấy theo kích thước cửa sổ
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Định dạng màu RGBA
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1; // Khử răng cưa (Antialiasing)
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2; // Double buffering (1 buffer ẩn, 1 buffer hiện)
    sd.OutputWindow = hwnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = 0;

    // Tạo Device, SwapChain và DeviceContext cùng lúc
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pDevice,
        nullptr,
        &m_pContext
    );

    // check lỗi
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create D3D device and swap chain");
    }

    // Lấy back buffer (bộ đệm ẩn) từ SwapChain để tạo Render Target View
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get back buffer");
    }

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTargetView);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view");
    }

    m_pContext->OMSetRenderTargets(1, m_pTargetView.GetAddressOf(), nullptr);

    RECT rect;
    GetClientRect(hwnd, &rect); // Lấy kích thước thực tế của cửa sổ

    D3D11_VIEWPORT vp = {};
    vp.Width = (float)(rect.right - rect.left);
    vp.Height = (float)(rect.bottom - rect.top);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;

    // Đưa Viewport vào Pipeline của DirectX
    m_pContext->RSSetViewports(1, &vp);

    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pContext.Get());
    m_states = std::make_unique<DirectX::CommonStates>(m_pDevice.Get());
}

void Graphics::ClearBuffer(float red, float green, float blue) {
    const float color[] = { red, green, blue, 1.0f };
    // Tô màu toàn bộ bộ đệm ẩn
    m_pContext->ClearRenderTargetView(m_pTargetView.Get(), color);

    m_pContext->OMSetRenderTargets(1, m_pTargetView.GetAddressOf(), nullptr);
}

void Graphics::EndFrame() {
    // Đổi bộ đệm ẩn lên màn hình chính (V-Sync bật: 1)
    HRESULT hr = m_pSwapChain->Present(1, 0);
    if (FAILED(hr)) {
        OutputDebugString(L"[Graphics] Present failed\n");
    }
}

// Hàm đọc file ảnh (.png, .jpg) từ ổ cứng và biến nó thành Texture của DirectX
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Graphics::LoadTexture(const wchar_t* filePath) {
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;

    // Gọi hàm của DirectXTK để đọc ảnh
    HRESULT hr = DirectX::CreateWICTextureFromFile(m_pDevice.Get(), filePath, nullptr, pTexture.GetAddressOf());

    // Báo lỗi nếu gõ sai tên file hoặc sai đường dẫn
    if (FAILED(hr)) {
        OutputDebugString(L"[Graphics] Failed to load texture\n");
        return nullptr;
    }

    return pTexture;
}