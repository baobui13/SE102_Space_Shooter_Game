#include "Bullet.h"
#include "AssetManager.h"
#include "BaseEnemy.h"
#include "EntityManager.h"
#include "GameContext.h"
#include "Player.h"
#include <cmath>

Bullet::Bullet(Graphics& gfx)
    : GameObject(0, 0, 16.0f, 16.0f)
    , m_damage(0)
    , m_maxDistance(0.0f)
    , m_distanceTraveled(0.0f)
    , m_owner(BulletOwner::Player)
{
    SetDefaultAnimation(gfx);
}

Bullet::Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance)
    : Bullet(gfx)
{
    ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance);
}

void Bullet::ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance) {
    ReInitialize(startX, startY, targetX, targetY, speed, damage, maxDistance, BulletOwner::Player);
}

void Bullet::ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance, BulletOwner owner) {
    m_x = startX;
    m_y = startY;
    m_damage = damage;
    m_maxDistance = maxDistance;
    m_distanceTraveled = 0.0f;
    m_owner = owner;
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

void Bullet::SetDefaultAnimation(Graphics& gfx) {
    m_width = 16.0f;
    m_height = 16.0f;
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Bullets1.png"));
    m_anim.AddClip("Fly", 15, 15, 17, 17, 1, 1, 1.0f, true);
    m_anim.Play("Fly");
}

void Bullet::SetAnimation(Graphics& gfx, const wchar_t* texturePath, const std::string& clipName,
    int frameX, int frameY, int frameWidth, int frameHeight,
    int frameCount, int columns, float frameDuration, bool loop,
    int spacingX, int spacingY)
{
    SetAnimation(gfx, texturePath, clipName, frameX, frameY, frameWidth, frameHeight,
        frameCount, columns, frameDuration, loop, (float)frameWidth, (float)frameHeight, spacingX, spacingY);
}

void Bullet::SetAnimation(Graphics& gfx, const wchar_t* texturePath, const std::string& clipName,
    int frameX, int frameY, int frameWidth, int frameHeight,
    int frameCount, int columns, float frameDuration, bool loop,
    float displayWidth, float displayHeight,
    int spacingX, int spacingY)
{
    m_width = displayWidth;
    m_height = displayHeight;
    auto texture = AssetManager::GetInstance().GetTexture(gfx, texturePath);
    if (!texture) {
        SetDefaultAnimation(gfx);
        m_width = displayWidth;
        m_height = displayHeight;
        return;
    }

    m_anim.Initialize(texture);
    m_anim.AddClip(clipName, frameX, frameY, frameWidth, frameHeight, frameCount, columns, frameDuration, loop, spacingX, spacingY);
    m_anim.Play(clipName);
}

void Bullet::Update(float dt, ::GameContext& ctx) {
    if (!m_isActive) return;

    GameObject::Update(dt, ctx);

    float moveDist = std::sqrt(m_vx * m_vx + m_vy * m_vy) * dt;
    m_distanceTraveled += moveDist;

    if (m_distanceTraveled >= m_maxDistance) {
        Destroy();
        return;
    }

    if (m_owner == BulletOwner::Enemy) {
        if (CheckCollision(ctx.player)) {
            ctx.player.TakeDamage(m_damage);
            Destroy();
        }
        return;
    }

    auto& allEntities = ctx.entityManager.GetEntities();
    for (auto& entity : allEntities) {
        if (!entity->IsActive()) continue;

        BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(entity.get());
        if (enemy && CheckCollision(*enemy)) {
            enemy->TakeDamage(m_damage);
            Destroy();
            break;
        }
    }
}
