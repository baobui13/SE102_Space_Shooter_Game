#pragma once
#include <DirectXMath.h>
#include <string>

enum class AttackMarkerType {
    DangerLine,
    DangerCircle,
    DangerRectangle,
    BossSkillCircle
};

enum class AttackMarkerShape {
    Line,
    Circle,
    Rectangle
};

struct AttackMarkerDefinition {
    AttackMarkerType type;
    AttackMarkerShape shape;
    std::wstring texturePath;
    DirectX::XMFLOAT4 color;
    float lineThickness;
    float outlineThickness;
};

struct AttackMarkerSpawnData {
    AttackMarkerType type;
    float x;
    float y;
    float targetX;
    float targetY;
    float width;
    float height;
    float radius;
    float rotation;
    float lifetime;
    bool followPlayer;

    static AttackMarkerSpawnData Line(
        AttackMarkerType markerType,
        float startX,
        float startY,
        float endX,
        float endY,
        float duration)
    {
        return { markerType, startX, startY, endX, endY, 0.0f, 0.0f, 0.0f, 0.0f, duration, false };
    }

    static AttackMarkerSpawnData FollowPlayerLine(
        AttackMarkerType markerType,
        float startX,
        float startY,
        float duration)
    {
        return { markerType, startX, startY, startX, startY, 0.0f, 0.0f, 0.0f, 0.0f, duration, true };
    }

    static AttackMarkerSpawnData Circle(
        AttackMarkerType markerType,
        float centerX,
        float centerY,
        float circleRadius,
        float duration)
    {
        return { markerType, centerX, centerY, centerX, centerY, 0.0f, 0.0f, circleRadius, 0.0f, duration, false };
    }

    static AttackMarkerSpawnData Rectangle(
        AttackMarkerType markerType,
        float centerX,
        float centerY,
        float rectWidth,
        float rectHeight,
        float rectRotation,
        float duration)
    {
        return { markerType, centerX, centerY, centerX, centerY, rectWidth, rectHeight, 0.0f, rectRotation, duration, false };
    }
};
