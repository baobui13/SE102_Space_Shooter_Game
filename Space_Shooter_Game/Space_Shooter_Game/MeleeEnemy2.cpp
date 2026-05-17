#include "MeleeEnemy2.h"
#include "EnemyFactory.h"
#include "EntityManager.h"
#include "GameContext.h"
#include "Player.h"

namespace {
LevelEnemySpawnDefinition DefaultSpawnEntry() {
    return {
        EnemyType::Melee_Fast,
        0.0f,
        0.0f,
        { 50.0f, 150.0f, 5.0f, 1.0f, 20.0f, 15 },
        EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(150.0f))
    };
}
}

MeleeEnemy2::MeleeEnemy2(float x, float y, float width, float height,
                         float health, float moveSpeed, float attackPower, float attackRange,
                         float attackSpeed, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
    m_spawnEntries.push_back(DefaultSpawnEntry());
}

void MeleeEnemy2::SetSpawnEntries(std::vector<LevelEnemySpawnDefinition> entries) {
    if (!entries.empty()) {
        m_spawnEntries = std::move(entries);
        m_periodicSpawnIndex = 0;
        m_deathSpawnIndex = 0;
    }
}

void MeleeEnemy2::RegisterSpawnEntry(LevelEnemySpawnDefinition entry) {
    m_spawnEntries.push_back(std::move(entry));
}

void MeleeEnemy2::SetPeriodicSpawnInterval(float seconds) {
    if (seconds > 0.0f) {
        m_periodicSpawnInterval = seconds;
    }
}

void MeleeEnemy2::SetDeathSpawnCount(int count) {
    if (count > 0) {
        m_deathSpawnCount = count;
    }
}

void MeleeEnemy2::SpawnEnemies(GameContext& ctx, float x, float y, int count, size_t& indexCursor) {
    if (m_spawnEntries.empty() || count <= 0) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        LevelEnemySpawnDefinition spawnDef = m_spawnEntries[indexCursor % m_spawnEntries.size()];
        indexCursor++;

        spawnDef.x = x;
        spawnDef.y = y;

        auto enemy = EnemyFactory::Create(spawnDef, ctx.gfx);
        ctx.entityManager.AddEntity(std::move(enemy));
    }
}

void MeleeEnemy2::TryPeriodicSpawn(float dt, GameContext& ctx) {
    m_spawnTimer += dt;
    if (m_spawnTimer < m_periodicSpawnInterval) {
        return;
    }

    m_spawnTimer = 0.0f;
    SpawnEnemies(ctx, m_x, m_y, 1, m_periodicSpawnIndex);
}

void MeleeEnemy2::Update(float dt, GameContext& ctx) {
    m_lastCtx = &ctx;

    if (m_pendingDeathSpawn) {
        SpawnEnemies(ctx, m_deathSpawnX, m_deathSpawnY, m_deathSpawnCount, m_deathSpawnIndex);
        m_pendingDeathSpawn = false;
        return;
    }

    if (!m_isActive) return;

    UpdateAnimation(dt);
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    float prevX = m_x;
    float prevY = m_y;

    Move(dt, ctx);
    TryPeriodicSpawn(dt, ctx);

    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;

    FacePoint(playerCX, playerCY);

    if (CheckCollision(ctx.player)) {
        m_x = prevX;
        m_y = prevY;
        m_vx = 0.0f;
        m_vy = 0.0f;
    }

    if (IsPlayerInRange(&ctx.player)) {
        Attack(&ctx.player);
    }
}

void MeleeEnemy2::TakeDamage(float damage) {
    bool willDie = m_health - damage <= 0.0f;
    if (willDie) {
        if (m_lastCtx) {
            SpawnEnemies(*m_lastCtx, m_x, m_y, m_deathSpawnCount, m_deathSpawnIndex);
        }
        else {
            m_pendingDeathSpawn = true;
            m_deathSpawnX = m_x;
            m_deathSpawnY = m_y;
        }
    }
    BaseEnemy::TakeDamage(damage);
}

void MeleeEnemy2::Attack(GameObject* target) {
    if (m_attackCooldown <= 0 && target) {
        Player* player = dynamic_cast<Player*>(target);
        if (player) {
            player->TakeDamage((int)m_attackPower);
            m_isAttacking = true;
            m_attackCooldown = 1.0f / m_attackSpeed;
        }
    }
}

bool MeleeEnemy2::IsPlayerInRange(GameObject* player) const {
    if (!player) return false;

    float margin = 4.0f;
    bool overlapX = (m_x - margin) < (player->GetX() + player->GetWidth()) &&
                    (m_x + m_width + margin) > player->GetX();
    bool overlapY = (m_y - margin) < (player->GetY() + player->GetHeight()) &&
                    (m_y + m_height + margin) > player->GetY();
    return overlapX && overlapY;
}
