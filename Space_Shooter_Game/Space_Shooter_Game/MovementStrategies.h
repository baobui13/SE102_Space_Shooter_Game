#pragma once
#include "IMovementStrategy.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

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
public:
    SineWaveMovement(float speedY, float amplitude, float frequency)
        : m_speedY(speedY), m_amplitude(amplitude), m_frequency(frequency) {}

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        m_elapsed += dt;
        outVx = m_amplitude * std::cos(m_frequency * m_elapsed);
        outVy = m_speedY;
    }
};

// ============================================================
// Di chuyển vòng tròn quanh một tâm
// ============================================================
class CircularMovement : public IMovementStrategy {
    float m_centerX, m_centerY;
    float m_radius, m_angularSpeed;
    float m_angle = 0.0f;
public:
    CircularMovement(float centerX, float centerY, float radius, float angularSpeed)
        : m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_angularSpeed(angularSpeed) {}

    void CalculateVelocity(float cx, float cy, float dt, GameContext& ctx,
                           float& outVx, float& outVy) override {
        m_angle += m_angularSpeed * dt;
        float targetX = m_centerX + m_radius * std::cos(m_angle);
        float targetY = m_centerY + m_radius * std::sin(m_angle);
        // Tính vận tốc cần để đến vị trí tiếp theo
        if (dt > 1e-6f) {
            outVx = (targetX - cx) / dt;
            outVy = (targetY - cy) / dt;
        } else {
            outVx = 0.0f;
            outVy = 0.0f;
        }
    }
};
