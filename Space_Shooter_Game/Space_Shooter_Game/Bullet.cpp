#include "Bullet.h"
#include "AssetManager.h"
#include <cmath>

Bullet::Bullet(Graphics& gfx, float startX, float startY, float targetX, float targetY, float speed, int damage, float maxDistance)
// Gọi constructor của class cha. Giả sử viên đạn có kích thước 16x16 pixel
    : GameObject(startX, startY, 16.0f, 16.0f), 
    m_damage(damage),
    m_maxDistance(maxDistance),
    m_distanceTraveled(0.0f) // Mới sinh ra nên quãng đường bay = 0
{
    // -----------------------------------------
    // 1. TÍNH TOÁN VẬN TỐC THEO HƯỚNG CHUỘT
    // -----------------------------------------
    float dx = targetX - startX;
    float dy = targetY - startY;

    // Tính khoảng cách từ đạn đến chuột
    float distance = std::sqrt(dx * dx + dy * dy);

    // Chuẩn hóa vector hướng (ngăn chặn lỗi chia cho 0)
    if (distance != 0.0f) {
        dx /= distance;
        dy /= distance;
    }
    else {
        // Nếu nhắm ngay tại thân mình thì cho bay đại sang phải
        dx = 0.0f;
        dy = 1.0f;
    }

    // Gán vận tốc (Nhờ 2 biến này, GameObject::Update sẽ tự động đẩy viên đạn đi)
    m_vx = dx * speed;
    m_vy = dy * speed;

    // -----------------------------------------
    // 2. KHỞI TẠO HÌNH ẢNH ĐẠN
    // -----------------------------------------
    // Bạn cần chuẩn bị 1 file ảnh Bullet.png. Giả sử nó có 1 khung hình 16x16
    m_anim.Initialize(AssetManager::GetInstance().GetTexture(gfx, L"Assets/Bullets1.png"));
    m_anim.AddClip("Fly", 15, 15, 17, 17, 1, 1, 1.0f, true);
    m_anim.Play("Fly");
}

void Bullet::Update(float dt) {
    // Kế thừa logic di chuyển mặc định từ GameObject (m_x += m_vx * dt)
    GameObject::Update(dt);

    // TÍNH TOÁN QUÃNG ĐƯỜNG ĐÃ BAY
    float speed = std::sqrt(m_vx * m_vx + m_vy * m_vy);
    m_distanceTraveled += speed * dt;

    // DỌN RÁC : Xóa đạn nếu bay quá tầm BÁN KÍNH cho phép
    if (m_distanceTraveled >= m_maxDistance) {
        Destroy();
    }

    // LOGIC DỌN RÁC: Xóa viên đạn nếu nó bay khỏi màn hình
    // (Giả sử màn hình 800x1000, ta mở rộng biên ra một chút (-50 và +50) để đạn khuất hẳn mới xóa)
    if (m_x < -50.0f || m_x > 850.0f || m_y < -50.0f || m_y > 1050.0f) {
        Destroy(); // Gọi hàm Destroy từ GameObject (Đánh dấu m_isActive = false)
    }
}