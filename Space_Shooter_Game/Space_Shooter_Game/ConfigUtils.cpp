#include "ConfigUtils.h"

#include <filesystem>

std::string ResolveConfigPath(const std::string& filePath) {
    if (std::filesystem::exists(filePath)) {
        return filePath;
    }

    std::filesystem::path current = std::filesystem::current_path();
    const std::filesystem::path relativePath(filePath);
    for (int i = 0; i < 6 && !current.empty(); ++i) {
        const std::filesystem::path candidate = current / relativePath;
        if (std::filesystem::exists(candidate)) {
            return candidate.string();
        }
        current = current.parent_path();
    }

    const std::filesystem::path projectPath =
        std::filesystem::path("Space_Shooter_Game") / "Space_Shooter_Game" / filePath;
    if (std::filesystem::exists(projectPath)) {
        return projectPath.string();
    }

    return filePath;
}
