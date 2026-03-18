#include "AssetManager.h"
#include "Graphics.h"
#include <filesystem>

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> AssetManager::GetTexture(Graphics& gfx, const std::wstring& filePath) {

    // Chuẩn hóa đường dẫn (Normalize)
    std::wstring normalizedPath = std::filesystem::weakly_canonical(filePath).wstring();

    // 1. DÙNG NORMALIZED PATH ĐỂ TÌM KIẾM
    auto it = m_textures.find(normalizedPath);

    if (it != m_textures.end()) {
        return it->second;
    }

    // 3. DÙNG NORMALIZED PATH ĐỂ NẠP TỪ Ổ CỨNG
    auto newTexture = gfx.LoadTexture(normalizedPath.c_str());

    // 4. DÙNG NORMALIZED PATH LÀM CHÌA KHÓA LƯU TRỮ VÀO KHO
    if (!newTexture) {
        OutputDebugString(L"[AssetManager] Failed to load texture!\n");
        return nullptr;
    }
    m_textures[normalizedPath] = newTexture;

    return newTexture;
}

void AssetManager::Clear() {
    // Xóa toàn bộ dữ liệu trong unordered_map, ComPtr sẽ tự động giải phóng RAM
    m_textures.clear();
}