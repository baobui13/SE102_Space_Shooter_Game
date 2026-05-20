#pragma once
#include "IMovementStrategy.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>
#include <memory>

namespace MovementMath {
    inline void VelocityToward(float cx, float cy, float targetX, float targetY,
        float dt, float& outVx, float& outVy) {
        if (dt > 1e-6f) {
            outVx = (targetX - cx) / dt;
            outVy = (targetY - cy) / dt;
        }
        else {
            outVx = 0.0f;
            outVy = 0.0f;
        }
    }

    inline float SmoothStep(float t) {
        if (t <= 0.0f) return 0.0f;
        if (t >= 1.0f) return 1.0f;
        return t * t * (3.0f - 2.0f * t);
    }
}

// ============================================================
// Di chuyển đuổi theo Player
// ============================================================
class ChaseMovement : public IMovementStrategy {
    float m_speed;
public:
    ChaseMovement(float speed) : m_speed(speed) {}

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        float targetX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
        float targetY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
        float dx = targetX - cx;
        float dy = targetY - cy;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist > 1e-2f) {
            outVx = m_speed * dx / dist;
            outVy = m_speed * dy / dist;
        } else {
            outVx = 0.0f;
            outVy = 0.0f;
        }
    }
};

// ============================================================
// Di chuyển thẳng theo hướng cố định (top-down classic)
// ============================================================
class LinearMovement : public IMovementStrategy {
    float m_speedX, m_speedY;
public:
    LinearMovement(float sx, float sy) : m_speedX(sx), m_speedY(sy) {}

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        outVx = m_speedX;
        outVy = m_speedY;
    }
};

// ============================================================
// Di chuyển sóng sin (zigzag) — xuống + lắc ngang
// ============================================================
class SineWaveMovement : public IMovementStrategy {
    float m_speedY;
    float m_amplitude;
    float m_frequency;
    float m_elapsed = 0.0f;
    float m_anchorX = 0.0f;
    float m_anchorY = 0.0f;
    bool m_hasAnchor = false;

    void EnsureAnchor(float cx, float cy) {
        if (!m_hasAnchor) {
            m_anchorX = cx;
            m_anchorY = cy;
            m_hasAnchor = true;
        }
    }

public:
    SineWaveMovement(float speedY, float amplitude, float frequency)
        : m_speedY(speedY), m_amplitude(amplitude), m_frequency(frequency) {}

    void SyncFromPosition(float cx, float cy) override {
        EnsureAnchor(cx, cy);

        if (std::abs(m_speedY) > 1e-3f) {
            m_elapsed = (cy - m_anchorY) / m_speedY;
            if (m_elapsed < 0.0f) {
                m_elapsed = 0.0f;
            }
        }

        if (std::abs(m_frequency) > 1e-3f && std::abs(m_amplitude) > 1e-3f) {
            constexpr int SAMPLE_COUNT = 36;
            constexpr float TWO_PI = 6.283185307f;
            float bestT = m_elapsed;
            float bestDistSq = -1.0f;

            for (int i = 0; i < SAMPLE_COUNT; ++i) {
                const float t = m_elapsed + (i - SAMPLE_COUNT / 2) * 0.05f;
                if (t < 0.0f) {
                    continue;
                }
                const float phase = m_frequency * t;
                const float px = m_anchorX + (m_amplitude / m_frequency) * std::sin(phase);
                const float py = m_anchorY + m_speedY * t;
                const float dx = cx - px;
                const float dy = cy - py;
                const float distSq = dx * dx + dy * dy;
                if (bestDistSq < 0.0f || distSq < bestDistSq) {
                    bestDistSq = distSq;
                    bestT = t;
                }
            }
            m_elapsed = bestT;
        }
    }

    bool TryGetIdealPosition(float& outX, float& outY) const override {
        if (!m_hasAnchor) {
            return false;
        }
        if (std::abs(m_frequency) > 1e-3f) {
            const float phase = m_frequency * m_elapsed;
            outX = m_anchorX + (m_amplitude / m_frequency) * std::sin(phase);
        } else {
            outX = m_anchorX;
        }
        outY = m_anchorY + m_speedY * m_elapsed;
        return true;
    }

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        EnsureAnchor(cx, cy);
        m_elapsed += dt;

        float idealX = cx;
        float idealY = cy;
        if (TryGetIdealPosition(idealX, idealY)) {
            MovementMath::VelocityToward(cx, cy, idealX, idealY, dt, outVx, outVy);
        } else {
            outVx = m_amplitude * std::cos(m_frequency * m_elapsed);
            outVy = m_speedY;
        }
    }
};

// ============================================================
// Di chuyển vòng tròn quanh một tâm
// ============================================================
class CircularMovement : public IMovementStrategy {
    float m_centerX, m_centerY;
    float m_radius, m_angularSpeed;
    float m_angle = 0.0f;

