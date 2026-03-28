#include "UpgradeManager.h"
#include "Player.h" // Bắt buộc include Player.h để gọi các hàm Set...
#include <random>
#include <algorithm>

UpgradeManager::UpgradeManager() {
    Initialize();
}

void UpgradeManager::Initialize() {
    m_allUpgrades.clear();

    // { "Loại", "Tên", "Mô tả", "TÊN ICON (iconTag)", Hàm Effect }
    
    // TẤN CÔNG
    m_allUpgrades.push_back({
        "ATTACK", "Tang sat thuong", "Tang 10 sat thuong dan", "sword",
        [](Player& p) { p.SetAttackDamage(p.GetAttackDamage() + 10); }
        });

    m_allUpgrades.push_back({
        "SPEED", "Tang toc do ban", "Tang 1 vien/s", "sword",
        [](Player& p) { p.SetAttackSpeed(p.GetAttackSpeed() + 1.0f); }
        });

    m_allUpgrades.push_back({
        "ATTACK", "Tang tam ban dan", "Tang tam ban", "sword",
        [](Player& p) { p.SetAttackRange(p.GetAttackRange() + 50.0f); }
        });

    m_allUpgrades.push_back({
        "ATTACK", "Giam hoi chieu", "Hoi chieu nhanh hon 10%", "sword",
        [](Player& p) { p.SetCooldownMultiplier(p.GetCooldownMultiplier() * 0.9f); }
        });

    m_allUpgrades.push_back({
        "ATTACK", "Tang kich thuoc", "Ky nang to hon 20%", "sword",
        [](Player& p) { p.SetSkillSizeMultiplier(p.GetSkillSizeMultiplier() + 0.2f); }
        });

    // PHÒNG THỦ & SINH TỒN
    m_allUpgrades.push_back({
        "DEFENSE", "Hoi mau", "Hoi 50 Mau", "heart",
        [](Player& p) { p.Heal(50); }
        });

    m_allUpgrades.push_back({
        "DEFENSE", "Tang mau toi da", "Tang 50 mau goc", "heart",
        [](Player& p) { 
            p.SetMaxHp(p.GetMaxHp() + 50); 
            p.Heal(50); 
        }
        });

    m_allUpgrades.push_back({
        "SPEED", "Toc do di chuyen", "Tau luot nhanh hon", "boot",
        [](Player& p) { p.SetSpeed(p.GetSpeed() + 50.0f); }
        });

    // TIỆN ÍCH
    m_allUpgrades.push_back({
        "UTILITY", "Tang tam hut EXP", "Hut nam cham xa hon", "shield",
        [](Player& p) { p.SetMagnetRange(p.GetMagnetRange() + 50.0f); }
        });
}

std::vector<UpgradeOption> UpgradeManager::GetRandomUpgrades(int count) {
    std::vector<UpgradeOption> result;
    if (m_allUpgrades.empty()) return result;

    // 1. Copy danh sách gốc ra một bộ bài phụ để chuẩn bị xáo
    std::vector<UpgradeOption> deck = m_allUpgrades;

    // 2. Xáo trộn bộ bài ngẫu nhiên (Shuffle)
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    // 3. Rút 'count' lá bài đầu tiên từ bộ bài đã xáo
    for (int i = 0; i < count && i < deck.size(); ++i) {
        result.push_back(deck[i]);
    }

    return result;
}