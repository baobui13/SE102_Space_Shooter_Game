#include "Bullet.h"
#include "GameContext.h"
#include "AssetManager.h"
#include <cmath>
#include "EntityManager.h"
#include "BaseEnemy.h"

Bullet::Bullet(Graphics& gfx)
    : GameObject(0, 0, 16.0f, 16.0f), m_damage(0), m_maxDistance(0), m_distanceTraveled(0)
{
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Bullets1.png"));
    m_anim.AddClip("Fly", 15, 15, 17, 17, 1, 1, 1.0f, true);
    m_anim.Play("Fly");
}

Bullet::Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance)
    : Bullet(gfx)
{
    ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance);
}

void Bullet::ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance) {
    m_x = startX;
    m_y = startY;
    m_damage = damage;
    m_maxDistance = maxDistance;
    m_distanceTraveled = 0.0f;
    m_isActive = true;

    float dx = targetX - startX;
    float dy = targetY - startY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance != 0.0f) {
        dx /= distance;
        dy /= distance;
    }
    else {
        dx = 0.0f;
        dy = 1.0f;
    }

    m_vx = dx * speed;
    m_vy = dy * speed;
}

void Bullet::Update(float dt, ::GameContext& ctx) {
    if (!m_isActive) return;

    // Kế thừa logic di chuyển mặc định từ GameObject (m_x += m_vx * dt)
    GameObject::Update(dt, ctx);

    // Tính toán khoảng cách đạn bay để xóa khi bay quá xa
    float moveDist = std::sqrt(m_vx * m_vx + m_vy * m_vy) * dt;
    m_distanceTraveled += moveDist;

    if (m_distanceTraveled >= m_maxDistance) {
        Destroy();
        return;
    }

    auto& allEntities = ctx.entityManager.GetEntities();
    for (auto& entity : allEntities) {
        // Bỏ qua các object đã chết hoặc bị vô hiệu hóa
        if (!entity->IsActive()) continue;

        // Ép kiểu để xem entity này có phải là Enemy hay không
        BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(entity.get());

        if (enemy) {
            // Hàm CheckCollision được kế thừa sẵn từ class GameObject
            if (this->CheckCollision(*enemy)) {

                // Trừ máu quái vật
                enemy->TakeDamage(m_damage);

                // Đạn trúng mục tiêu thì tự biến mất
                this->Destroy();

                // Thoát vòng lặp ngay vì đạn thường không xuyên thấu
                break;
            }
        }
    }
}
