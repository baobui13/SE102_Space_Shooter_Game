#include "AttackMarkerPool.h"
#include <array>

namespace {
const std::array<AttackMarkerDefinition, 4> ATTACK_MARKERS = { {
    {
        AttackMarkerType::DangerLine,
        AttackMarkerShape::Line,
        L"Assets/WhitePoint.png",
        { 1.0f, 0.05f, 0.05f, 0.75f },
        3.0f,
        2.0f
    },
    {
        AttackMarkerType::DangerCircle,
        AttackMarkerShape::Circle,
        L"Assets/WhitePoint.png",
        { 1.0f, 0.1f, 0.05f, 0.8f },
        3.0f,
        3.0f
    },
    {
        AttackMarkerType::DangerRectangle,
        AttackMarkerShape::Rectangle,
        L"Assets/WhitePoint.png",
        { 1.0f, 0.15f, 0.05f, 0.7f },
        3.0f,
        3.0f
    },
    {
        AttackMarkerType::BossSkillCircle,
        AttackMarkerShape::Circle,
        L"Assets/WhitePoint.png",
        { 0.9f, 0.0f, 1.0f, 0.8f },
        4.0f,
        4.0f
    }
} };
}

const AttackMarkerDefinition& AttackMarkerPool::Get(AttackMarkerType type) {
    for (const auto& marker : ATTACK_MARKERS) {
        if (marker.type == type) {
            return marker;
        }
    }

    return ATTACK_MARKERS[0];
}
