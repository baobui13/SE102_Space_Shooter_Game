#pragma once
#include "Skill.h"

// Kỹ năng bắn viên đạn đặc biệt bay chậm về phía chuột,
// chạm rìa màn hình hoặc kẻ địch thì phát nổ.
class ExplodingBulletSkill : public Skill {
private:
    float m_speed;   // Tốc độ viên đạn (px/giây)
    int   m_damage;  // Sát thương khi nổ (dùng sau khi có Enemy)

public:
    ExplodingBulletSkill();

    bool CanActivate(GameContext& ctx) override;
    void Activate(GameContext& ctx) override;

protected:
    void OnLevelUp() override;
};
