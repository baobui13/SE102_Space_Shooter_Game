#include "Level1Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"

Level1Scene::Level1Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 1) {
    SetBackgroundColor({0.02f, 0.02f, 0.1f});
}

void Level1Scene::InitializeLevel() {
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.125f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.875f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.125f, VIRTUAL_HEIGHT * 0.5f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.875f, VIRTUAL_HEIGHT * 0.5f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.1f, 50));
}