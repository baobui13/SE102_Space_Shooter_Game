#include "AnimationManager.h"
#include "AssetManager.h"
#include "ConfigUtils.h"
#include "SimpleJson.h"
#include <filesystem>

AnimationManager& AnimationManager::GetInstance() {
    static AnimationManager instance;
    return instance;
}

// ─── Helpers cục bộ ───────────────────────────────────────────────────────────
namespace {

std::wstring ToWide(const std::string& s) {
    return std::wstring(s.begin(), s.end());
}

float ReadFloat(const JsonValue& obj, const std::string& key, float fallback) {
    const JsonValue* v = obj.Find(key);
    return v ? static_cast<float>(v->AsNumber(fallback)) : fallback;
}

int ReadInt(const JsonValue& obj, const std::string& key, int fallback) {
    const JsonValue* v = obj.Find(key);
    return v ? static_cast<int>(v->AsNumber(fallback)) : fallback;
}

// Tìm file config theo path tương đối (hỗ trợ chạy từ nhiều thư mục)
std::string ResolveAnimPath(const std::string& filePath) {
    if (std::filesystem::exists(filePath)) return filePath;

    std::filesystem::path current = std::filesystem::current_path();
    for (int i = 0; i < 6 && !current.empty(); ++i) {
        const auto candidate = current / std::filesystem::path(filePath);
        if (std::filesystem::exists(candidate)) return candidate.string();
        current = current.parent_path();
    }
    return filePath;
}

} // namespace

// ─── Public API ───────────────────────────────────────────────────────────────

void AnimationManager::LoadAll(Graphics& gfx) {
    m_gfx = &gfx;

    const std::string files[] = {
        "config/animations/player.json",
        "config/animations/enemies.json",
        "config/animations/bullets.json",
    };

    for (const auto& f : files) {
        LoadFromFile(f);
    }

    const auto total = static_cast<int>(m_defs.size());
    OutputDebugStringA(("[AnimationManager] Loaded " + std::to_string(total) + " animations.\n").c_str());
}

void AnimationManager::LoadFromFile(const std::string& filePath) {
    const std::string resolved = ResolveAnimPath(filePath);

    try {
        const JsonValue root = SimpleJson::ParseFile(resolved);
        if (!root.IsObject()) return;

        for (const auto& [animId, value] : root.AsObject()) {
            AnimationDef def;
            def.texturePath = ToWide(value.At("texture").AsStringOr("Assets/Spaceship.png"));
            // "clip" là tên clip bên trong SpriteAnimation (dùng khi gọi Play())
            def.clipName    = value.At("clip").AsStringOr(animId);
            def.x           = ReadInt(value, "x", 0);
            def.y           = ReadInt(value, "y", 0);
            def.w           = ReadInt(value, "w", 64);
            def.h           = ReadInt(value, "h", 64);
            def.count       = ReadInt(value, "count", 1);
            def.cols        = ReadInt(value, "cols", 1);
            def.duration    = ReadFloat(value, "duration", 1.0f);
            def.loop        = value.At("loop").AsBool(true);
            def.spacingX    = ReadInt(value, "spacingX", 0);
            def.spacingY    = ReadInt(value, "spacingY", 0);

            m_defs[animId] = std::move(def);
        }
    }
    catch (...) {
        OutputDebugStringA(("[AnimationManager] Không load được: " + filePath + "\n").c_str());
    }
}

bool AnimationManager::Configure(const std::string& animId, SpriteAnimation& outAnim) const {
    const auto it = m_defs.find(animId);
    if (it == m_defs.end()) {
        OutputDebugStringA(("[AnimationManager] Animation không tồn tại: " + animId + "\n").c_str());
        return false;
    }

    const AnimationDef& def = it->second;
    auto texture = GetTexture(def.texturePath);
    if (!texture) {
        OutputDebugStringA(("[AnimationManager] Không load được texture cho: " + animId + "\n").c_str());
        return false;
    }

    outAnim.Initialize(texture);
    outAnim.AddClip(
        def.clipName,
        def.x, def.y, def.w, def.h,
        def.count, def.cols,
        def.duration, def.loop,
        def.spacingX, def.spacingY
    );
    return true;
}

const std::string& AnimationManager::GetClipName(const std::string& animId) const {
    const auto it = m_defs.find(animId);
    if (it != m_defs.end()) return it->second.clipName;
    return animId; // fallback: trả về chính animId
}

bool AnimationManager::HasAnimation(const std::string& animId) const {
    return m_defs.count(animId) > 0;
}

// ─── Private ──────────────────────────────────────────────────────────────────

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
AnimationManager::GetTexture(const std::wstring& path) const {
    if (!m_gfx) return nullptr;
    return AssetManager::GetInstance().GetTexture(*m_gfx, path.c_str());
}
