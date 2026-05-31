#include "Level4Scene.h"
#include "AssetManager.h"
#include "LevelConfig.h"
#include "LevelEnemyPool.h"

Level4Scene::Level4Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 4) {
    const std::wstring backgroundPath = LevelConfig::GetBackgroundTexturePath(m_levelIndex);
    if (!backgroundPath.empty()) {
        SetBackgroundTexture(AssetManager::GetInstance().GetTexture(gfx, backgroundPath));
    }
    InitializeLevel();
}

void Level4Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));
    LevelConfig::SpawnObjects(m_levelIndex, m_gfx, m_entityManager);
}
