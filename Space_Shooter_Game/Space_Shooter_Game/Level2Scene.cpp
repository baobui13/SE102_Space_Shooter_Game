#include "Level2Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "EnemySpawner.h"

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
    // === Wave 1: 1 giây — enemy zigzag từ hai bên ===
    m_enemySpawner.AddWave(WaveDefinition(1.0f, {
        { EnemyType::Melee_Fast, 100.0f, -50.0f },
        { EnemyType::Melee_Fast, 500.0f, -50.0f },
    }));

    // === Wave 2: 8 giây — hỗn hợp ===
    m_enemySpawner.AddWave(WaveDefinition(8.0f, {
        { EnemyType::Melee_Basic, 200.0f, -50.0f },
        { EnemyType::Melee_Basic, 400.0f, -50.0f },
        { EnemyType::Melee_Fast,  300.0f, -80.0f },
        { EnemyType::Melee_Fast,  100.0f, -80.0f },
        { EnemyType::Melee_Fast,  500.0f, -80.0f },
    }));

    // === Wave 3: 15 giây — wave dày đặc ===
    m_enemySpawner.AddWave(WaveDefinition(15.0f, {
        { EnemyType::Melee_Basic, 80.0f,  -50.0f },
        { EnemyType::Melee_Fast,  180.0f, -50.0f },
        { EnemyType::Melee_Basic, 280.0f, -50.0f },
        { EnemyType::Melee_Fast,  380.0f, -50.0f },
        { EnemyType::Melee_Basic, 480.0f, -50.0f },
        { EnemyType::Melee_Fast,  580.0f, -50.0f },
    }));

    // === Wave 4: 25 giây — wave cuối ===
    m_enemySpawner.AddWave(WaveDefinition(25.0f, {
        { EnemyType::Melee_Fast,  100.0f, -50.0f },
        { EnemyType::Melee_Fast,  200.0f, -80.0f },
        { EnemyType::Melee_Fast,  300.0f, -50.0f },
        { EnemyType::Melee_Fast,  400.0f, -80.0f },
        { EnemyType::Melee_Fast,  500.0f, -50.0f },
        { EnemyType::Melee_Basic, 300.0f, -120.0f },
        { EnemyType::Melee_Basic, 150.0f, -120.0f },
        { EnemyType::Melee_Basic, 450.0f, -120.0f },
    }));

    // ExpOrb
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.2f, VIRTUAL_HEIGHT * 0.15f, 15));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.8f, VIRTUAL_HEIGHT * 0.15f, 15));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.2f, 60));
}