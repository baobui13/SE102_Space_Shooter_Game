#pragma once
#include <string>
#include <d3d11.h>
#include <wrl.h>

class Button {
public:
    enum class State { NORMAL, HOVER, CLICKED };

    Button(float x, float y, float width, float height, int origin = 1);
    ~Button() = default;

    void SetTextures(
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texNormal,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texHover,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texClicked = nullptr
    );

    void Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth = 0.0f, float screenHeight = 0.0f);
    bool IsClicked() const;

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    void SetPosition(float x, float y) { m_x = x; m_y = y; }
    void SetDimensions(float width, float height) { m_width = width; m_height = height; }
    void SetOrigin(int origin);
    int GetOrigin() const { return m_origin; }

    ID3D11ShaderResourceView* GetCurrentTexture() const;
    RECT GetDestinationRect(float screenWidth = 0.0f, float screenHeight = 0.0f) const;

private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    int m_origin;
    State m_state;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texNormal;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texHover;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texClicked;
};
