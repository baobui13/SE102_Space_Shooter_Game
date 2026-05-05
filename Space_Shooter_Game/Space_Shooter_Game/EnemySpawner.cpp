#include "EnemySpawner.h"

void EnemySpawner::AddWave(WaveDefinition wave) {
    m_waves.push_back(std::move(wave));
}

void EnemySpawner::Update(float dt, Graphics& gfx, EntityManager& entityManager) {
    m_elapsedTime += dt;

    bool allDone = true;
    for (auto& wave : m_waves) {
        if (!wave.triggered && m_elapsedTime >= wave.triggerTime) {
            // Spawn tất cả enemy trong wave này
            for (auto& entry : wave.entries) {
                auto enemy = EnemyFactory::Create(entry.type, gfx, entry.x, entry.y);
                entityManager.AddEntity(std::move(enemy));
            }
            wave.triggered = true;
        }
        if (!wave.triggered) {
            allDone = false;
        }
    }
    m_allWavesComplete = allDone;
}

void EnemySpawner::Reset() {
    m_elapsedTime = 0.0f;
    m_allWavesComplete = false;
    for (auto& wave : m_waves) {
        wave.triggered = false;
    }
}

size_t EnemySpawner::GetTriggeredWaveCount() const {
    size_t count = 0;
    for (const auto& wave : m_waves) {
        if (wave.triggered) count++;
    }
    return count;
}
