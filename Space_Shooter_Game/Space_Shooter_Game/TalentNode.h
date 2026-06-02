#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>
#include <wrl.h>

enum class TalentType {
    ATTACK_DAMAGE,
    ATTACK_SPEED,
    ATTACK_RANGE,
    COOLDOWN_REDUCTION,
    SKILL_SIZE,
    MAX_HP,
    MOVEMENT_SPEED
};

class TalentNode {
public:
    TalentNode(int id, int parentId, TalentType type, float buffAmount, float x, float y);
    ~TalentNode() = default;

    void SetIcon(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> icon);

    void Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth, float screenHeight);
    void Render(DirectX::SpriteBatch* spriteBatch, float screenWidth, float screenHeight, DirectX::SpriteFont* font = nullptr) const;

    int GetId() const { return m_id; }
    int GetParentId() const { return m_parentId; }
    TalentType GetType() const { return m_type; }
    float GetBuffAmount() const { return m_buffAmount; }
    bool IsUnlocked() const { return m_isUnlocked; }
    void Unlock() { m_isUnlocked = true; }

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    bool IsClicked() const { return m_isClicked; }
    void ResetClicked() { m_isClicked = false; }
    bool IsHovered() const { return m_isHovered; }
    
    std::wstring GetDescription() const;

private:
    int m_id;
    int m_parentId;
    TalentType m_type;
    float m_buffAmount;
    float m_x;
    float m_y;

    bool m_isUnlocked;
    bool m_isHovered;
    bool m_isClicked;
    bool m_wasLeftClicked;

    float m_width;
    float m_height;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_icon;
};
