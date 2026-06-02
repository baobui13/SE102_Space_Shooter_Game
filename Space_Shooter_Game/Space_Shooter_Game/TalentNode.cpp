#include "TalentNode.h"
#include <sstream>
#include <iomanip>

TalentNode::TalentNode(int id, int parentId, TalentType type, float buffAmount, float x, float y)
    : m_id(id), m_parentId(parentId), m_type(type), m_buffAmount(buffAmount),
      m_x(x), m_y(y), m_isUnlocked(false), m_isHovered(false), m_isClicked(false), m_wasLeftClicked(false),
      m_width(64.0f), m_height(64.0f) // Default size
{
    if (id == 0) { // Root node is always unlocked
        m_isUnlocked = true;
    }
}

void TalentNode::SetIcon(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> icon) {
    m_icon = icon;
}

void TalentNode::Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth, float screenHeight) {
    m_isClicked = false;
    
    // We assume coordinates are relative to the center or scaled according to screen if needed.
    // For simplicity, let's treat x,y as center of the node.
    float halfW = m_width / 2.0f;
    float halfH = m_height / 2.0f;
    
    if (mouseX >= m_x - halfW && mouseX <= m_x + halfW &&
        mouseY >= m_y - halfH && mouseY <= m_y + halfH) {
        m_isHovered = true;
        if (isLeftClicked && !m_wasLeftClicked) {
            m_isClicked = true;
        }
    } else {
        m_isHovered = false;
    }
    
    m_wasLeftClicked = isLeftClicked;
}

void TalentNode::Render(DirectX::SpriteBatch* spriteBatch, float screenWidth, float screenHeight, DirectX::SpriteFont* font) const {
    if (!spriteBatch) return;

    if (!m_icon) return;
    
    DirectX::XMVECTOR color = DirectX::Colors::White;
    if (!m_isUnlocked) {
        if (m_isHovered) {
            color = DirectX::Colors::LightGray;
        } else {
            color = DirectX::Colors::DarkSlateGray;
        }
    } else if (m_isHovered) {
        color = DirectX::Colors::Yellow; // Highlight even when unlocked
    }
    
    RECT destRect = {
        static_cast<LONG>(m_x - m_width / 2.0f),
        static_cast<LONG>(m_y - m_height / 2.0f),
        static_cast<LONG>(m_x + m_width / 2.0f),
        static_cast<LONG>(m_y + m_height / 2.0f)
    };
    spriteBatch->Draw(m_icon.Get(), destRect, nullptr, color);
}

std::wstring TalentNode::GetDescription() const {
    std::wstringstream wss;
    switch (m_type) {
        case TalentType::ATTACK_DAMAGE: wss << L"Attack Damage +"; break;
        case TalentType::ATTACK_SPEED: wss << L"Attack Speed +"; break;
        case TalentType::ATTACK_RANGE: wss << L"Attack Range +"; break;
        case TalentType::COOLDOWN_REDUCTION: wss << L"Cooldown Reduction +"; break;
        case TalentType::SKILL_SIZE: wss << L"Skill Size +"; break;
        case TalentType::MAX_HP: wss << L"Max HP +"; break;
        case TalentType::MOVEMENT_SPEED: wss << L"Movement Speed +"; break;
    }
    
    if (m_type == TalentType::ATTACK_SPEED || m_type == TalentType::COOLDOWN_REDUCTION || m_type == TalentType::SKILL_SIZE) {
        wss << std::fixed << std::setprecision(1) << (m_buffAmount * 100.0f) << L"%";
    } else {
        wss << std::fixed << std::setprecision(0) << m_buffAmount;
    }
    
    return wss.str();
}
