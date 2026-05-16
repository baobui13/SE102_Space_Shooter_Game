#pragma once
#include "AttackMarkerDefinitions.h"
#include "GameObject.h"

class AttackMarker : public GameObject {
public:
    explicit AttackMarker(const AttackMarkerSpawnData& spawnData);

    void Update(float dt, GameContext& ctx) override;
    void Render(Graphics& gfx) override;

private:
    AttackMarkerSpawnData m_spawnData;
    float m_elapsed = 0.0f;

    void RenderLine(Graphics& gfx, const AttackMarkerDefinition& definition);
    void RenderCircle(Graphics& gfx, const AttackMarkerDefinition& definition);
    void RenderRectangle(Graphics& gfx, const AttackMarkerDefinition& definition);
};
