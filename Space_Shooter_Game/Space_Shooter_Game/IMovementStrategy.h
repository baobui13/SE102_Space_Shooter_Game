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

    // Đồng bộ phase đường đi từ vị trí hiện tại — tránh giật khi đổi movement
    virtual void SyncFromPosition(float currentX, float currentY) {}

    // Vị trí lý tưởng trên quỹ đạo (sau khi advance phase trong CalculateVelocity)
    virtual bool TryGetIdealPosition(float& outX, float& outY) const { return false; }
};
