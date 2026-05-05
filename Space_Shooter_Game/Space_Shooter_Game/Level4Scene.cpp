#include "Level4Scene.h"
#include "AssetManager.h"
#include "ExpOrb.h"
#include "EnemySpawner.h"

Level4Scene::Level4Scene(Graphics& gfx)
    : BaseGameplayScene(gfx, 4) {
    auto bgTexture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/sheen__0011_Background.png");
    SetBackgroundTexture(bgTexture);
    InitializeLevel();
}

void Level4Scene::InitializeLevel() {
    // === Wave 1: 1 giây — bắt đầu ngay ===
    m_enemySpawner.AddWave(WaveDefinition(1.0f, {
        { EnemyType::Melee_Fast,  100.0f, -50.0f },
        { EnemyType::Melee_Fast,  300.0f, -50.0f },
        { EnemyType::Melee_Fast,  500.0f, -50.0f },
    }));

    // === Wave 2: 6 giây ===
    m_enemySpawner.AddWave(WaveDefinition(6.0f, {
        { EnemyType::Melee_Basic, 100.0f, -50.0f },
        { EnemyType::Melee_Basic, 200.0f, -80.0f },
        { EnemyType::Melee_Fast,  300.0f, -50.0f },
        { EnemyType::Melee_Basic, 400.0f, -80.0f },
        { EnemyType::Melee_Basic, 500.0f, -50.0f },
        { EnemyType::Melee_Fast,  150.0f, -110.0f },
        { EnemyType::Melee_Fast,  450.0f, -110.0f },
    }));

    // === Wave 3: 12 giây — wave lớn ===
    m_enemySpawner.AddWave(WaveDefinition(12.0f, {
        { EnemyType::Melee_Fast,  50.0f,  -50.0f },
        { EnemyType::Melee_Basic, 130.0f, -50.0f },
        { EnemyType::Melee_Fast,  210.0f, -80.0f },
        { EnemyType::Melee_Basic, 290.0f, -50.0f },
        { EnemyType::Melee_Fast,  370.0f, -80.0f },
        { EnemyType::Melee_Basic, 450.0f, -50.0f },
        { EnemyType::Melee_Fast,  530.0f, -50.0f },
    }));

    // === Wave 4: 20 giây — wave cuối khốc liệt ===
    m_enemySpawner.AddWave(WaveDefinition(20.0f, {
        { EnemyType::Melee_Fast,  60.0f,  -50.0f },
        { EnemyType::Melee_Fast,  140.0f, -80.0f },
        { EnemyType::Melee_Fast,  220.0f, -50.0f },
        { EnemyType::Melee_Basic, 300.0f, -120.0f },
        { EnemyType::Melee_Fast,  380.0f, -50.0f },
        { EnemyType::Melee_Fast,  460.0f, -80.0f },
        { EnemyType::Melee_Fast,  540.0f, -50.0f },
        { EnemyType::Melee_Basic, 200.0f, -150.0f },
        { EnemyType::Melee_Basic, 400.0f, -150.0f },
    }));

    // === Wave 5: 28 giây — bonus wave ===
    m_enemySpawner.AddWave(WaveDefinition(28.0f, {
        { EnemyType::Melee_Basic, 100.0f, -50.0f },
        { EnemyType::Melee_Basic, 200.0f, -50.0f },
        { EnemyType::Melee_Basic, 300.0f, -50.0f },
        { EnemyType::Melee_Basic, 400.0f, -50.0f },
        { EnemyType::Melee_Basic, 500.0f, -50.0f },
        { EnemyType::Melee_Fast,  150.0f, -100.0f },
        { EnemyType::Melee_Fast,  250.0f, -100.0f },
        { EnemyType::Melee_Fast,  350.0f, -100.0f },
        { EnemyType::Melee_Fast,  450.0f, -100.0f },
    }));

    // ExpOrb
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.2f, VIRTUAL_HEIGHT * 0.2f, 25));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.8f, VIRTUAL_HEIGHT * 0.2f, 25));
    m_entityManager.AddEntity(std::make_unique<ExpOrb>(m_gfx, VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.3f, 80));
}