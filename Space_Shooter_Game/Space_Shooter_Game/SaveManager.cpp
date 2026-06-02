#include "SaveManager.h"
#include "ConfigUtils.h"
#include <filesystem>
#include <iostream>

// Tìm đường dẫn file save tương đối với thư mục thực thi
std::string SaveManager::ResolveSavePath() const {
    // Thử tìm từ working dir trở lên tối đa 6 bậc (giống ConfigUtils)
    std::filesystem::path current = std::filesystem::current_path();
    const std::filesystem::path relative(m_savePath);

    for (int i = 0; i < 6 && !current.empty(); ++i) {
        const std::filesystem::path candidate = current / relative;
        // Nếu file đã tồn tại, dùng luôn đường dẫn đó
        if (std::filesystem::exists(candidate)) {
            return candidate.string();
        }
        current = current.parent_path();
    }

    // Chưa tồn tại → trả về đường dẫn cạnh file exe hiện tại
    std::filesystem::path exeDir = std::filesystem::current_path();
    return (exeDir / relative).string();
}

bool SaveManager::LoadTalentData(TalentSaveData& outData) {
    std::string path = ResolveSavePath();
    try {
        const JsonValue root = SimpleJson::ParseFile(path);

        const JsonValue* pts = root.Find("upgradePoints");
        if (pts) {
            outData.upgradePoints = static_cast<int>(pts->AsNumber(0));
        }

        const JsonValue* nodes = root.Find("unlockedNodeIds");
        if (nodes && nodes->IsArray()) {
            outData.unlockedNodeIds.clear();
            for (const auto& v : nodes->AsArray()) {
                outData.unlockedNodeIds.push_back(static_cast<int>(v.AsNumber(0)));
            }
        }

        std::cout << "[SaveManager] Loaded save from: " << path << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "[SaveManager] No save found or parse error (" << e.what() << "). Starting fresh.\n";
        return false;
    }
}

bool SaveManager::SaveTalentData(const TalentSaveData& data) {
    std::string path = ResolveSavePath();

    // Tạo mảng JSON cho danh sách node đã mở
    JsonValue::Array nodeArr;
    nodeArr.reserve(data.unlockedNodeIds.size());
    for (int id : data.unlockedNodeIds) {
        nodeArr.push_back(JsonValue(static_cast<double>(id)));
    }

    // Tạo object JSON gốc
    JsonValue::Object root;
    root["upgradePoints"] = JsonValue(static_cast<double>(data.upgradePoints));
    root["unlockedNodeIds"] = JsonValue(std::move(nodeArr));

    bool ok = SimpleJson::WriteFile(path, JsonValue(std::move(root)));
    if (ok) {
        std::cout << "[SaveManager] Saved to: " << path << "\n";
    } else {
        std::cerr << "[SaveManager] Failed to write save file: " << path << "\n";
    }
    return ok;
}

void SaveManager::DeleteSave() {
    std::string path = ResolveSavePath();
    try {
        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
            std::cout << "[SaveManager] Save deleted.\n";
        }
    } catch (...) {
        std::cerr << "[SaveManager] Failed to delete save.\n";
    }
}
