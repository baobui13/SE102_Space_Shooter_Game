#pragma once
#include "EnemyDefinitions.h"
#include "EnemyFactory.h"
#include "EntityManager.h"
#include <utility>
#include <vector>

using SpawnEntry = LevelEnemySpawnDefinition;
using WaveDefinition = EnemyPhaseDefinition;

// Quản lý spawn enemy theo wave cho một level
class EnemySpawner {
private:
    std::vector<EnemyPhaseDefinition> m_phases;
    float m_elapsedTime = 0.0f;
    bool m_allWavesComplete = false;

public:
    // Thêm một wave vào danh sách
    void AddWave(EnemyPhaseDefinition wave);
    void AddPhase(EnemyPhaseDefinition phase);
    void LoadPhases(std::vector<EnemyPhaseDefinition> phases);

    // Gọi mỗi frame — kiểm tra và spawn khi đến thời điểm
    void Update(float dt, Graphics& gfx, EntityManager& entityManager);

    // Kiểm tra tất cả wave đã spawn xong chưa
    bool IsAllWavesComplete() const { return m_allWavesComplete; }

    // Reset để chơi lại
    void Reset();

    // Lấy thời gian đã trôi qua
    float GetElapsedTime() const { return m_elapsedTime; }

    // Lấy số wave
    size_t GetWaveCount() const { return m_phases.size(); }

    // Lấy số wave đã trigger
    size_t GetTriggeredWaveCount() const;
};
