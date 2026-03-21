#include "Player.h"
#include "AssetManager.h"
#include <cmath> // Dùng cho hàm sqrt (tính căn bậc 2)

Player::Player(Graphics& gfx, float startX, float startY)
// Gọi Constructor của class cha (GameObject) để thiết lập tọa độ và kích thước (64x64)
    : GameObject(startX, startY, 64.0f, 64.0f),
    m_speed(300.0f),
    m_maxHp(100), m_hp(100),                // Khởi đầu với 100/100 HP
    m_attackDamage(10),                     // Sát thương khởi điểm
    m_attackSpeed(3.0f),                    // Tốc độ đánh: 3 viên / 1 giây
	m_attackRange(250.0f),                  // Tầm tấn công
    m_attackTimer(0.0f),                    // Không cần chờ ở phát bắn đầu tiên
    m_level(1), m_exp(0), m_expToNextLevel(100) // Khởi đầu lv 1, cần 100 exp để lên lv 2
{
    // Cài đặt Animation cho Player
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Spaceship.png"));
    m_anim.AddClip("Idle", 0, 0, 500, 500, 1, 1, 1.0f, true);
    m_anim.Play("Idle");
}

void Player::Update(float dt) {

}

void Player::Update(float dt, InputManager& input, std::vector<std::unique_ptr<Bullet>>& bullets, Graphics& gfx) {
    // -----------------------------------------
    // 1. LOGIC TẤN CÔNG (ATTACK SPEED & TIMER)
    // -----------------------------------------

    // Đếm lùi thời gian chờ đạn
    if (m_attackTimer > 0.0f) {
        m_attackTimer -= dt;
    }

    // Nếu NHẤN GIỮ chuột trái và ĐÃ HẾT thời gian chờ
    if (input.IsLeftMouseDown() && m_attackTimer <= 0.0f) {

        // Lấy tọa độ chuột từ InputManager
        float mouseX = (float)input.GetMouseX();
        float mouseY = (float)input.GetMouseY();

        // Tính toán vị trí nòng súng (Cho đạn bay ra từ chính giữa Player)
        // Kích thước Player là 64x64, kích thước Đạn là 16x16
        float spawnX = m_x + (m_width / 2.0f) - 8.0f;
        float spawnY = m_y + (m_height / 2.0f) - 8.0f;

        // Tốc độ đạn bay: 500 px/giây
        float bulletSpeed = 500.0f;

        // -------------------------------------------------------------
        // TÍNH TOÁN TẦM BAY THỰC TẾ = TẦM BẮN PLAYER * TỶ SỐ ĐỘ XA ĐẠN
        // -------------------------------------------------------------
        float bulletDistanceRatio = 1.0f;
        float finalMaxDistance = m_attackRange * bulletDistanceRatio;

        // Truyền finalMaxDistance vào viên đạn
        bullets.push_back(std::make_unique<Bullet>(gfx, spawnX, spawnY, mouseX, mouseY, bulletSpeed, m_attackDamage, finalMaxDistance));

        // Đặt lại thời gian chờ dựa trên Attack Speed (Công thức: 1.0 / Số đòn mỗi giây)
        m_attackTimer = 1.0f / m_attackSpeed;
    }

    // -----------------------------------------
    // 2. LOGIC DI CHUYỂN
    // -----------------------------------------

    // 1. Khởi tạo vector hướng di chuyển
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (input.IsKeyDown('W') || input.IsKeyDown(VK_UP)) dirY -= 1.0f;
    if (input.IsKeyDown('S') || input.IsKeyDown(VK_DOWN)) dirY += 1.0f;
    if (input.IsKeyDown('A') || input.IsKeyDown(VK_LEFT)) dirX -= 1.0f;
    if (input.IsKeyDown('D') || input.IsKeyDown(VK_RIGHT)) dirX += 1.0f;

    // 2. Chuẩn hóa vector hướng (Ngăn chặn việc bay chéo nhanh hơn bay thẳng)
    if (dirX != 0.0f || dirY != 0.0f) {
        float length = std::sqrt(dirX * dirX + dirY * dirY);
        dirX /= length;
        dirY /= length;

        // Cập nhật tọa độ
        m_x += dirX * m_speed * dt;
        m_y += dirY * m_speed * dt;
    }

    // -----------------------------------------
    // 3. CẬP NHẬT HOẠT ẢNH & KIỂM TRA MÀN HÌNH
    // -----------------------------------------

    // 3. Giới hạn không cho bay ra khỏi màn hình (cửa sổ 800x1000)
    if (m_x < 0) m_x = 0;
    if (m_y < 0) m_y = 0;
    if (m_x > 800.0f - m_width) m_x = 800.0f - m_width;
    if (m_y > 1000.0f - m_height) m_y = 1000.0f - m_height;

    // 4. Cập nhật hoạt ảnh
    m_anim.Update(dt);
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
    m_exp += amount;

    std::string msg = "[Player] Nhan duoc " + std::to_string(amount) + " EXP!\n";
    OutputDebugStringA(msg.c_str());

    // Dùng vòng lặp while để xử lý trường hợp nhận quá nhiều exp thăng nhiều cấp 1 lúc
    while (m_exp >= m_expToNextLevel) {
        LevelUp();
    }
}

void Player::LevelUp() {
    m_level++;
    m_exp -= m_expToNextLevel; // Khấu trừ exp đã dùng để lên cấp
    m_expToNextLevel = (int)(m_expToNextLevel * 1.5f); // Cấp sau cần nhiều exp hơn cấp trước 50%

    // Tăng chỉ số
    m_maxHp += 20;
    m_hp = m_maxHp; // Hồi đầy máu
    m_attackDamage += 5;

    // Ví dụ tăng tốc độ đánh thêm 0.2 viên/giây mỗi khi lên cấp
    m_attackSpeed += 0.2f;

    std::string msg = "[Player] LEN CAP " + std::to_string(m_level) + "!!!\n";
    OutputDebugStringA(msg.c_str());
}