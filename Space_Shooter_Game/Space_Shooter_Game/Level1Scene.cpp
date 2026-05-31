#include "Level1Scene.h"
#include "AssetManager.h"
#include "LevelConfig.h"
#include "LevelEnemyPool.h"

Level1Scene::Level1Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 1) {
    const std::wstring backgroundPath = LevelConfig::GetBackgroundTexturePath(m_levelIndex);
    if (!backgroundPath.empty()) {
        SetBackgroundTexture(AssetManager::GetInstance().GetTexture(gfx, backgroundPath));
    } else {
        SetBackgroundColor({ 0.02f, 0.02f, 0.1f });
    }
    InitializeLevel();
}

void Level1Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));
    LevelConfig::SpawnObjects(m_levelIndex, m_gfx, m_entityManager);
}
