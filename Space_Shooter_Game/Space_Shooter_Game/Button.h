#pragma once
#include <string>
#include <d3d11.h>
#include <wrl.h> // Để dùng ComPtr quản lý bộ nhớ ảnh

class Button {
public:
    // Trạng thái của nút 
    enum class State { NORMAL, HOVER, CLICKED };

    // Khởi tạo nút với tọa độ và kích thước
    Button(float x, float y, float width, float height);
    ~Button() = default;

    // Nạp ảnh cho các trạng thái (sẽ gọi sau khi tạo nút)
    void SetTextures(
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texNormal,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texHover,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texClicked = nullptr
    );

    // Cập nhật trạng thái nút
    void Update(float mouseX, float mouseY, bool isLeftClicked);
    bool IsClicked() const;

    // Lấy tọa độ để vẽ
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }

	// Lấy kích thước để vẽ
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    // Trả về đúng bức ảnh cần vẽ dựa trên trạng thái hiện tại
    ID3D11ShaderResourceView* GetCurrentTexture() const;

private:
    float m_x, m_y, m_width, m_height;
    State m_state;

    // 3 biến chứa 3 bức ảnh của nút
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texNormal;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texHover;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texClicked;
};