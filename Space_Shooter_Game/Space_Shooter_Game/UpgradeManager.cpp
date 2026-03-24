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
    m_allUpgrades.push_back({
        "ATTACK", "Sung Nhieu Nong", "Tang 5 sat thuong", "sword",
        [](Player& p) { p.SetAttackDamage(p.GetAttackDamage() + 5); }
        });

    m_allUpgrades.push_back({
        "UTILITY", "Nam cham sieu cap", "Hut xa hon", "shield",
        [](Player& p) { p.SetMagnetRange(p.GetMagnetRange() + 50.0f); }
        });

    m_allUpgrades.push_back({
        "SPEED", "Dong co Turbo", "Ban nhanh hon", "boot",
        [](Player& p) { p.SetAttackSpeed(p.GetAttackSpeed() + 1.0f); }
        });

    m_allUpgrades.push_back({
        "DEFENSE", "Ao Giap Thep", "Hoi phuc 50 Mau", "heart",
        [](Player& p) { p.TakeDamage(-50); /* Trừ damage = Hồi máu */ }
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