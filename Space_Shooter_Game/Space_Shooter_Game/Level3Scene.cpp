#include "Level3Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"

Level3Scene::Level3Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 3) {
    SetBackgroundColor({0.03f, 0.12f, 0.03f});
    InitializeLevel();
}

void Level3Scene::InitializeLevel() {
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.1f, VIRTUAL_HEIGHT * 0.2f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.9f, VIRTUAL_HEIGHT * 0.2f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.25f, VIRTUAL_HEIGHT * 0.6f, 30));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.75f, VIRTUAL_HEIGHT * 0.6f, 30));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.25f, 70));
}