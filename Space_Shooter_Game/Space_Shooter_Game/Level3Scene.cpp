#include "Level3Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "LevelEnemyPool.h"

Level3Scene::Level3Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 3) {
    SetBackgroundTexture(
        AssetManager::GetInstance().GetTexture(
            gfx,
            L"Assets/background_lv3.png"
        )
    );
    InitializeLevel();
}

void Level3Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));

    // ExpOrb
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.1f, VIRTUAL_HEIGHT * 0.2f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.9f, VIRTUAL_HEIGHT * 0.2f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.25f, 70));
}
