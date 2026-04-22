#include "AudioManager.h"
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

constexpr float PLAYER_DASH_SPEED = 900.0f;
constexpr float PLAYER_DASH_DURATION = 0.18f;
constexpr int PLAYER_MAX_DASH_CHARGES = 2;
constexpr float PLAYER_DASH_RECHARGE_TIME = 1.5f;

constexpr float PLAYER_BULLET_OFFSET = 8.0f;
constexpr float PLAYER_BULLET_SPEED = 500.0f;
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
    , m_isDashing(false)
    , m_dashTimer(0.0f)
    , m_dashSpeed(PLAYER_DASH_SPEED)
    , m_dashDuration(PLAYER_DASH_DURATION)
    , m_dashCharges(PLAYER_MAX_DASH_CHARGES)
    , m_maxDashCharges(PLAYER_MAX_DASH_CHARGES)
    , m_dashRechargeTime(PLAYER_DASH_RECHARGE_TIME)
    , m_dashRechargeTimer(0.0f)
    , m_lastMoveDirX(0.0f)
    , m_lastMoveDirY(-1.0f)
    , m_dashDirX(0.0f)
    , m_dashDirY(0.0f)
    , m_skillManager() {
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Spaceship.png"));
    m_anim.AddClip("Idle", 0, 0, 500, 500, 1, 1, 1.0f, true);
    m_anim.Play("Idle");

    m_deathAnim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Explosion Animation.png"));
    m_deathAnim.AddClip("Die", 0, 0, 64, 64, 11, 6, 0.1f, false);
}

void Player::Update(float dt, GameContext& ctx) {
    if (!m_isActive) {
        return;
    }

    if (m_isDead) {
        m_deathAnim.Update(dt);
        if (m_deathAnim.IsFinished()) {
            Destroy(); // Thực sự biến mất sau khi nổ xong
        }
        return; // Thoát sớm, không cho di chuyển hay bắn đạn
    }

    // Giảm timer hồi phục theo thời gian
    if (m_invulTimer > 0.0f) {
        m_invulTimer -= dt;
        m_blinkTimer += dt;
    }
    else {
        m_blinkTimer = 0.0f; // Reset khi hết nháy
    }

    UpdateAttackCooldown(dt);
    UpdateDashRecharge(dt);

    float dirX = 0.0f;
    float dirY = 0.0f;
    bool hasMoveInput = false;
    ReadMovementInput(ctx.input, dirX, dirY, hasMoveInput);

    TryStartDash(ctx.input, dirX, dirY, hasMoveInput);

    if (m_isDashing) {
        UpdateDashMovement(dt);
    } else {
        UpdateAttack(ctx);
        ApplyMovement(dt, dirX, dirY);
    }

    ClampToScreen(ctx);
    m_anim.Update(dt);
    m_skillManager.Update(dt, ctx);
}

void Player::Render(Graphics& gfx) {
    if (!m_isActive) return; 
    
    if (m_isDead) {
        // Vẽ hiệu ứng nổ tại vị trí hiện tại của Player
        m_deathAnim.Render(gfx, m_x, m_y, m_width, m_height);
    }
    else {
        if (m_invulTimer > 0.0f) {
            // Tạo biến dao động từ 1.0f (bình thường) đến 3.0f (sáng rực)
            float brightness = 1.0f + (std::sin(m_invulTimer * 25.0f) + 1.0f) * 1.0f;

            // Tạo vector màu: R, G, B nhân với brightness, Alpha giữ nguyên là 1.0f
            DirectX::XMVECTOR flashColor = DirectX::XMVectorSet(brightness, brightness, brightness, 1.0f);

            m_anim.Render(gfx, m_x, m_y, m_width, m_height, flashColor);
        }
        else {
            m_anim.Render(gfx, m_x, m_y, m_width, m_height);
        }
    }
}

void Player::TakeDamage(int damage) {
    if (m_isShielded || m_isDashing || m_invulTimer > 0.0f || m_isDead) return;

    m_hp -= damage;
    if (m_hp <= 0) {
        m_hp = 0;
        m_isDead = true;          // Đánh dấu trạng thái chết
        m_deathAnim.Play("Die");   // Chạy hiệu ứng nổ
        AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerDeath);
        OutputDebugStringA("[Player] Bat dau hieu ung no!\n");
    }
    else {
        m_invulTimer = m_invulDuration;
        AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerHit);
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
    AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerLevelUp);
}

