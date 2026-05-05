#include "Level1Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "EnemySpawner.h"

Level1Scene::Level1Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 1) {
    SetBackgroundColor({0.02f, 0.02f, 0.1f});
    InitializeLevel();
}

void Level1Scene::InitializeLevel() {
    // === Wave 1: 3 giây — 3 enemy cơ bản từ trên ===
    m_enemySpawner.AddWave(WaveDefinition(3.0f, {
        { EnemyType::Melee_Basic, 100.0f, -50.0f },
        { EnemyType::Melee_Basic, 300.0f, -50.0f },
        { EnemyType::Melee_Basic, 500.0f, -50.0f },
        { EnemyType::Ranged_Basic, 300.0f, -100.0f },
    }));

    // === Wave 2: 10 giây — thêm enemy nhanh zigzag ===
    m_enemySpawner.AddWave(WaveDefinition(10.0f, {
        { EnemyType::Ranged_Basic, 50.0f, -50.0f },
        { EnemyType::Ranged_Basic, 550.0f, -50.0f },
        { EnemyType::Melee_Fast, 150.0f, -50.0f },
        { EnemyType::Melee_Fast, 450.0f, -50.0f },
        { EnemyType::Melee_Basic, 300.0f, -80.0f },
    }));

    // === Wave 3: 20 giây — wave lớn ===
    m_enemySpawner.AddWave(WaveDefinition(20.0f, {
        { EnemyType::Melee_Basic, 50.0f,  -50.0f },
        { EnemyType::Melee_Basic, 150.0f, -50.0f },
        { EnemyType::Melee_Fast,  250.0f, -50.0f },
        { EnemyType::Melee_Fast,  350.0f, -50.0f },
        { EnemyType::Melee_Basic, 450.0f, -50.0f },
        { EnemyType::Melee_Basic, 550.0f, -50.0f },
        { EnemyType::Ranged_Basic, 50.0f, -50.0f },
        { EnemyType::Ranged_Basic, 550.0f, -50.0f },
    }));

    // ExpOrb để thu thập kinh nghiệm
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH / 2.0f - 8.0f, VIRTUAL_HEIGHT / 2.0f - 8.0f, 50));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.125f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.875f, VIRTUAL_HEIGHT * 0.1f, 10));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.1f, 50));
}