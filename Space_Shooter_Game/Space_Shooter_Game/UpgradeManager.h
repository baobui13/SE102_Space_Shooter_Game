#pragma once
#include "Upgrade.h"
#include <vector>

class UpgradeManager {
private:
    std::vector<UpgradeOption> m_allUpgrades;

    // Chặn khởi tạo tự do (Singleton Pattern)
    UpgradeManager();
    ~UpgradeManager() = default;
    UpgradeManager(const UpgradeManager&) = delete;
    UpgradeManager& operator=(const UpgradeManager&) = delete;

public:
    // Hàm lấy instance duy nhất
    static UpgradeManager& GetInstance() {
        static UpgradeManager instance;
        return instance;
    }

    // Nạp toàn bộ thẻ vào kho
    void Initialize();

    // Bốc ngẫu nhiên 'count' thẻ không trùng nhau
    std::vector<UpgradeOption> GetRandomUpgrades(int count);
};