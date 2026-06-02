#include "SpriteAnimation.h"
#include "Graphics.h"

void SpriteAnimation::Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex) {
    m_texture = tex;
}

void SpriteAnimation::AddClip(const std::string& name, int x, int y, int w, int h, int count, int cols, float duration, bool loop, int spacingX, int spacingY) {
    m_clips[name] = { x, y, w, h, count, cols, spacingX, spacingY, duration, loop };
}

void SpriteAnimation::Play(const std::string& name) {
    auto it = m_clips.find(name);

    // FIX: check tồn tại
    if (it == m_clips.end()) return;

    if (m_currentClipName == name) return;

    m_currentClipName = name;
    m_currentClip = &it->second; // FIX: cache pointer

    m_localFrameIndex = 0;
    m_timer = 0.0f;
    m_isFinished = false;
}

void SpriteAnimation::Update(float dt) {
    if (!m_currentClip || m_isFinished) return;

    m_timer += dt;

    // FIX: dùng while để không skip frame
    while (m_timer >= m_currentClip->frameDuration) {
        m_timer -= m_currentClip->frameDuration;
        m_localFrameIndex++;

        if (m_localFrameIndex >= m_currentClip->frameCount) {
            if (m_currentClip->isLooping) {
                m_localFrameIndex = 0;
            }
            else {
                m_localFrameIndex = m_currentClip->frameCount - 1;
                m_isFinished = true;
                break;
            }
        }
    }
}

void SpriteAnimation::Render(Graphics& gfx, float drawX, float drawY, float drawW, float drawH, DirectX::XMVECTOR color, float rotation) {
    if (!m_texture || !m_currentClip) return;

    int col = m_localFrameIndex % m_currentClip->columns;
    int row = m_localFrameIndex / m_currentClip->columns;

    // tính toán sourceRect để cắt đúng frame ảnh
    RECT sourceRect = {
        m_currentClip->startX + (col * (m_currentClip->frameWidth + m_currentClip->spacingX)),
        m_currentClip->startY + (row * (m_currentClip->frameHeight + m_currentClip->spacingY)),
        m_currentClip->startX + (col * (m_currentClip->frameWidth + m_currentClip->spacingX)) + m_currentClip->frameWidth,
        m_currentClip->startY + (row * (m_currentClip->frameHeight + m_currentClip->spacingY)) + m_currentClip->frameHeight
    };

    // Origin (Tâm quay): Tính dựa trên kích thước của frame ảnh gốc
    DirectX::XMFLOAT2 origin(m_currentClip->frameWidth / 2.0f, m_currentClip->frameHeight / 2.0f);

    // Position (Vị trí vẽ): Dịch từ góc trên-trái (drawX, drawY) vào đúng tâm của quái vật
    DirectX::XMFLOAT2 position(drawX + drawW / 2.0f, drawY + drawH / 2.0f);

    // Scale (Tỉ lệ): Do hàm Draw này không tự bóp ảnh theo drawW/drawH như destRect, ta phải tự tính scale
    DirectX::XMFLOAT2 scale(drawW / (float)m_currentClip->frameWidth, drawH / (float)m_currentClip->frameHeight);

    // Gọi hàm Draw hỗ trợ xoay
    gfx.GetSpriteBatch()->Draw(
        m_texture.Get(),             // Texture
        position,                    // Tọa độ vẽ (Tâm)
        &sourceRect,                 // Vùng cắt trên ảnh
        color,                       // Màu sắc / Alpha
        rotation,                    // Góc xoay (Radian)
        origin,                      // Tâm quay (Local của frame)
        scale,                       // Tỉ lệ scale
        DirectX::SpriteEffects_None, // Hiệu ứng lật ảnh (nếu có)
        0.0f                         // Độ sâu (Layer Depth)
    );
}

void SpriteAnimation::RenderFrameTopLeft(Graphics& gfx, float drawX, float drawY, float uniformScale, DirectX::XMVECTOR color) {
    if (!m_texture || !m_currentClip) {
        return;
    }

    const RECT sourceRect = GetCurrentFrameRect();
    const float drawW = m_currentClip->frameWidth * uniformScale;
    const float drawH = m_currentClip->frameHeight * uniformScale;

    const RECT destRect = {
        static_cast<LONG>(drawX),
        static_cast<LONG>(drawY),
        static_cast<LONG>(drawX + drawW),
        static_cast<LONG>(drawY + drawH)
    };

    gfx.GetSpriteBatch()->Draw(m_texture.Get(), destRect, &sourceRect, color);
}

bool SpriteAnimation::IsFinished() const {
    return m_isFinished;
}

RECT SpriteAnimation::GetFrameRect(int frameIndex) const {
    if (!m_currentClip) {
        return { 0, 0, 0, 0 };
    }

    const int safeIndex = frameIndex % m_currentClip->frameCount;
    const int col = safeIndex % m_currentClip->columns;
    const int row = safeIndex / m_currentClip->columns;

    RECT rect;
    rect.left = m_currentClip->startX + (col * (m_currentClip->frameWidth + m_currentClip->spacingX));
    rect.top = m_currentClip->startY + (row * (m_currentClip->frameHeight + m_currentClip->spacingY));
    rect.right = rect.left + m_currentClip->frameWidth;
    rect.bottom = rect.top + m_currentClip->frameHeight;

    return rect;
}

RECT SpriteAnimation::GetCurrentFrameRect() const {
    return GetFrameRect(m_localFrameIndex);
}

void SpriteAnimation::RenderFrameAtIndex(Graphics& gfx, int frameIndex, float drawX, float drawY, float uniformScale, DirectX::XMVECTOR color) {
    if (!m_texture || !m_currentClip) {
        return;
    }

    const RECT sourceRect = GetFrameRect(frameIndex);
    const float drawW = m_currentClip->frameWidth * uniformScale;
    const float drawH = m_currentClip->frameHeight * uniformScale;

    const RECT destRect = {
        static_cast<LONG>(drawX),
        static_cast<LONG>(drawY),
        static_cast<LONG>(drawX + drawW),
        static_cast<LONG>(drawY + drawH)
    };

    gfx.GetSpriteBatch()->Draw(m_texture.Get(), destRect, &sourceRect, color);
}
