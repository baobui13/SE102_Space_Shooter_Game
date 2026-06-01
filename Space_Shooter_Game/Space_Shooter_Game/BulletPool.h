#pragma once
#include "Bullet.h"
#include "GameContext.h"
#include "Graphics.h"
#include <memory>
#include <vector>

class BulletPool {
private:
    std::vector<std::unique_ptr<Bullet>> m_pool;
    Graphics& m_gfx;
    size_t m_poolSize;

public:
    BulletPool(Graphics& gfx, size_t initialSize = 100) : m_gfx(gfx), m_poolSize(initialSize) {
        for (size_t i = 0; i < initialSize; ++i) {
            auto bullet = std::make_unique<Bullet>(m_gfx);
            bullet->Destroy();
            m_pool.push_back(std::move(bullet));
        }
    }

    Bullet* GetBullet(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance) {
        return GetBullet(startX, startY, targetX, targetY, speed, damage, maxDistance, BulletOwner::Player);
    }

    Bullet* GetEnemyBullet(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance) {
        return GetBullet(startX, startY, targetX, targetY, speed, damage, maxDistance, BulletOwner::Enemy);
    }

    Bullet* GetBullet(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance, BulletOwner owner) {
        for (auto& bullet : m_pool) {
            if (!bullet->IsActive()) {
                bullet->ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance, owner);
                bullet->SetDefaultAnimation();
                return bullet.get();
            }
        }

        auto newBullet = std::make_unique<Bullet>(m_gfx);
        newBullet->ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance, owner);
        newBullet->SetDefaultAnimation();
        Bullet* ptr = newBullet.get();
        m_pool.push_back(std::move(newBullet));
        return ptr;
    }

    void Update(float dt, ::GameContext& ctx) {
        for (auto& bullet : m_pool) {
            if (bullet->IsActive()) {
                bullet->Update(dt, ctx);
            }
        }
    }

    void Render(Graphics& gfx) {
        for (auto& bullet : m_pool) {
            if (bullet->IsActive()) {
                bullet->Render(gfx);
            }
        }
    }

    const std::vector<std::unique_ptr<Bullet>>& GetActiveBullets() const {
        return m_pool;
    }
};