void Player::UpdateAttackCooldown(float dt) {
    if (m_attackTimer > 0.0f) {
        m_attackTimer -= dt;
    }
}

void Player::UpdateDashRecharge(float dt) {
    if (m_dashCharges >= m_maxDashCharges) {
        return;
    }

    m_dashRechargeTimer -= dt;
    if (m_dashRechargeTimer <= 0.0f) {
        m_dashCharges++;
        if (m_dashCharges < m_maxDashCharges) {
            m_dashRechargeTimer += m_dashRechargeTime;
        } else {
            m_dashRechargeTimer = 0.0f;
        }
    }
}

void Player::ReadMovementInput(const InputManager& input, float& dirX, float& dirY, bool& hasMoveInput) {
    const float horizontal =
        (input.IsKeyDown('D') || input.IsKeyDown(VK_RIGHT) ? 1.0f : 0.0f) -
        (input.IsKeyDown('A') || input.IsKeyDown(VK_LEFT) ? 1.0f : 0.0f);
    const float vertical =
        (input.IsKeyDown('S') || input.IsKeyDown(VK_DOWN) ? 1.0f : 0.0f) -
        (input.IsKeyDown('W') || input.IsKeyDown(VK_UP) ? 1.0f : 0.0f);

    dirX = horizontal;
    dirY = vertical;
    hasMoveInput = (dirX != 0.0f || dirY != 0.0f);

    if (!hasMoveInput) {
        return;
    }

    const float length = std::sqrt((dirX * dirX) + (dirY * dirY));
    dirX /= length;
    dirY /= length;

    m_lastMoveDirX = dirX;
    m_lastMoveDirY = dirY;
}

void Player::TryStartDash(const InputManager& input, float dirX, float dirY, bool hasMoveInput) {
    if (m_isDashing || !input.IsRightMouseClicked() || m_dashCharges <= 0) {
        return;
    }

    if (!hasMoveInput && m_lastMoveDirX == 0.0f && m_lastMoveDirY == 0.0f) {
        return;
    }

    m_isDashing = true;
    m_dashTimer = m_dashDuration;
    m_dashDirX = hasMoveInput ? dirX : m_lastMoveDirX;
    m_dashDirY = hasMoveInput ? dirY : m_lastMoveDirY;
    m_dashCharges--;
    AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerDash);

    if (m_dashCharges < m_maxDashCharges && m_dashRechargeTimer <= 0.0f) {
        m_dashRechargeTimer = m_dashRechargeTime;
    }
}

void Player::UpdateDashMovement(float dt) {
    m_x += m_dashDirX * m_dashSpeed * dt;
    m_y += m_dashDirY * m_dashSpeed * dt;

    m_dashTimer -= dt;
    if (m_dashTimer <= 0.0f) {
        m_isDashing = false;
        m_dashTimer = 0.0f;
    }
}

void Player::UpdateAttack(GameContext& ctx) {
    if (!ctx.input.IsLeftMouseDown() || m_attackTimer > 0.0f) {
        return;
    }

    const float mouseX = static_cast<float>(ctx.input.GetMouseX());
    const float mouseY = static_cast<float>(ctx.input.GetMouseY());

    const float spawnX = m_x + (m_width * 0.5f) - PLAYER_BULLET_OFFSET;
    const float spawnY = m_y + (m_height * 0.5f) - PLAYER_BULLET_OFFSET;

    ctx.bulletPool.GetBullet(
        spawnX,
        spawnY,
        mouseX,
        mouseY,
        PLAYER_BULLET_SPEED,
        m_attackDamage,
        m_attackRange
    );

    AudioManager::GetInstance().PlaySoundEffect(AudioIds::PlayerShoot, 0.8f);
    m_attackTimer = 1.0f / m_attackSpeed;
}

void Player::ApplyMovement(float dt, float dirX, float dirY) {
    if (dirX == 0.0f && dirY == 0.0f) {
        return;
    }

    m_x += dirX * m_speed * dt;
    m_y += dirY * m_speed * dt;
}

void Player::ClampToScreen(const GameContext& ctx) {
    if (m_x < 0.0f) m_x = 0.0f;
    if (m_y < 0.0f) m_y = 0.0f;
    if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
    if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;
}
