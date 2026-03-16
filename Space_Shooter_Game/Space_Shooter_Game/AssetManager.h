#pragma once
#include <unordered_map>
#include <string>
#include <wrl/client.h>
#include <d3d11.h>

// Báo cho C++ biết có class Graphics để truyền vào tham số
class Graphics;

class AssetManager {
private:
    // Kho lưu trữ ảnh: key là tên đường dẫn file, Giá trị (Value) là ảnh Texture
    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

    // Đưa Constructor vào private để ngăn việc tạo ra nhiều AssetManager
    AssetManager() = default;

public:
    // Xóa hàm copy để đảm bảo nguyên tắc Singleton (chỉ có 1 bản thể)
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    // Hàm lấy thực thể duy nhất của AssetManager (Singleton Pattern)
    static AssetManager& GetInstance() {
        static AssetManager instance;
        return instance;
    }

    // --- CÁC HÀM CHỨC NĂNG CHÍNH ---

    // Nạp hoặc lấy ảnh từ kho
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(Graphics& gfx, const std::wstring& filePath);

    // Giải phóng toàn bộ bộ nhớ (Gọi khi chuyển màn hình nếu cần)
    void Clear();
};