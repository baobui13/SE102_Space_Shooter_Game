#pragma once
#include "BaseEnemy.h"

enum class RangeState {
    Moving,     // Trạng thái di chuyển (hoặc kiểm tra khoảng cách)
    Aiming,     // Trạng thái ngắm (hiện đường đỏ)
    Shooting    // Trạng thái chuẩn bị bắn và bắn
};

class RangeEnemy1 : public BaseEnemy {
private:
    RangeState m_state = RangeState::Moving;
    float m_stateTimer = 0.0f;

    // Các hằng số cấu hình
    const float TIME_MOVE = 1.0f;
    const float TIME_AIM = 1.0f;
    const float TIME_SHOOT = 0.5f;
    const float SAFE_DISTANCE = 300.0f; // Khoảng cách mà quái sẽ bỏ chạy nếu player lại gần
    const float BULLET_SPEED = 600.0f;

public:
    RangeEnemy1(float x, float y, float width, float height,
        float health, float moveSpeed, float attackPower,
        float attackSpeed = 1.0f, float attackRange = 500.0f,
        EnemyType type = EnemyType::Ranged_Basic);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;

private:
    void ResetState(RangeState newState);
    void UpdateRotationToPlayer(GameContext& ctx);
};
