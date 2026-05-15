#include "Level4Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "LevelEnemyPool.h"

Level4Scene::Level4Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 4) {
    auto bgTexture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/sheen__0011_Background.png");
    SetBackgroundTexture(
        AssetManager::GetInstance().GetTexture(
            gfx,
            L"Assets/background_lv4.png"
        )
    );
    InitializeLevel();
}

void Level4Scene::InitializeLevel() {
    SetEnemyPhases(LevelEnemyPool::Create(m_levelIndex));

    // ExpOrb
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.2f, VIRTUAL_HEIGHT * 0.2f, 25));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.8f, VIRTUAL_HEIGHT * 0.2f, 25));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.3f, 80));
}
