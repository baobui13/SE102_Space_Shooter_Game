#pragma once
#include <vector>
#include <memory>
#include "Bullet.h"
#include "Graphics.h"
#include "GameContext.h"

class BulletPool {
private:
    std::vector<std::unique_ptr<Bullet>> m_pool;
    Graphics& m_gfx;
    size_t m_poolSize;

public:
    BulletPool(Graphics& gfx, size_t initialSize = 100) : m_gfx(gfx), m_poolSize(initialSize) {
        for (size_t i = 0; i < initialSize; ++i) {
            auto bullet = std::make_unique<Bullet>(m_gfx);
            bullet->Destroy(); // Mặc định là Deactive
            m_pool.push_back(std::move(bullet));
        }
    }

    Bullet* GetBullet(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance) {
        for (auto& bullet : m_pool) {
            if (!bullet->IsActive()) {
                bullet->ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance);
                return bullet.get();
            }
        }
        
        // Nếu hết đạn trong pool, tạo mới và thêm vào pool (tự động mở rộng)
        auto newBullet = std::make_unique<Bullet>(m_gfx);
        newBullet->ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance);
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
