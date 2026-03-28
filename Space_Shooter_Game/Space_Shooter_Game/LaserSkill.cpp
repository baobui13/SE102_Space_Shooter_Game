#include "LaserSkill.h"
#include "GameContext.h"
#include "EntityManager.h"
#include "Player.h"

LaserSkill::LaserSkill()
    : Skill("Laser Beam", "Fires a massive laser beam from your ship.", SkillType::ACTIVE, 10.0f, 5) {
    m_duration = 5.0f;
    m_baseDamage = 50;
}

bool LaserSkill::CanActivate(GameContext& ctx) {
    if (ctx.input.IsKeyDown('1')) {
        return true;
    }
    return false;
}

void LaserSkill::Activate(GameContext& ctx) {
    // Sát thương tăng theo Level và scale theo tỉ lệ Attack của Player (ví dụ 1.5x)
    int damage = m_baseDamage + (m_level - 1) * 20 + (int)(ctx.player.GetAttackDamage() * 1.5f);
    
    // Scale kích thước
    float sizeMultiplier = ctx.player.GetSkillSizeMultiplier();
    
    auto laser = std::make_unique<Laser>(ctx.gfx, m_duration, damage, sizeMultiplier);
    ctx.entityManager.AddEntity(std::move(laser));
}

void LaserSkill::OnLevelUp() {
    // Làm gì đó khi nâng cấp, ví dụ buff damage, thêm tia...
}
