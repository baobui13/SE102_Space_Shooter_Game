#pragma once
#include "Bullet.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SkillManager.h"
#include <memory>
#include <vector>

class Player : public GameObject {
private:
    float m_speed;
    int m_hp;
    int m_maxHp;

    int m_attackDamage;
    float m_attackSpeed;
    float m_attackTimer;
    float m_attackRange;

    int m_level;
    int m_currentExp;
    int m_expToNextLevel;

    float m_magnetRange;
    float m_cooldownMultiplier;
    float m_skillSizeMultiplier;
    bool m_isShielded;

    int m_upgradePoints = 0;
    SkillManager m_skillManager;

public:
    Player(Graphics& gfx, float startX, float startY);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;
    void TakeDamage(int damage);

    void AddSkill(std::unique_ptr<Skill> skill) {
        m_skillManager.AddSkill(std::move(skill));
    }

    void GainExp(int amount);
    void LevelUp();

    int GetLevel() const { return m_level; }
    int GetCurrentExp() const { return m_currentExp; }
    int GetExpToNextLevel() const { return m_expToNextLevel; }
    int GetHp() const { return m_hp; }
    int GetMaxHp() const { return m_maxHp; }

    int GetAttackDamage() const { return m_attackDamage; }
    float GetAttackSpeed() const { return m_attackSpeed; }
    float GetAttackRange() const { return m_attackRange; }
    float GetMagnetRange() const { return m_magnetRange; }
    float GetCooldownMultiplier() const { return m_cooldownMultiplier; }
    float GetSkillSizeMultiplier() const { return m_skillSizeMultiplier; }
    float GetSpeed() const { return m_speed; }
    bool IsShielded() const { return m_isShielded; }

    int GetUpgradePoints() const { return m_upgradePoints; }
    void UseUpgradePoint() {
        if (m_upgradePoints > 0) {
            m_upgradePoints--;
        }
    }

    void Heal(int amount) {
        m_hp += amount;
        if (m_hp > m_maxHp) {
            m_hp = m_maxHp;
        }
    }

    void SetAttackDamage(int dmg) { m_attackDamage = dmg; }
    void SetMagnetRange(float range) { m_magnetRange = range; }
    void SetAttackSpeed(float speed) { m_attackSpeed = speed; }
    void SetCooldownMultiplier(float val) { m_cooldownMultiplier = val; }
    void SetSkillSizeMultiplier(float val) { m_skillSizeMultiplier = val; }
    void SetMaxHp(int maxHp) { m_maxHp = maxHp; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetAttackRange(float range) { m_attackRange = range; }
    void SetShielded(bool value) { m_isShielded = value; }
};
