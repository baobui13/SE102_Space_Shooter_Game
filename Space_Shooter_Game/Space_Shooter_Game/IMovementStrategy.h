#pragma once

struct GameContext;

// Interface cho các chiến lược di chuyển của Enemy
// Sử dụng Strategy Pattern để tách logic di chuyển ra khỏi Enemy class
class IMovementStrategy {
public:
    virtual ~IMovementStrategy() = default;

    // Tính toán vận tốc (vx, vy) dựa trên vị trí hiện tại và context
    virtual void CalculateVelocity(
        float currentX, float currentY,
        float dt, GameContext& ctx,
        float& outVx, float& outVy
    ) = 0;
};
