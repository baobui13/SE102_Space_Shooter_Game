#include "Player.h"
#include "AssetManager.h"
#include <cmath> // Dùng cho hàm sqrt (tính căn bậc 2)

Player::Player(Graphics& gfx, float startX, float startY)
// Gọi Constructor của class cha (GameObject) để thiết lập tọa độ và kích thước (64x64)
    : GameObject(startX, startY, 64.0f, 64.0f), m_speed(300.0f)
{
    // Cài đặt Animation cho Player
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Spaceship.png"));
    m_anim.AddClip("Idle", 0, 0, 500, 500, 1, 1, 1.0f, true);
    m_anim.Play("Idle");
}

void Player::Update(float dt) {

}

void Player::Update(float dt, InputManager& input) {
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

    // 3. Giới hạn không cho bay ra khỏi màn hình (cửa sổ 800x1000)
    if (m_x < 0) m_x = 0;
    if (m_y < 0) m_y = 0;
    if (m_x > 800.0f - m_width) m_x = 800.0f - m_width;
    if (m_y > 1000.0f - m_height) m_y = 1000.0f - m_height;

    // 4. Cập nhật hoạt ảnh
    m_anim.Update(dt);
}