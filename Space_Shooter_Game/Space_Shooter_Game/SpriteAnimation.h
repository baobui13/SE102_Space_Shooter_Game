#pragma once
#include <unordered_map>
#include <string>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXColors.h>

struct AnimClip {
    int startX, startY;
    int frameWidth, frameHeight;
    int frameCount;
    int columns;
    int spacingX;
    int spacingY;
    float frameDuration;
    bool isLooping;
};

class Graphics;

class SpriteAnimation {
public:
    void Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex);
    void AddClip(const std::string& name, int x, int y, int w, int h, int count, int cols, float duration, bool loop, int spacingX = 0, int spacingY = 0);
    void Play(const std::string& name);
    void Update(float dt);
    void Render(Graphics& gfx, float drawX, float drawY, float drawW, float drawH, DirectX::XMVECTOR color = DirectX::Colors::White);
    bool IsFinished() const;

    // Lấy Texture đang sử dụng
    ID3D11ShaderResourceView* GetCurrentTexture() const {
        return m_texture.Get();
    }

    // Lấy thông tin một Clip theo tên
    const AnimClip* GetClip(const std::string& name) const {
        auto it = m_clips.find(name);
        if (it != m_clips.end()) return &it->second;
        return nullptr;
    }

    // Lấy Rect của Frame hiện tại (dùng nếu bạn muốn chính xác khung hình đang chạy)
    RECT GetCurrentFrameRect() const;

    void SetAlpha(float alpha) { m_alpha = alpha; }

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    std::unordered_map<std::string, AnimClip> m_clips;

    std::string m_currentClipName;

    const AnimClip* m_currentClip = nullptr;

    int m_localFrameIndex = 0;
    float m_timer = 0.0f;
    bool m_isFinished = false;

    float m_alpha = 1.0f;
};
