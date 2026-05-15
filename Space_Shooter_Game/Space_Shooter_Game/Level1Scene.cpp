#include "Level1Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "LevelEnemyPool.h"

Level1Scene::Level1Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 1) {
    SetBackgroundColor({0.02f, 0.02f, 0.1f});
    InitializeLevel();
}

void Level1Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));

    // ExpOrb để thu thập kinh nghiệm
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH / 2.0f - 8.0f, VIRTUAL_HEIGHT / 2.0f - 8.0f, 50));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.125f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.875f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.1f, 50));
}
