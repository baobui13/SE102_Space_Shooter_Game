#pragma once
#include "SpriteAnimation.h"
#include "Graphics.h"
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <d3d11.h>

// Cấu hình cho một animation được đặt tên (một clip, một texture)
// Được đọc từ JSON, không liên quan gì đến đối tượng game.
struct AnimationDef {
    std::wstring texturePath;
    std::string  clipName;   // Tên clip dùng trong SpriteAnimation::Play()
    int x = 0, y = 0;
    int w = 64, h = 64;
    int count = 1, cols = 1;
    float duration = 1.0f;
    bool loop = true;
    int spacingX = 0, spacingY = 0;
    // Kích thước hiển thị trên màn hình (0 = dùng w/h của frame)
    float displayWidth = 0.0f;
    float displayHeight = 0.0f;
};

// AnimationManager — Singleton quản lý toàn bộ animation của game.
//
// Quy tắc thiết kế:
//  - Animation là đối tượng độc lập, được định danh bằng tên (ví dụ: "player_idle", "enemy1_death")
//  - Các object (Player, Enemy, Bullet...) KHÔNG được chứa thông số cấu hình animation.
//    Chúng chỉ gọi Configure() / GetClipName() với tên animation cần dùng.
//  - Nhiều object khác nhau có thể tái sử dụng cùng một animation (ví dụ "explosion_small").
//
// Workflow:
//  1. Gọi AnimationManager::GetInstance().LoadAll(gfx) một lần khi khởi động game.
//  2. Trong constructor của object: Configure("anim_id", m_anim);
//  3. Khi state thay đổi: m_anim.Play(GetClipName("anim_id"));
class AnimationManager {
public:
    static AnimationManager& GetInstance();

    // Load toàn bộ animation config từ các file JSON trong config/animations/
    void LoadAll(Graphics& gfx);

    // Load animation config từ một file JSON cụ thể.
    // JSON format: { "anim_id": { "texture": "...", "clip": "...", "x":0, ... }, ... }
    void LoadFromFile(const std::string& filePath);

    // Cấu hình một SpriteAnimation theo tên animation.
    // - Initialize texture cho outAnim.
    // - AddClip tương ứng vào outAnim.
    // Trả về false nếu animId không tồn tại.
    bool Configure(const std::string& animId, SpriteAnimation& outAnim) const;

    // Configure + Play clip — dùng khi gán animation lần đầu cho object.
    bool PlayAnimation(const std::string& animId, SpriteAnimation& outAnim) const;

    // Kích thước hiển thị từ JSON (fallback: kích thước frame w/h).
    void GetDisplaySize(const std::string& animId, float& outW, float& outH) const;

    // Lấy tên clip của một animation (để gọi SpriteAnimation::Play()).
    // Trả về animId nếu không tìm thấy.
    const std::string& GetClipName(const std::string& animId) const;

    bool HasAnimation(const std::string& animId) const;

private:
    AnimationManager() = default;
    AnimationManager(const AnimationManager&) = delete;
    AnimationManager& operator=(const AnimationManager&) = delete;

    Graphics* m_gfx = nullptr;

    // map: animId → AnimationDef (stateless config)
    std::unordered_map<std::string, AnimationDef> m_defs;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
        GetTexture(const std::wstring& path) const;
};
