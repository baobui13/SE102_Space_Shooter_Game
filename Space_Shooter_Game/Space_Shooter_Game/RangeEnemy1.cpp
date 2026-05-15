#include "RangeEnemy1.h"
#include "GameContext.h"
#include "Player.h"
#include "BulletPool.h"
#include <cmath>

RangeEnemy1::RangeEnemy1(float x, float y, float width, float height,
    float health, float moveSpeed, float attackPower,
    float attackSpeed, float attackRange, EnemyType type)
    : BaseEnemy(x, y, width, height, health, moveSpeed, attackPower, attackSpeed, attackRange, type)
{
    m_state = RangeState::Moving;
    m_stateTimer = 0.0f;
    m_attackCooldown = 1.0f;
}

void RangeEnemy1::Update(float dt, GameContext& ctx) {
    if (!m_isActive) return;

    UpdateAnimation(dt);
    m_stateTimer += dt;

    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
    float myCX = m_x + m_width / 2.0f;
    float myCY = m_y + m_height / 2.0f;

    float dx = playerCX - myCX;
    float dy = playerCY - myCY;
    float dist = std::sqrt(dx * dx + dy * dy);

    switch (m_state) {
    case RangeState::Moving:
        // Cập nhật hướng về player để trông tự nhiên
        UpdateRotationToPlayer(ctx);

        if (dist < SAFE_DISTANCE) {
            // Player ở gần -> bỏ chạy
            float moveDirX = -dx / dist;
            float moveDirY = -dy / dist;
            m_x += moveDirX * m_moveSpeed * dt;
            m_y += moveDirY * m_moveSpeed * dt;
        }
        else {
            // THÊM ĐOẠN NÀY: Player ở xa -> Di chuyển bình thường đi vào màn hình
            Move(dt, ctx);
        }

        if (m_stateTimer >= TIME_MOVE) {
            ResetState(RangeState::Aiming);
        }
        break;

    case RangeState::Aiming:
        // Trong lúc ngắm, xoay liên tục đuổi theo Player
        UpdateRotationToPlayer(ctx);

        if (m_stateTimer >= TIME_AIM) {
            ResetState(RangeState::Shooting);
        }
        break;

    case RangeState::Shooting:
        // Đứng im, KHÔNG cập nhật rotation (khóa hướng bắn)

        if (m_stateTimer >= TIME_SHOOT) {
            // Thực hiện bắn viên đạn cực nhanh theo hướng m_rotation đã khóa
            float aimAngle = GetAimAngle();
            float targetX = myCX + std::cos(aimAngle) * 100.0f;
            float targetY = myCY + std::sin(aimAngle) * 100.0f;

            ctx.bulletPool.GetBullet(
                myCX, myCY,
                targetX, targetY,
                BULLET_SPEED,
                (int)m_attackPower,
                m_attackRange
            );

            ResetState(RangeState::Moving);
        }
        break;
    }
}

void RangeEnemy1::Render(Graphics& gfx) {
    // Vẽ quái vật
    BaseEnemy::Render(gfx);

    // Nếu đang ở trạng thái ngắm, vẽ đường line màu đỏ
    if (m_state == RangeState::Aiming && m_isActive) {
        float myCX = m_x + m_width / 2.0f;
        float myCY = m_y + m_height / 2.0f;

        // Tính điểm kết thúc của đường kẻ (kéo dài ra hết màn hình, ví dụ 2000px)
        float lineLength = 2000.0f;
        float aimAngle = GetAimAngle();
        float endX = myCX + std::cos(aimAngle) * lineLength;
        float endY = myCY + std::sin(aimAngle) * lineLength;

        auto whitePixel = AssetManager::GetInstance().GetTexture(gfx, L"Assets/WhitePixel.png"); // Nhớ tạo file WhitePixel.png 1x1 nhé

        gfx.DrawLine(whitePixel.Get(), myCX, myCY, endX, endY, 2.0f, DirectX::Colors::Red);
    }
}

void RangeEnemy1::ResetState(RangeState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

void RangeEnemy1::UpdateRotationToPlayer(GameContext& ctx) {
    float playerCX = ctx.player.GetX() + ctx.player.GetWidth() / 2.0f;
    float playerCY = ctx.player.GetY() + ctx.player.GetHeight() / 2.0f;
    float myCX = m_x + m_width / 2.0f;
    float myCY = m_y + m_height / 2.0f;

    FacePoint(playerCX, playerCY);
}