    void UpdateIdealPosition(float& outX, float& outY) const {
        outX = m_centerX + m_radius * std::cos(m_angle);
        outY = m_centerY + m_radius * std::sin(m_angle);
    }

public:
    CircularMovement(float centerX, float centerY, float radius, float angularSpeed)
        : m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_angularSpeed(angularSpeed) {}

    void SyncFromPosition(float cx, float cy) override {
        m_angle = std::atan2(cy - m_centerY, cx - m_centerX);
    }

    bool TryGetIdealPosition(float& outX, float& outY) const override {
        UpdateIdealPosition(outX, outY);
        return true;
    }

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        m_angle += m_angularSpeed * dt;
        float idealX = cx;
        float idealY = cy;
        UpdateIdealPosition(idealX, idealY);
        MovementMath::VelocityToward(cx, cy, idealX, idealY, dt, outVx, outVy);
    }
};

// ============================================================
// Di chuyển hình số 8 (lemniscate) quanh tâm
// ============================================================
class FigureEightMovement : public IMovementStrategy {
    float m_centerX;
    float m_centerY;
    float m_radiusX;
    float m_radiusY;
    float m_angularSpeed;
    float m_angle = 0.0f;

    void UpdateIdealPosition(float& outX, float& outY) const {
        const float sinT = std::sin(m_angle);
        const float cosT = std::cos(m_angle);
        outX = m_centerX + m_radiusX * sinT;
        outY = m_centerY + m_radiusY * sinT * cosT;
    }

public:
    FigureEightMovement(float centerX, float centerY, float radiusX, float radiusY, float angularSpeed = 0.5f)
        : m_centerX(centerX),
        m_centerY(centerY),
        m_radiusX(radiusX),
        m_radiusY(radiusY),
        m_angularSpeed(angularSpeed) {}

    void SyncFromPosition(float cx, float cy) override {
        constexpr float TWO_PI = 6.283185307f;
        constexpr int SAMPLE_COUNT = 72;

        float bestAngle = 0.0f;
        float bestDistSq = -1.0f;

        for (int i = 0; i < SAMPLE_COUNT; ++i) {
            const float t = TWO_PI * static_cast<float>(i) / static_cast<float>(SAMPLE_COUNT);
            const float sinT = std::sin(t);
            const float cosT = std::cos(t);
            const float px = m_centerX + m_radiusX * sinT;
            const float py = m_centerY + m_radiusY * sinT * cosT;
            const float dx = cx - px;
            const float dy = cy - py;
            const float distSq = dx * dx + dy * dy;

            if (bestDistSq < 0.0f || distSq < bestDistSq) {
                bestDistSq = distSq;
                bestAngle = t;
            }
        }

        m_angle = bestAngle;
    }

    bool TryGetIdealPosition(float& outX, float& outY) const override {
        UpdateIdealPosition(outX, outY);
        return true;
    }

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        m_angle += m_angularSpeed * dt;
        float idealX = cx;
        float idealY = cy;
        UpdateIdealPosition(idealX, idealY);
        MovementMath::VelocityToward(cx, cy, idealX, idealY, dt, outVx, outVy);
    }
};

// ============================================================
// Bọc movement khác: tránh xa player khi quá gần
// ============================================================
class FleeFromPlayerMovement : public IMovementStrategy {
    std::unique_ptr<IMovementStrategy> m_inner;
    float m_triggerDistance;
    float m_fleeSpeed;

public:
    FleeFromPlayerMovement(std::unique_ptr<IMovementStrategy> inner,
                           float triggerDistance,
                           float fleeSpeed)
        : m_inner(std::move(inner)),
        m_triggerDistance(triggerDistance),
        m_fleeSpeed(fleeSpeed) {}

    void SyncFromPosition(float currentX, float currentY) override {
        if (m_inner) {
            m_inner->SyncFromPosition(currentX, currentY);
        }
    }

    bool TryGetIdealPosition(float& outX, float& outY) const override {
        if (m_inner) {
            return m_inner->TryGetIdealPosition(outX, outY);
        }
        return false;
    }

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
        float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
        float dx = cx - playerCX;
        float dy = cy - playerCY;
        float distSq = dx * dx + dy * dy;
        float triggerSq = m_triggerDistance * m_triggerDistance;

        if (distSq < triggerSq) {
            if (distSq > 1e-4f) {
                float dist = std::sqrt(distSq);
                outVx = m_fleeSpeed * dx / dist;
                outVy = m_fleeSpeed * dy / dist;
            }
            else {
                outVx = 0.0f;
                outVy = -m_fleeSpeed;
            }
            return;
        }

        if (m_inner) {
            m_inner->CalculateVelocity(cx, cy, dt, ctx, outVx, outVy);
        }
        else {
            outVx = 0.0f;
            outVy = 0.0f;
        }
    }
};
