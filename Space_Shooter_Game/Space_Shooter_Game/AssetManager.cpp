#include "AssetManager.h"
#include "Graphics.h"

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> AssetManager::GetTexture(Graphics& gfx, const std::wstring& filePath) {

    // 1. Tìm xem đường dẫn file này đã tồn tại trong kho chưa
    auto it = m_textures.find(filePath);

    // 2. Nếu ĐÃ CÓ trong kho -> Trả về ảnh đó
    if (it != m_textures.end()) {
        return it->second;
    }

    // 3. Nếu CHƯA CÓ -> Nhờ Graphics nạp từ ổ cứng
    auto newTexture = gfx.LoadTexture(filePath.c_str());

    // 4. Nếu nạp thành công, cất vào kho để các Object sau dùng lại
    if (newTexture) {
        m_textures[filePath] = newTexture;
    }

    // 5. Trả ảnh về cho người gọi
    return newTexture;
}

void AssetManager::Clear() {
    // Xóa toàn bộ dữ liệu trong unordered_map, ComPtr sẽ tự động giải phóng RAM
    m_textures.clear();
}