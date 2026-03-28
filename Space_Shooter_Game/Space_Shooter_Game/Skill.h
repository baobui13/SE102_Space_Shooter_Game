#pragma once
#include <string>

enum class SkillType { ACTIVE, PASSIVE };

struct GameContext;

class Skill {
protected:
    std::string m_name;
    std::string m_description;
    SkillType m_type;
    int m_level;
    int m_maxLevel;
    float m_cooldown;
    float m_currentTimer;

public:
    Skill(std::string name, std::string desc, SkillType type, float cooldown, int maxLevel)
        : m_name(name), m_description(desc), m_type(type),
        m_cooldown(cooldown), m_currentTimer(0.0f),
        m_level(1), m_maxLevel(maxLevel) {
    }

    virtual ~Skill() = default;

    virtual void Update(float dt) {
        if (m_currentTimer > 0.0f) {
            m_currentTimer -= dt;
        }
    }

    bool IsReady() const { return m_currentTimer <= 0.0f; }
    void ResetCooldown() { m_currentTimer = m_cooldown; }

    virtual void Activate(GameContext& ctx) = 0;

    virtual void LevelUp() {
        if (m_level < m_maxLevel) {
            m_level++;
            OnLevelUp();
        }
    }

    std::string GetName() const { return m_name; }
    int GetLevel() const { return m_level; }
    SkillType GetType() const { return m_type; }

protected:
    virtual void OnLevelUp() = 0;
};