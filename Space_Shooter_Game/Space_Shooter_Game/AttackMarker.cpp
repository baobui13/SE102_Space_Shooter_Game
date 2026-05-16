#include "AttackMarker.h"
#include "AssetManager.h"
#include "AttackMarkerPool.h"
#include "GameContext.h"
#include "Player.h"
#include <DirectXColors.h>
#include <cmath>

namespace {
DirectX::XMVECTOR LoadColor(const DirectX::XMFLOAT4& color) {
    return DirectX::XMLoadFloat4(&color);
}
}

AttackMarker::AttackMarker(const AttackMarkerSpawnData& spawnData)
    : GameObject(spawnData.x, spawnData.y, spawnData.width, spawnData.height)
    , m_spawnData(spawnData)
{
}

void AttackMarker::Update(float dt, GameContext& ctx) {
    m_elapsed += dt;
    if (m_spawnData.followPlayer) {
        m_spawnData.targetX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
        m_spawnData.targetY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
    }

    if (m_elapsed >= m_spawnData.lifetime) {
        Destroy();
    }
}

void AttackMarker::Render(Graphics& gfx) {
    const auto& definition = AttackMarkerPool::Get(m_spawnData.type);

    switch (definition.shape) {
    case AttackMarkerShape::Line:
        RenderLine(gfx, definition);
        break;
    case AttackMarkerShape::Circle:
        RenderCircle(gfx, definition);
        break;
    case AttackMarkerShape::Rectangle:
        RenderRectangle(gfx, definition);
        break;
    }
}

void AttackMarker::RenderLine(Graphics& gfx, const AttackMarkerDefinition& definition) {
    auto pixel = AssetManager::GetInstance().GetTexture(gfx, definition.texturePath);
    gfx.DrawLine(
        pixel.Get(),
        m_spawnData.x,
        m_spawnData.y,
        m_spawnData.targetX,
        m_spawnData.targetY,
        definition.lineThickness,
        LoadColor(definition.color)
    );
}

void AttackMarker::RenderCircle(Graphics& gfx, const AttackMarkerDefinition& definition) {
    auto pixel = AssetManager::GetInstance().GetTexture(gfx, definition.texturePath);
    constexpr int SEGMENT_COUNT = 48;
    constexpr float TWO_PI = 6.283185307f;

    for (int i = 0; i < SEGMENT_COUNT; ++i) {
        float angleA = TWO_PI * (float)i / (float)SEGMENT_COUNT;
        float angleB = TWO_PI * (float)(i + 1) / (float)SEGMENT_COUNT;

        float x1 = m_spawnData.x + std::cos(angleA) * m_spawnData.radius;
        float y1 = m_spawnData.y + std::sin(angleA) * m_spawnData.radius;
        float x2 = m_spawnData.x + std::cos(angleB) * m_spawnData.radius;
        float y2 = m_spawnData.y + std::sin(angleB) * m_spawnData.radius;

        gfx.DrawLine(pixel.Get(), x1, y1, x2, y2, definition.outlineThickness, LoadColor(definition.color));
    }
}

void AttackMarker::RenderRectangle(Graphics& gfx, const AttackMarkerDefinition& definition) {
    auto pixel = AssetManager::GetInstance().GetTexture(gfx, definition.texturePath);
    float halfW = m_spawnData.width / 2.0f;
    float halfH = m_spawnData.height / 2.0f;
    float c = std::cos(m_spawnData.rotation);
    float s = std::sin(m_spawnData.rotation);

    auto rotateX = [&](float localX, float localY) {
        return m_spawnData.x + localX * c - localY * s;
    };

    auto rotateY = [&](float localX, float localY) {
        return m_spawnData.y + localX * s + localY * c;
    };

    float x1 = rotateX(-halfW, -halfH);
    float y1 = rotateY(-halfW, -halfH);
    float x2 = rotateX(halfW, -halfH);
    float y2 = rotateY(halfW, -halfH);
    float x3 = rotateX(halfW, halfH);
    float y3 = rotateY(halfW, halfH);
    float x4 = rotateX(-halfW, halfH);
    float y4 = rotateY(-halfW, halfH);

    auto color = LoadColor(definition.color);
    gfx.DrawLine(pixel.Get(), x1, y1, x2, y2, definition.outlineThickness, color);
    gfx.DrawLine(pixel.Get(), x2, y2, x3, y3, definition.outlineThickness, color);
    gfx.DrawLine(pixel.Get(), x3, y3, x4, y4, definition.outlineThickness, color);
    gfx.DrawLine(pixel.Get(), x4, y4, x1, y1, definition.outlineThickness, color);
}
