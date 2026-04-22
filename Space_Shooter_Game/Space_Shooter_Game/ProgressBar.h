#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>

class ProgressBar {
public:
    ProgressBar(float x, float y, float width, float height, int origin = 1);
    ~ProgressBar() = default;

    void SetTextures(
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> background,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> progress,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> border
    );

    void Render(DirectX::SpriteBatch* spriteBatch, float percentage, float screenWidth = 0.0f, float screenHeight = 0.0f);

    void SetPosition(float x, float y) { m_x = x; m_y = y; }
    void SetDimensions(float width, float height) { m_width = width; m_height = height; }
    void SetOrigin(int origin);
    int GetOrigin() const { return m_origin; }

private:
    RECT BuildDestinationRect(float screenWidth, float screenHeight) const;

    float m_x;
    float m_y;
    float m_width;
    float m_height;
    int m_origin;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texBackground;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texProgress;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texBorder;
};
