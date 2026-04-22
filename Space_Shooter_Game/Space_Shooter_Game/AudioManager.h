#pragma once

#include <Audio.h>
#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

enum class AudioCategory : size_t {
    BackgroundMusic = 0,
    SoundEffect,
    UiEffect,
    Count
};

namespace AudioIds {
inline constexpr const char* MenuMusic = "bgm.menu";
inline constexpr const char* GameplayMusic = "bgm.gameplay";

inline constexpr const char* PlayerShoot = "sfx.player.shoot";
inline constexpr const char* PlayerDash = "sfx.player.dash";
inline constexpr const char* PlayerHit = "sfx.player.hit";
inline constexpr const char* PlayerDeath = "sfx.player.death";
inline constexpr const char* PlayerLevelUp = "sfx.player.levelup";

inline constexpr const char* UiHover = "ui.hover";
inline constexpr const char* UiClick = "ui.click";
inline constexpr const char* UiOpenLevelUp = "ui.levelup.open";
inline constexpr const char* UiSelectUpgrade = "ui.levelup.select";
}

class AudioManager {
public:
    static AudioManager& GetInstance();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    void Initialize();
    void Shutdown();
    void Update();

    void RegisterClip(const std::string& id, const std::wstring& filePath, AudioCategory category, float defaultVolume = 1.0f);

    void PlayMusic(const std::string& id, bool loop = true);
    void StopMusic(bool immediate = true);

    void PlaySoundEffect(const std::string& id, float volumeMultiplier = 1.0f, float pitch = 0.0f, float pan = 0.0f);
    void PlayUiEffect(const std::string& id, float volumeMultiplier = 1.0f, float pitch = 0.0f, float pan = 0.0f);

    void SetMasterVolume(float volume);
    float GetMasterVolume() const { return m_masterVolume; }

    void SetCategoryVolume(AudioCategory category, float volume);
    float GetCategoryVolume(AudioCategory category) const;

    void SetBackgroundMusicVolume(float volume) { SetCategoryVolume(AudioCategory::BackgroundMusic, volume); }
    void SetSoundEffectVolume(float volume) { SetCategoryVolume(AudioCategory::SoundEffect, volume); }
    void SetUiEffectVolume(float volume) { SetCategoryVolume(AudioCategory::UiEffect, volume); }

    float GetBackgroundMusicVolume() const { return GetCategoryVolume(AudioCategory::BackgroundMusic); }
    float GetSoundEffectVolume() const { return GetCategoryVolume(AudioCategory::SoundEffect); }
    float GetUiEffectVolume() const { return GetCategoryVolume(AudioCategory::UiEffect); }

    bool IsReady() const { return m_ready && m_audioEngine != nullptr; }

private:
    struct ClipRecord {
        std::wstring filePath;
        AudioCategory category = AudioCategory::SoundEffect;
        float defaultVolume = 1.0f;
        bool warnedMissingFile = false;
        bool warnedLoadFailure = false;
        std::unique_ptr<DirectX::SoundEffect> soundEffect;
    };

    AudioManager() = default;

    void RegisterDefaultClips();
    void PlayOneShot(const std::string& id, float volumeMultiplier, float pitch, float pan);
    ClipRecord* FindClip(const std::string& id);
    const ClipRecord* FindClip(const std::string& id) const;
    ClipRecord* EnsureClipLoaded(const std::string& id);
    void RefreshMusicVolume();
    float ComputeFinalVolume(const ClipRecord& clip, float volumeMultiplier) const;

    static size_t CategoryToIndex(AudioCategory category);
    static float Clamp01(float value);

    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
    std::unordered_map<std::string, ClipRecord> m_clips;
    std::unique_ptr<DirectX::SoundEffectInstance> m_musicInstance;
    std::string m_currentMusicId;
    std::array<float, static_cast<size_t>(AudioCategory::Count)> m_categoryVolumes{ 0.55f, 0.9f, 1.0f };
    float m_masterVolume = 1.0f;
    bool m_ready = false;
    bool m_defaultClipsRegistered = false;
    bool m_loggedMissingDevice = false;
};
