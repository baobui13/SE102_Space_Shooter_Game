#pragma once
#include "BaseEnemy.h"
#include "EnemyDefinitions.h"
#include <vector>

class MeleeEnemy2 : public BaseEnemy {
public:
    MeleeEnemy2(float x, float y, float width, float height,
                float health, float moveSpeed, float attackPower, float attackRange,
                float attackSpeed = 1.0f, EnemyType type = EnemyType::Melee_Spawner);

    void Update(float dt, GameContext& ctx) override;
    void Attack(GameObject* target) override;
    void TakeDamage(float damage) override;
    bool IsPlayerInRange(GameObject* player) const;

    void SetSpawnEntries(std::vector<LevelEnemySpawnDefinition> entries);
    void RegisterSpawnEntry(LevelEnemySpawnDefinition entry);
    void SetPeriodicSpawnInterval(float seconds);
    void SetDeathSpawnCount(int count);

private:
    void SpawnEnemies(GameContext& ctx, float x, float y, int count, size_t& indexCursor);
    void TryPeriodicSpawn(float dt, GameContext& ctx);

    std::vector<LevelEnemySpawnDefinition> m_spawnEntries;
    float m_spawnTimer = 0.0f;
    float m_periodicSpawnInterval = 3.0f;
    int m_deathSpawnCount = 3;
    size_t m_periodicSpawnIndex = 0;
    size_t m_deathSpawnIndex = 0;
    bool m_pendingDeathSpawn = false;
    float m_deathSpawnX = 0.0f;
    float m_deathSpawnY = 0.0f;
    GameContext* m_lastCtx = nullptr;
};
