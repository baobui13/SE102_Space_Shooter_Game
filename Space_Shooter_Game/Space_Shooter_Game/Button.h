#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>
#include <wrl.h>

class Button {
public:
    enum class State { NORMAL, HOVER, CLICKED };
    enum class TextAlignment {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    };

    Button(
        float x,
        float y,
        float width,
        float height,
        int origin = 1,
        const std::wstring& title = L"",
        DirectX::SpriteFont* font = nullptr,
        TextAlignment textAlignment = TextAlignment::CENTER,
        float textScale = 1.0f
    );
    ~Button() = default;

    void SetTextures(
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texNormal,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texHover,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texClicked = nullptr
    );

    void Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth = 0.0f, float screenHeight = 0.0f);
    void Render(
        DirectX::SpriteBatch* spriteBatch,
        float screenWidth = 0.0f,
        float screenHeight = 0.0f,
        DirectX::FXMVECTOR tint = DirectX::Colors::White,
        DirectX::FXMVECTOR textColor = DirectX::Colors::Black,
        float textScaleOverride = -1.0f,
        float padding = 20.0f
    ) const;
    bool IsClicked() const;

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    void SetPosition(float x, float y) { m_x = x; m_y = y; }
    void SetDimensions(float width, float height) { m_width = width; m_height = height; }
    void SetOrigin(int origin);
    int GetOrigin() const { return m_origin; }

    void SetTitle(const std::wstring& title) { m_title = title; }
    const std::wstring& GetTitle() const { return m_title; }
    void SetFont(DirectX::SpriteFont* font) { m_font = font; }
    DirectX::SpriteFont* GetFont() const { return m_font; }
    void SetTextAlignment(TextAlignment alignment) { m_textAlignment = alignment; }
    TextAlignment GetTextAlignment() const { return m_textAlignment; }
    void SetTextScale(float textScale) { m_textScale = textScale; }
    float GetTextScale() const { return m_textScale; }

    ID3D11ShaderResourceView* GetCurrentTexture() const;
    RECT GetDestinationRect(float screenWidth = 0.0f, float screenHeight = 0.0f) const;

private:
    DirectX::XMFLOAT2 CalculateTextPosition(const RECT& rect, float textWidth, float textHeight, float padding) const;

    float m_x;
    float m_y;
    float m_width;
    float m_height;
    int m_origin;
    State m_state;
    std::wstring m_title;
    DirectX::SpriteFont* m_font;
    TextAlignment m_textAlignment;
    float m_textScale;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texNormal;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texHover;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texClicked;
};
