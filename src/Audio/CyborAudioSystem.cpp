#include "CyborAudioSystem.h"
#include <iostream>

CyborAudioSystem::CyborAudioSystem()
    : m_initialized(false), m_masterVolume(1.0f), m_musicVolume(0.7f), m_sfxVolume(0.8f),
      m_currentMusic(""), m_backgroundMusicEnabled(true) {
}

CyborAudioSystem::~CyborAudioSystem() {
    Shutdown();
}

bool CyborAudioSystem::Initialize() {
    // Initialize audio system
    // In a real implementation, this would initialize OpenAL, FMOD, or similar
    std::cout << "Initializing Cybor Audio System..." << std::endl;
    
    m_initialized = true;
    std::cout << "Cybor Audio System initialized successfully!" << std::endl;
    return true;
}

void CyborAudioSystem::Update(float deltaTime) {
    if (!m_initialized) return;
    
    // Update audio sources, fade effects, etc.
    // This would handle audio updates in a real implementation
}

void CyborAudioSystem::PlaySound(const std::string& soundName, float volume) {
    if (!m_initialized) return;
    
    float finalVolume = volume * m_sfxVolume * m_masterVolume;
    std::cout << "Playing sound: " << soundName << " (Volume: " << finalVolume << ")" << std::endl;
}

void CyborAudioSystem::PlayBackgroundMusic(const std::string& musicFile) {
    if (!m_initialized || !m_backgroundMusicEnabled) return;
    
    m_currentMusic = musicFile;
    float finalVolume = m_musicVolume * m_masterVolume;
    std::cout << "Playing background music: " << musicFile << " (Volume: " << finalVolume << ")" << std::endl;
}

void CyborAudioSystem::StopMusic() {
    if (!m_initialized) return;
    
    m_currentMusic = "";
    std::cout << "Stopped background music" << std::endl;
}

void CyborAudioSystem::SetMasterVolume(float volume) {
    m_masterVolume = std::clamp(volume, 0.0f, 1.0f);
    std::cout << "Master volume set to: " << m_masterVolume << std::endl;
}

void CyborAudioSystem::SetMusicVolume(float volume) {
    m_musicVolume = std::clamp(volume, 0.0f, 1.0f);
    std::cout << "Music volume set to: " << m_musicVolume << std::endl;
}

void CyborAudioSystem::SetSFXVolume(float volume) {
    m_sfxVolume = std::clamp(volume, 0.0f, 1.0f);
    std::cout << "SFX volume set to: " << m_sfxVolume << std::endl;
}

void CyborAudioSystem::EnableBackgroundMusic(bool enable) {
    m_backgroundMusicEnabled = enable;
    if (!enable) {
        StopMusic();
    }
    std::cout << "Background music " << (enable ? "enabled" : "disabled") << std::endl;
}

void CyborAudioSystem::Shutdown() {
    if (m_initialized) {
        StopMusic();
        m_initialized = false;
        std::cout << "Cybor Audio System shut down" << std::endl;
    }
} 