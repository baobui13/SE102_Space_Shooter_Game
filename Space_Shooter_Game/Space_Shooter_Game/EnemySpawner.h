#pragma once
#include "BaseEnemy.h"
#include "EnemyFactory.h"
#include "EntityManager.h"
#include <vector>

// Một entry spawn: loại enemy + vị trí xuất hiện
struct SpawnEntry {
    EnemyType type;
    float x, y;

    SpawnEntry(EnemyType t, float spawnX, float spawnY)
        : type(t), x(spawnX), y(spawnY) {}
};

// Một wave: danh sách enemy + thời gian bắt đầu (giây kể từ đầu level)
struct WaveDefinition {
    float triggerTime;
    std::vector<SpawnEntry> entries;
    bool triggered = false;

    WaveDefinition(float time, std::vector<SpawnEntry> spawnEntries)
        : triggerTime(time), entries(std::move(spawnEntries)) {}
};

// Quản lý spawn enemy theo wave cho một level
class EnemySpawner {
private:
    std::vector<WaveDefinition> m_waves;
    float m_elapsedTime = 0.0f;
    bool m_allWavesComplete = false;

public:
    // Thêm một wave vào danh sách
    void AddWave(WaveDefinition wave);

    // Gọi mỗi frame — kiểm tra và spawn khi đến thời điểm
    void Update(float dt, Graphics& gfx, EntityManager& entityManager);

    // Kiểm tra tất cả wave đã spawn xong chưa
    bool IsAllWavesComplete() const { return m_allWavesComplete; }

    // Reset để chơi lại
    void Reset();

    // Lấy thời gian đã trôi qua
    float GetElapsedTime() const { return m_elapsedTime; }

    // Lấy số wave
    size_t GetWaveCount() const { return m_waves.size(); }

    // Lấy số wave đã trigger
    size_t GetTriggeredWaveCount() const;
};
