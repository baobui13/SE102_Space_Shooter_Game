#include "Level2Scene.h"
#include "AssetManager.h"
#include "LevelConfig.h"
#include "LevelEnemyPool.h"

Level2Scene::Level2Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 2) {
    const std::wstring backgroundPath = LevelConfig::GetBackgroundTexturePath(m_levelIndex);
    if (!backgroundPath.empty()) {
        SetBackgroundTexture(AssetManager::GetInstance().GetTexture(gfx, backgroundPath));
    }
    InitializeLevel();
}

void Level2Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));
    LevelConfig::SpawnObjects(m_levelIndex, m_gfx, m_entityManager);
}
