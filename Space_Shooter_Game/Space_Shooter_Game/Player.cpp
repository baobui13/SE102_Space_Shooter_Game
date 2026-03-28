#include "Player.h"
#include "GameContext.h"
#include "BulletPool.h"
#include "AssetManager.h"
#include <cmath> // Dùng cho hàm sqrt (tính căn bậc 2)

Player::Player(Graphics& gfx, float startX, float startY)
// Gọi Constructor của class cha (GameObject) để thiết lập tọa độ và kích thước (64x64)
    : GameObject(startX, startY, 64.0f, 64.0f),
    m_skillManager(),
    m_speed(300.0f),
    m_maxHp(100), 
    m_hp(100),                // Khởi đầu với 100/100 HP
    m_attackDamage(10),                     // Sát thương khởi điểm
    m_attackSpeed(3.0f),                    // Tốc độ đánh: 3 viên / 1 giây
	m_attackRange(250.0f),                  // Tầm tấn công
    m_attackTimer(0.0f),                    // Không cần chờ ở phát bắn đầu tiên
    m_level(1), 
    m_currentExp(0),
    m_expToNextLevel(100),                  // Khởi đầu lv 1, cần 100 exp để lên lv 2
    m_magnetRange(100.0f)
{
    // Cài đặt Animation cho Player
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Spaceship.png"));
    m_anim.AddClip("Idle", 0, 0, 500, 500, 1, 1, 1.0f, true);
    m_anim.Play("Idle");
}

void Player::Update(float dt, ::GameContext& ctx) {
    if (!m_isActive) return;

    // -----------------------------------------
    // 1. LOGIC TẤN CÔNG (ATTACK SPEED & TIMER)
    // -----------------------------------------

    if (m_attackTimer > 0.0f) {
        m_attackTimer -= dt;
    }

    if (ctx.input.IsLeftMouseDown() && m_attackTimer <= 0.0f) {
        float mouseX = (float)ctx.input.GetMouseX();
        float mouseY = (float)ctx.input.GetMouseY();

        float spawnX = m_x + (m_width / 2.0f) - 8.0f;
        float spawnY = m_y + (m_height / 2.0f) - 8.0f;

        float bulletSpeed = 500.0f;
        float bulletDistanceRatio = 1.0f;
        float finalMaxDistance = m_attackRange * bulletDistanceRatio;

        // Lấy đạn từ Pool thay vì tạo mới
        ctx.bulletPool.GetBullet(spawnX, spawnY, mouseX, mouseY, bulletSpeed, m_attackDamage, finalMaxDistance);

        m_attackTimer = 1.0f / m_attackSpeed;
    }

    // -----------------------------------------
    // 2. LOGIC DI CHUYỂN
    // -----------------------------------------

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

    // -----------------------------------------
    // 3. GIỚI HẠN MÀN HÌNH & CẬP NHẬT ANIM
    // -----------------------------------------

    if (m_x < 0) m_x = 0;
    if (m_y < 0) m_y = 0;
    if (m_x > ctx.screenWidth - m_width) m_x = ctx.screenWidth - m_width;
    if (m_y > ctx.screenHeight - m_height) m_y = ctx.screenHeight - m_height;

    m_anim.Update(dt);

    m_skillManager.Update(dt, ctx);
}

void Player::Render(Graphics& gfx) {
    // Vẽ bản thân Player trước (bằng hàm Render của GameObject)
    GameObject::Render(gfx);
}

// -----------------------------------------
// CÁC HÀM XỬ LÝ CHỈ SỐ (MÁU, KINH NGHIỆM)
// -----------------------------------------

void Player::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp <= 0) {
        m_hp = 0;
        Destroy(); // Đánh dấu m_isActive = false
        OutputDebugStringA("[Player] Nhan vat da chet!\n");
    }
}

void Player::GainExp(int amount) {
    m_currentExp += amount;

    std::string msg = "[Player] Nhan duoc " + std::to_string(amount) + " EXP!\n";
    OutputDebugStringA(msg.c_str());

    // Dùng vòng lặp while để xử lý trường hợp nhận quá nhiều exp thăng nhiều cấp 1 lúc
    while (m_currentExp >= m_expToNextLevel) {
        LevelUp();
    }
}

void Player::LevelUp() {
    m_level++;
    m_expToNextLevel = (int)(m_expToNextLevel * 1.3f);

    // Tích lũy 1 lượt nâng cấp
    m_upgradePoints++;
}