#include "Player.h"
#include "AssetManager.h"
#include "BulletPool.h"
#include "GameContext.h"
#include <cmath>

namespace {
constexpr float PLAYER_SIZE = 64.0f;
constexpr float PLAYER_BASE_SPEED = 300.0f;
constexpr int PLAYER_BASE_HP = 100;
constexpr int PLAYER_BASE_DAMAGE = 10;
constexpr float PLAYER_BASE_ATTACK_SPEED = 3.0f;
constexpr float PLAYER_BASE_ATTACK_RANGE = 250.0f;
constexpr int PLAYER_START_LEVEL = 1;
constexpr int PLAYER_START_EXP = 0;
constexpr int PLAYER_BASE_EXP_TO_NEXT_LEVEL = 100;
constexpr float PLAYER_BASE_MAGNET_RANGE = 100.0f;
constexpr float PLAYER_BASE_COOLDOWN_MULTIPLIER = 1.0f;
constexpr float PLAYER_BASE_SKILL_SIZE_MULTIPLIER = 1.0f;
}

Player::Player(Graphics& gfx, float startX, float startY)
    : GameObject(startX, startY, PLAYER_SIZE, PLAYER_SIZE)
    , m_speed(PLAYER_BASE_SPEED)
    , m_hp(PLAYER_BASE_HP)
    , m_maxHp(PLAYER_BASE_HP)
    , m_attackDamage(PLAYER_BASE_DAMAGE)
    , m_attackSpeed(PLAYER_BASE_ATTACK_SPEED)
    , m_attackTimer(0.0f)
    , m_attackRange(PLAYER_BASE_ATTACK_RANGE)
    , m_level(PLAYER_START_LEVEL)
    , m_currentExp(PLAYER_START_EXP)
    , m_expToNextLevel(PLAYER_BASE_EXP_TO_NEXT_LEVEL)
    , m_magnetRange(PLAYER_BASE_MAGNET_RANGE)
    , m_cooldownMultiplier(PLAYER_BASE_COOLDOWN_MULTIPLIER)
    , m_skillSizeMultiplier(PLAYER_BASE_SKILL_SIZE_MULTIPLIER)
    , m_isShielded(false)
    , m_skillManager() {
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Spaceship.png"));
    m_anim.AddClip("Idle", 0, 0, 500, 500, 1, 1, 1.0f, true);
    m_anim.Play("Idle");
}

void Player::Update(float dt, GameContext& ctx) {
    if (!m_isActive) {
        return;
    }

    if (m_attackTimer > 0.0f) {
        m_attackTimer -= dt;
    }

    if (ctx.input.IsLeftMouseDown() && m_attackTimer <= 0.0f) {
        float mouseX = static_cast<float>(ctx.input.GetMouseX());
        float mouseY = static_cast<float>(ctx.input.GetMouseY());

        float spawnX = m_x + (m_width * 0.5f) - 8.0f;
        float spawnY = m_y + (m_height * 0.5f) - 8.0f;

        float bulletSpeed = 500.0f;
        float finalMaxDistance = m_attackRange;

        ctx.bulletPool.GetBullet(
            spawnX,
            spawnY,
            mouseX,
            mouseY,
            bulletSpeed,
            m_attackDamage,
            finalMaxDistance
        );

        m_attackTimer = 1.0f / m_attackSpeed;
    }

    float dirX = 0.0f;
    float dirY = 0.0f;

    if (ctx.input.IsKeyDown('W') || ctx.input.IsKeyDown(VK_UP)) dirY -= 1.0f;
    if (ctx.input.IsKeyDown('S') || ctx.input.IsKeyDown(VK_DOWN)) dirY += 1.0f;
    if (ctx.input.IsKeyDown('A') || ctx.input.IsKeyDown(VK_LEFT)) dirX -= 1.0f;
    if (ctx.input.IsKeyDown('D') || ctx.input.IsKeyDown(VK_RIGHT)) dirX += 1.0f;

    if (dirX != 0.0f || dirY != 0.0f) {
        float length = std::sqrt(dirX * dirX + dirY * dirY);
        dirX /= length;
        dirY /= length;

        m_x += dirX * m_speed * dt;
        m_y += dirY * m_speed * dt;
    }

    if (m_x < 0.0f) m_x = 0.0f;
    if (m_y < 0.0f) m_y = 0.0f;
    if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
    if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;

    m_anim.Update(dt);
    m_skillManager.Update(dt, ctx);
}

void Player::Render(Graphics& gfx) {
    GameObject::Render(gfx);
}

void Player::TakeDamage(int damage) {
    if (m_isShielded) {
        return;
    }

    m_hp -= damage;
    if (m_hp <= 0) {
        m_hp = 0;
        Destroy();
        OutputDebugStringA("[Player] Nhan vat da chet!\n");
    }
}

void Player::GainExp(int amount) {
    m_currentExp += amount;

    std::string msg = "[Player] Nhan duoc " + std::to_string(amount) + " EXP!\n";
    OutputDebugStringA(msg.c_str());

    while (m_currentExp >= m_expToNextLevel) {
        LevelUp();
    }
}

void Player::LevelUp() {
    m_level++;
    m_expToNextLevel = static_cast<int>(m_expToNextLevel * 1.3f);
    m_upgradePoints++;
}
