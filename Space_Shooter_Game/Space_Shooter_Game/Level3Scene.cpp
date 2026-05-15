#include "Level3Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "EnemySpawner.h"

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
    // === Wave 1: 2 giây — enemy nhanh từ hai bên ===
    m_enemySpawner.AddWave(WaveDefinition(2.0f, {
        { EnemyType::Melee_Fast, 50.0f,  -50.0f },
        { EnemyType::Melee_Fast, 550.0f, -50.0f },
        { EnemyType::Melee_Basic, 300.0f, -50.0f },
    }));

    // === Wave 2: 7 giây — nhiều enemy hơn ===
    m_enemySpawner.AddWave(WaveDefinition(7.0f, {
        { EnemyType::Melee_Basic, 100.0f, -50.0f },
        { EnemyType::Melee_Basic, 200.0f, -80.0f },
        { EnemyType::Melee_Fast,  300.0f, -50.0f },
        { EnemyType::Melee_Basic, 400.0f, -80.0f },
        { EnemyType::Melee_Basic, 500.0f, -50.0f },
    }));

    // === Wave 3: 14 giây — wave hỗn hợp lớn ===
    m_enemySpawner.AddWave(WaveDefinition(14.0f, {
        { EnemyType::Melee_Fast,  80.0f,  -50.0f },
        { EnemyType::Melee_Fast,  180.0f, -50.0f },
        { EnemyType::Melee_Basic, 280.0f, -80.0f },
        { EnemyType::Melee_Fast,  380.0f, -50.0f },
        { EnemyType::Melee_Fast,  480.0f, -50.0f },
        { EnemyType::Melee_Basic, 580.0f, -80.0f },
    }));

    // === Wave 4: 22 giây — wave cuối dày đặc ===
    m_enemySpawner.AddWave(WaveDefinition(22.0f, {
        { EnemyType::Melee_Basic, 50.0f,  -50.0f },
        { EnemyType::Melee_Fast,  130.0f, -80.0f },
        { EnemyType::Melee_Basic, 210.0f, -50.0f },
        { EnemyType::Melee_Fast,  290.0f, -80.0f },
        { EnemyType::Melee_Basic, 370.0f, -50.0f },
        { EnemyType::Melee_Fast,  450.0f, -80.0f },
        { EnemyType::Melee_Basic, 530.0f, -50.0f },
        { EnemyType::Melee_Fast,  300.0f, -120.0f },
    }));

    // ExpOrb
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.1f, VIRTUAL_HEIGHT * 0.2f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.9f, VIRTUAL_HEIGHT * 0.2f, 20));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.25f, 70));
}