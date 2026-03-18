#pragma once
#include <unordered_map>
#include <string>
#include <wrl.h>
#include <d3d11.h>

struct AnimClip {
    int startX, startY;
    int frameWidth, frameHeight;
    int frameCount;
    int columns;
    float frameDuration;
    bool isLooping;
};

class Graphics;

class SpriteAnimation {
public:
    void Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex);
    void AddClip(const std::string& name, int x, int y, int w, int h, int count, int cols, float duration, bool loop);
    void Play(const std::string& name);
    void Update(float dt);
    void Render(Graphics& gfx, float drawX, float drawY, float drawW, float drawH);
    bool IsFinished() const;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    std::unordered_map<std::string, AnimClip> m_clips;

    std::string m_currentClipName;

    // FIX: thêm biến này 👇
    const AnimClip* m_currentClip = nullptr;

    int m_localFrameIndex = 0;
    float m_timer = 0.0f;
    bool m_isFinished = false;
};