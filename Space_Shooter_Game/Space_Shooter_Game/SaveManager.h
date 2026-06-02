#pragma once
#include "SimpleJson.h"
#include <string>
#include <vector>

// ===================================================
// SaveManager — Singleton quản lý đọc/ghi file save
// File save: save/player_save.json
// Lưu: upgradePoints, danh sách id node đã mở
// ===================================================
class SaveManager {
public:
    struct TalentSaveData {
        int upgradePoints = 0;
        std::vector<int> unlockedNodeIds;
    };

    static SaveManager& GetInstance() {
        static SaveManager instance;
        return instance;
    }

    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    // Tải dữ liệu talent từ file. Trả false nếu không có file / lỗi.
    bool LoadTalentData(TalentSaveData& outData);

    // Ghi dữ liệu talent ra file.
    bool SaveTalentData(const TalentSaveData& data);

    // Xoá toàn bộ save (dùng khi start new game).
    void DeleteSave();

    // Đường dẫn file save (có thể ghi đè cho unit test).
    void SetSavePath(const std::string& path) { m_savePath = path; }
    const std::string& GetSavePath() const { return m_savePath; }

private:
    SaveManager() = default;
    std::string ResolveSavePath() const;

    std::string m_savePath = "save/player_save.json";
};
