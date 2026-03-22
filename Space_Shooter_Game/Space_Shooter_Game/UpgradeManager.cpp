#include "UpgradeManager.h"
#include "Player.h" // Bắt buộc include Player.h để gọi các hàm Set...
#include <random>
#include <algorithm>

UpgradeManager::UpgradeManager() {
    Initialize();
}

void UpgradeManager::Initialize() {
    m_allUpgrades.clear();

    // Thêm các thẻ nâng cấp vào đây (Nhớ viết tiếng Việt KHÔNG DẤU nhé)
    m_allUpgrades.push_back({ "Sung Nhieu Nong", "Tang 5 sat thuong", [](Player& p) { p.SetAttackDamage(p.GetAttackDamage() + 5); } });
    m_allUpgrades.push_back({ "Nam cham sieu cap", "Hut xa hon", [](Player& p) { p.SetMagnetRange(p.GetMagnetRange() + 50.0f); } });
    m_allUpgrades.push_back({ "Dong co Turbo", "Ban nhanh hon", [](Player& p) { p.SetAttackSpeed(p.GetAttackSpeed() + 1.0f); } });
    m_allUpgrades.push_back({ "Ao Giap Thep", "Hoi phuc 50 Mau", [](Player& p) { p.TakeDamage(-50); /* Trừ damage = Hồi máu */ } });

    // Bạn có thể thoải mái thêm 100 thẻ nữa ở đây!
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