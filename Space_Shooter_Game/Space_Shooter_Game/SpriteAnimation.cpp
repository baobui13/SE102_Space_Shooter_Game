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

void SpriteAnimation::Render(Graphics& gfx, float drawX, float drawY, float drawW, float drawH, DirectX::XMVECTOR color) {
    if (!m_texture || !m_currentClip) return;

    int col = m_localFrameIndex % m_currentClip->columns;
    int row = m_localFrameIndex / m_currentClip->columns;

    RECT sourceRect = {
        m_currentClip->startX + (col * (m_currentClip->frameWidth + m_currentClip->spacingX)),
        m_currentClip->startY + (row * (m_currentClip->frameHeight + m_currentClip->spacingY)),
        m_currentClip->startX + (col * (m_currentClip->frameWidth + m_currentClip->spacingX)) + m_currentClip->frameWidth,
        m_currentClip->startY + (row * (m_currentClip->frameHeight + m_currentClip->spacingY)) + m_currentClip->frameHeight
    };

    RECT destRect = {
        (LONG)drawX,
        (LONG)drawY,
        (LONG)(drawX + drawW),
        (LONG)(drawY + drawH)
    };

    // Chỉ gọi Draw một lần với color
    gfx.GetSpriteBatch()->Draw(m_texture.Get(), destRect, &sourceRect, color);
}

bool SpriteAnimation::IsFinished() const {
    return m_isFinished;
}

RECT SpriteAnimation::GetCurrentFrameRect() const {
    if (!m_currentClip) return { 0, 0, 0, 0 };

    int col = m_localFrameIndex % m_currentClip->columns;
    int row = m_localFrameIndex / m_currentClip->columns;

    RECT rect;
    rect.left = m_currentClip->startX + (col * (m_currentClip->frameWidth + m_currentClip->spacingX));
    rect.top = m_currentClip->startY + (row * (m_currentClip->frameHeight + m_currentClip->spacingY));
    rect.right = rect.left + m_currentClip->frameWidth;
    rect.bottom = rect.top + m_currentClip->frameHeight;

    return rect;
}
