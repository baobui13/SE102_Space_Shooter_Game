#pragma once
#include "IMovementStrategy.h"
#include "GameConfig.h"
#include "MovementStrategies.h"
#include <cmath>
#include <memory>
#include <utility>

// Blend mượt giữa hai movement strategy khi đổi bước trong sequence
class BlendedMovementTransition : public IMovementStrategy {
    std::unique_ptr<IMovementStrategy> m_from;
    std::unique_ptr<IMovementStrategy> m_to;
    float m_elapsed = 0.0f;
    bool m_synced = false;

public:
    BlendedMovementTransition(std::unique_ptr<IMovementStrategy> from,
                              std::unique_ptr<IMovementStrategy> to)
        : m_from(std::move(from)), m_to(std::move(to)) {}

    bool IsComplete() const {
        return m_elapsed >= MOVE_BLEND_DURATION;
    }

    std::unique_ptr<IMovementStrategy> ReleaseTo() {
        m_from.reset();
        return std::move(m_to);
    }

    void SyncFromPosition(float cx, float cy) override {
        if (m_from) {
            m_from->SyncFromPosition(cx, cy);
        }
        if (m_to) {
            m_to->SyncFromPosition(cx, cy);
        }
        m_synced = true;
    }

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        if (!m_synced) {
            SyncFromPosition(cx, cy);
        }

        if (!m_to) {
            outVx = 0.0f;
            outVy = 0.0f;
            return;
        }

        if (IsComplete() || !m_from) {
            m_to->CalculateVelocity(cx, cy, dt, ctx, outVx, outVy);
            return;
        }

        m_elapsed += dt;
        const float blendT = m_elapsed / MOVE_BLEND_DURATION;
        const float w = MovementMath::SmoothStep(blendT);

        float fromVx = 0.0f;
        float fromVy = 0.0f;
        float toVx = 0.0f;
        float toVy = 0.0f;
        m_from->CalculateVelocity(cx, cy, dt, ctx, fromVx, fromVy);
        m_to->CalculateVelocity(cx, cy, dt, ctx, toVx, toVy);

        float fromX = cx;
        float fromY = cy;
        float toX = cx;
        float toY = cy;
        const bool hasFromIdeal = m_from->TryGetIdealPosition(fromX, fromY);
        const bool hasToIdeal = m_to->TryGetIdealPosition(toX, toY);

        if (hasFromIdeal && hasToIdeal && dt > 1e-6f) {
            const float blendX = fromX + (toX - fromX) * w;
            const float blendY = fromY + (toY - fromY) * w;
            MovementMath::VelocityToward(cx, cy, blendX, blendY, dt, outVx, outVy);
        } else {
            outVx = fromVx + (toVx - fromVx) * w;
            outVy = fromVy + (toVy - fromVy) * w;
        }
    }
};
