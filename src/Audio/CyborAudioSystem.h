#pragma once

#include <string>
#include <unordered_map>
#include <memory>

/*
 * CyborAudioSystem - Advanced 3D audio system
 * Handles weapon sounds, ambient audio, and Cybor sound effects
 */
class CyborAudioSystem {
public:
    CyborAudioSystem();
    ~CyborAudioSystem();

    bool Initialize();
    void Update(float deltaTime);
    void Shutdown();

    // Sound loading and management
    bool LoadSound(const std::string& name, const std::string& filename);
    void UnloadSound(const std::string& name);
    void UnloadAllSounds();

    // 2D Sound playback
    void PlaySound(const std::string& name, float volume = 1.0f, bool loop = false);
    void PlayBackgroundMusic(const std::string& filename, float volume = 0.5f);
    void StopSound(const std::string& name);
    void StopAllSounds();

    // 3D Positional audio
    void PlaySound3D(const std::string& name, const glm::vec3& position, 
                     float volume = 1.0f, float maxDistance = 100.0f);
    void SetListenerPosition(const glm::vec3& position, const glm::vec3& forward, 
                           const glm::vec3& up);

    // Volume controls
    void SetMasterVolume(float volume);
    void SetSFXVolume(float volume);
    void SetMusicVolume(float volume);

    // Cybor audio enhancements
    void EnableCyborAudio(bool enable) { m_cyborAudioEnabled = enable; }
    void SetCyborAudioIntensity(float intensity) { m_cyborAudioIntensity = intensity; }
    void PlayCyborEffect(const std::string& effectName);

private:
    struct AudioSource {
        // Audio implementation would use OpenAL or similar
        std::string filename;
        bool is3D;
        bool isLooping;
        float volume;
        glm::vec3 position;
    };

    std::unordered_map<std::string, std::unique_ptr<AudioSource>> m_sounds;

    // Volume settings
    float m_masterVolume;
    float m_sfxVolume;
    float m_musicVolume;

    // 3D Audio
    glm::vec3 m_listenerPosition;
    glm::vec3 m_listenerForward;
    glm::vec3 m_listenerUp;

    // Cybor enhancements
    bool m_cyborAudioEnabled;
    float m_cyborAudioIntensity;

    // Private methods
    void InitializeAudioDevice();
    void LoadDefaultSounds();
    float CalculateVolumeByDistance(const glm::vec3& sourcePos, float maxDistance);
};
