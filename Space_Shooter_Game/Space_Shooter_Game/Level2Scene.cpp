#include "Level2Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"

Level2Scene::Level2Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 2) {
    SetBackgroundColor({0.12f, 0.03f, 0.03f});
    InitializeLevel();
}

void Level2Scene::InitializeLevel() {
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.2f, VIRTUAL_HEIGHT * 0.15f, 15));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.8f, VIRTUAL_HEIGHT * 0.15f, 15));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.35f, VIRTUAL_HEIGHT * 0.55f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.65f, VIRTUAL_HEIGHT * 0.55f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.2f, 60));
}