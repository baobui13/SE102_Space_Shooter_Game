#include "Level3Scene.h"
#include "AssetManager.h"
#include "LevelConfig.h"
#include "LevelEnemyPool.h"

Level3Scene::Level3Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 3) {
    const std::wstring backgroundPath = LevelConfig::GetBackgroundTexturePath(m_levelIndex);
    if (!backgroundPath.empty()) {
        SetBackgroundTexture(AssetManager::GetInstance().GetTexture(gfx, backgroundPath));
    }
    InitializeLevel();
}

void Level3Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));
    LevelConfig::SpawnObjects(m_levelIndex, m_gfx, m_entityManager);
}
