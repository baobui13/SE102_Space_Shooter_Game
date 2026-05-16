#pragma once
#include "GameObject.h"
#include <string>

enum class BulletOwner {
    Player,
    Enemy
};

class Bullet : public GameObject {
private:
    int m_damage;
    float m_maxDistance;
    float m_distanceTraveled;
    BulletOwner m_owner;

    float m_rotation = 0.0f;
    float m_spriteForwardAngle = 0.0f;

public:
    Bullet(Graphics& gfx);
    Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance);

    void ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance);
    void ReInitialize(float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance, BulletOwner owner);
    void SetDefaultAnimation(Graphics& gfx);
    void SetAnimation(Graphics& gfx, const wchar_t* texturePath, const std::string& clipName,
        int frameX, int frameY, int frameWidth, int frameHeight,
        int frameCount, int columns, float frameDuration, bool loop,
        int spacingX = 0, int spacingY = 0);
    void SetAnimation(Graphics& gfx, const wchar_t* texturePath, const std::string& clipName,
        int frameX, int frameY, int frameWidth, int frameHeight,
        int frameCount, int columns, float frameDuration, bool loop,
        float displayWidth, float displayHeight,
        int spacingX = 0, int spacingY = 0);

    void Update(float dt, ::GameContext& ctx) override;

    int GetDamage() const { return m_damage; }
    BulletOwner GetOwner() const { return m_owner; }
    
    void SetSpriteForwardAngle(float radians);
    float GetRotation() const { return m_rotation; }

    void Render(Graphics& gfx) override;
};
