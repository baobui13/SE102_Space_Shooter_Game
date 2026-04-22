#include "AudioManager.h"

#include <Windows.h>

#include <algorithm>
#include <filesystem>
#include <string>

namespace {
std::wstring ToWide(const std::string& value) {
    return std::wstring(value.begin(), value.end());
}

void DebugLog(const std::wstring& message) {
    std::wstring finalMessage = message + L"\n";
    OutputDebugStringW(finalMessage.c_str());
}
}

AudioManager& AudioManager::GetInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::Initialize() {
    RegisterDefaultClips();

    if (m_audioEngine) {
        m_audioEngine->SetMasterVolume(m_masterVolume);
        RefreshMusicVolume();
        m_ready = true;
        return;
    }

    try {
        m_audioEngine = std::make_unique<DirectX::AudioEngine>(DirectX::AudioEngine_Default);
        m_audioEngine->SetMasterVolume(m_masterVolume);
        m_ready = true;
        m_loggedMissingDevice = false;
    }
    catch (...) {
        DebugLog(L"[AudioManager] Failed to initialize DirectXTK AudioEngine.");
        m_audioEngine.reset();
        m_ready = false;
    }
}

void AudioManager::Shutdown() {
    StopMusic();
    m_clips.clear();
    m_audioEngine.reset();
    m_currentMusicId.clear();
    m_ready = false;
    m_defaultClipsRegistered = false;
    m_loggedMissingDevice = false;
}

void AudioManager::Update() {
    if (!m_audioEngine) {
        return;
    }

    if (m_audioEngine->Update()) {
        m_loggedMissingDevice = false;
        return;
    }

    if (!m_audioEngine->IsCriticalError()) {
        if (!m_loggedMissingDevice) {
            DebugLog(L"[AudioManager] No audio device found. Game is running in silent mode.");
            m_loggedMissingDevice = true;
        }
        return;
    }

    const std::string currentMusicId = m_currentMusicId;
    m_musicInstance.reset();

    try {
        m_ready = m_audioEngine->Reset();
        if (!m_ready) {
            DebugLog(L"[AudioManager] Audio engine reset entered silent mode.");
            return;
        }

        m_audioEngine->SetMasterVolume(m_masterVolume);
        if (!currentMusicId.empty()) {
            PlayMusic(currentMusicId, true);
        }
        DebugLog(L"[AudioManager] Audio engine reset successfully.");
    }
    catch (...) {
        m_ready = false;
        DebugLog(L"[AudioManager] Audio engine reset failed.");
    }
}

void AudioManager::RegisterClip(const std::string& id, const std::wstring& filePath, AudioCategory category, float defaultVolume) {
    if (id == m_currentMusicId) {
        StopMusic();
    }

    ClipRecord clip;
    clip.filePath = filePath;
    clip.category = category;
    clip.defaultVolume = Clamp01(defaultVolume);

    m_clips[id] = std::move(clip);
}

void AudioManager::PlayMusic(const std::string& id, bool loop) {
    if (!IsReady()) {
        return;
    }

    ClipRecord* clip = EnsureClipLoaded(id);
    if (!clip) {
        return;
    }

    if (m_currentMusicId == id && m_musicInstance) {
        RefreshMusicVolume();
        if (m_musicInstance->GetState() != DirectX::PLAYING) {
            m_musicInstance->Play(loop);
        }
        return;
    }

    StopMusic();

    try {
        m_musicInstance = clip->soundEffect->CreateInstance();
        m_currentMusicId = id;
        RefreshMusicVolume();
        m_musicInstance->Play(loop);
    }
    catch (...) {
        DebugLog(L"[AudioManager] Failed to start background music: " + ToWide(id));
        m_musicInstance.reset();
        m_currentMusicId.clear();
    }
}

void AudioManager::StopMusic(bool immediate) {
    if (m_musicInstance) {
        m_musicInstance->Stop(immediate);
        m_musicInstance.reset();
    }

    m_currentMusicId.clear();
}

void AudioManager::PlaySoundEffect(const std::string& id, float volumeMultiplier, float pitch, float pan) {
    PlayOneShot(id, volumeMultiplier, pitch, pan);
}

void AudioManager::PlayUiEffect(const std::string& id, float volumeMultiplier, float pitch, float pan) {
    PlayOneShot(id, volumeMultiplier, pitch, pan);
}

void AudioManager::SetMasterVolume(float volume) {
    m_masterVolume = Clamp01(volume);

    if (m_audioEngine) {
        m_audioEngine->SetMasterVolume(m_masterVolume);
    }
}

void AudioManager::SetCategoryVolume(AudioCategory category, float volume) {
    m_categoryVolumes[CategoryToIndex(category)] = Clamp01(volume);

    if (category == AudioCategory::BackgroundMusic) {
        RefreshMusicVolume();
    }
}

float AudioManager::GetCategoryVolume(AudioCategory category) const {
    return m_categoryVolumes[CategoryToIndex(category)];
}

