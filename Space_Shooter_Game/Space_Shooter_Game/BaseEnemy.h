#pragma once
#include "AttackMarkerDefinitions.h"
#include "GameObject.h"
#include "IMovementStrategy.h"
#include <memory>

// Enum để phân loại enemy — dùng trong Factory và config
enum class EnemyType {
    Melee_Basic,
    Melee_Fast,
    Melee_Spawner,
    Ranged_Basic,
    Ranged_Burst,
    Boss_Stage1,
};

class BaseEnemy : public GameObject {
protected:
    float m_health;
    float m_maxHealth;
    float m_moveSpeed;
    float m_attackPower;
    float m_attackSpeed;
    float m_attackRange;
    bool m_isAttacking;
    float m_attackCooldown;
    float m_rotation = 0.0f;
    float m_spriteForwardAngle = 0.0f;
    float m_damageFlashTimer = 0.0f;

    EnemyType m_type;
    int m_expReward;
    float m_talentDropChance = 0.0f;
    int m_talentPointReward = 1;
    GameContext* m_lastCtx = nullptr;

    // Movement Strategy — có thể thay đổi runtime
    std::unique_ptr<IMovementStrategy> m_movementStrategy;

public:
    BaseEnemy(float x, float y, float width, float height,
              float health, float moveSpeed, float attackPower,
              float attackSpeed, float attackRange, EnemyType type = EnemyType::Melee_Basic);

    virtual void Update(float dt, GameContext& ctx) override;
    virtual void Render(Graphics& gfx) override;

    // Di chuyển — delegate sang movement strategy
    virtual void Move(float dt, GameContext& ctx);

    // Tấn công
    virtual void Attack(GameObject* target);
    // Nhận sát thương
    virtual void TakeDamage(float damage);
    // Chết
    virtual void OnDeath();
    // Xử lý hình ảnh/animation
    virtual void UpdateAnimation(float dt);

    // Đổi chiến lược di chuyển runtime
    void SetMovementStrategy(std::unique_ptr<IMovementStrategy> strategy);
    void SetSpriteForwardAngle(float radians) { m_spriteForwardAngle = radians; }
    void FacePoint(float targetX, float targetY);
    float GetAimAngle() const { return m_rotation + m_spriteForwardAngle; }
    void SpawnAttackMarker(GameContext& ctx, const AttackMarkerSpawnData& markerData);
    void RecordContext(GameContext& ctx);
    void DropRewards();
    void UpdateRotationToMovement();
    void SmoothRotationToward(float targetAngle, float dt, float maxAngularSpeed);

    // Getter
    float GetHealth() const { return m_health; }
    float GetMaxHealth() const { return m_maxHealth; }
    float GetMoveSpeed() const { return m_moveSpeed; }
    float GetAttackPower() const { return m_attackPower; }
    float GetAttackSpeed() const { return m_attackSpeed; }
    float GetAttackRange() const { return m_attackRange; }
    bool IsAttacking() const { return m_isAttacking; }
    EnemyType GetType() const { return m_type; }
    int GetExpReward() const { return m_expReward; }
    void SetExpReward(int exp) { m_expReward = exp; }
    void SetTalentDrop(float chance, int points) { m_talentDropChance = chance; m_talentPointReward = points; }
    float GetRotation() const { return m_rotation; }
    float GetSpriteForwardAngle() const { return m_spriteForwardAngle; }
};
