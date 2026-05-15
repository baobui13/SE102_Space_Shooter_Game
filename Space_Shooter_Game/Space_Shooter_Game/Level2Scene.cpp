#include "Level2Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "LevelEnemyPool.h"

Level2Scene::Level2Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 2) {
    SetBackgroundTexture(
        AssetManager::GetInstance().GetTexture(
            gfx,
            L"Assets/background_lv2.png"
        )
    );
    InitializeLevel();
}

void Level2Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));

    // ExpOrb
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.2f, VIRTUAL_HEIGHT * 0.15f, 15));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.8f, VIRTUAL_HEIGHT * 0.15f, 15));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.2f, 60));
}
