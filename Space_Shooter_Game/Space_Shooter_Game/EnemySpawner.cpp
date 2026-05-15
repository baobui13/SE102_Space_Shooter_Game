#include "EnemySpawner.h"

void EnemySpawner::AddWave(EnemyPhaseDefinition wave) {
    AddPhase(std::move(wave));
}

void EnemySpawner::AddPhase(EnemyPhaseDefinition phase) {
    m_phases.push_back(std::move(phase));
}

void EnemySpawner::LoadPhases(std::vector<EnemyPhaseDefinition> phases) {
    m_phases = std::move(phases);
    Reset();
}

void EnemySpawner::Update(float dt, Graphics& gfx, EntityManager& entityManager) {
    m_elapsedTime += dt;

    bool allDone = true;
    for (auto& phase : m_phases) {
        if (!phase.triggered && m_elapsedTime >= phase.triggerTime) {
            // Spawn tất cả enemy trong phase này
            for (auto& entry : phase.enemies) {
                auto enemy = EnemyFactory::Create(entry, gfx);
                entityManager.AddEntity(std::move(enemy));
            }
            phase.triggered = true;
        }
        if (!phase.triggered) {
            allDone = false;
        }
    }
    m_allWavesComplete = allDone;
}

void EnemySpawner::Reset() {
    m_elapsedTime = 0.0f;
    m_allWavesComplete = false;
    for (auto& phase : m_phases) {
        phase.triggered = false;
    }
}

size_t EnemySpawner::GetTriggeredWaveCount() const {
    size_t count = 0;
    for (const auto& phase : m_phases) {
        if (phase.triggered) count++;
    }
    return count;
}