void AudioManager::RegisterDefaultClips() {
    if (m_defaultClipsRegistered) {
        return;
    }

    RegisterClip(AudioIds::MenuMusic, L"Assets/Audio/BGM/menu_theme.wav", AudioCategory::BackgroundMusic, 0.7f);
    RegisterClip(AudioIds::GameplayMusic, L"Assets/Audio/BGM/gameplay_theme.wav", AudioCategory::BackgroundMusic, 0.8f);

    RegisterClip(AudioIds::PlayerShoot, L"Assets/Audio/SFX/player_shoot.wav", AudioCategory::SoundEffect, 0.7f);
    RegisterClip(AudioIds::PlayerDash, L"Assets/Audio/SFX/player_dash.wav", AudioCategory::SoundEffect, 0.85f);
    RegisterClip(AudioIds::PlayerHit, L"Assets/Audio/SFX/player_hit.wav", AudioCategory::SoundEffect, 0.9f);
    RegisterClip(AudioIds::PlayerDeath, L"Assets/Audio/SFX/player_death.wav", AudioCategory::SoundEffect, 1.0f);
    RegisterClip(AudioIds::PlayerLevelUp, L"Assets/Audio/SFX/player_levelup.wav", AudioCategory::SoundEffect, 0.95f);

    RegisterClip(AudioIds::UiHover, L"Assets/Audio/UI/ui_hover.wav", AudioCategory::UiEffect, 0.5f);
    RegisterClip(AudioIds::UiClick, L"Assets/Audio/UI/ui_click.wav", AudioCategory::UiEffect, 0.75f);
    RegisterClip(AudioIds::UiOpenLevelUp, L"Assets/Audio/UI/ui_open_levelup.wav", AudioCategory::UiEffect, 0.8f);
    RegisterClip(AudioIds::UiSelectUpgrade, L"Assets/Audio/UI/ui_select_upgrade.wav", AudioCategory::UiEffect, 0.85f);

    m_defaultClipsRegistered = true;
}

void AudioManager::PlayOneShot(const std::string& id, float volumeMultiplier, float pitch, float pan) {
    if (!IsReady()) {
        return;
    }

    ClipRecord* clip = EnsureClipLoaded(id);
    if (!clip) {
        return;
    }

    try {
        clip->soundEffect->Play(ComputeFinalVolume(*clip, volumeMultiplier), pitch, pan);
    }
    catch (...) {
        DebugLog(L"[AudioManager] Failed to play clip: " + ToWide(id));
    }
}

AudioManager::ClipRecord* AudioManager::FindClip(const std::string& id) {
    auto it = m_clips.find(id);
    if (it == m_clips.end()) {
        return nullptr;
    }

    return &it->second;
}

const AudioManager::ClipRecord* AudioManager::FindClip(const std::string& id) const {
    auto it = m_clips.find(id);
    if (it == m_clips.end()) {
        return nullptr;
    }

    return &it->second;
}

AudioManager::ClipRecord* AudioManager::EnsureClipLoaded(const std::string& id) {
    ClipRecord* clip = FindClip(id);
    if (!clip) {
        DebugLog(L"[AudioManager] Unknown audio clip id: " + ToWide(id));
        return nullptr;
    }

    if (clip->soundEffect) {
        return clip;
    }

    if (!std::filesystem::exists(clip->filePath)) {
        if (!clip->warnedMissingFile) {
            DebugLog(L"[AudioManager] Missing audio file for '" + ToWide(id) + L"': " + clip->filePath);
            clip->warnedMissingFile = true;
        }
        return nullptr;
    }

    try {
        clip->soundEffect = std::make_unique<DirectX::SoundEffect>(m_audioEngine.get(), clip->filePath.c_str());
        clip->warnedMissingFile = false;
        clip->warnedLoadFailure = false;
        return clip;
    }
    catch (...) {
        if (!clip->warnedLoadFailure) {
            DebugLog(L"[AudioManager] Failed to load audio file for '" + ToWide(id) + L"': " + clip->filePath);
            clip->warnedLoadFailure = true;
        }
        return nullptr;
    }
}

void AudioManager::RefreshMusicVolume() {
    if (!m_musicInstance || m_currentMusicId.empty()) {
        return;
    }

    const ClipRecord* clip = FindClip(m_currentMusicId);
    if (!clip) {
        return;
    }

    m_musicInstance->SetVolume(ComputeFinalVolume(*clip, 1.0f));
}

float AudioManager::ComputeFinalVolume(const ClipRecord& clip, float volumeMultiplier) const {
    const float categoryVolume = m_categoryVolumes[CategoryToIndex(clip.category)];
    return Clamp01(clip.defaultVolume * categoryVolume * volumeMultiplier);
}

size_t AudioManager::CategoryToIndex(AudioCategory category) {
    return static_cast<size_t>(category);
}

float AudioManager::Clamp01(float value) {
    return std::clamp(value, 0.0f, 1.0f);
}
