#include "CyborWeapon.h"
#include <iostream>
#include <cmath>
#include <random>

CyborWeapon::CyborWeapon(const std::string& name, WeaponType type)
    : m_name(name), m_type(type), m_fireMode(FireMode::SINGLE),
      m_damage(25.0f), m_fireRate(0.1f), m_accuracy(0.9f), m_range(100.0f), m_recoilAmount(1.0f),
      m_currentAmmo(30), m_maxAmmo(30), m_reserveAmmo(90),
      m_lastFireTime(0.0f), m_isReloading(false), m_reloadTime(2.0f), m_reloadProgress(0.0f),
      m_currentRecoilIndex(0), m_recoilRecoveryRate(0.1f),
      m_cyborModeEnabled(false), m_cyborDamageMultiplier(1.5f), m_cyborAccuracyBonus(0.1f), m_cyborFireRateBonus(0.2f),
      m_fireSound(""), m_reloadSound(""), m_emptySound("") {
    
    // Initialize recoil pattern based on weapon type
    InitializeRecoilPattern();
}

CyborWeapon::~CyborWeapon() {
}

void CyborWeapon::InitializeRecoilPattern() {
    m_recoilPattern.clear();
    
    switch (m_type) {
        case WeaponType::RIFLE:
            // AK-47 style recoil pattern
            for (int i = 0; i < 30; i++) {
                float x = (i * 0.5f) * (i % 2 == 0 ? 1.0f : -1.0f);
                float y = i * 0.3f;
                m_recoilPattern.push_back(glm::vec2(x, y));
            }
            break;
            
        case WeaponType::SMG:
            // SMG style recoil pattern
            for (int i = 0; i < 25; i++) {
                float x = (i * 0.3f) * (i % 2 == 0 ? 1.0f : -1.0f);
                float y = i * 0.2f;
                m_recoilPattern.push_back(glm::vec2(x, y));
            }
            break;
            
        case WeaponType::PISTOL:
            // Pistol style recoil pattern
            for (int i = 0; i < 15; i++) {
                float x = (i * 0.2f) * (i % 2 == 0 ? 1.0f : -1.0f);
                float y = i * 0.1f;
                m_recoilPattern.push_back(glm::vec2(x, y));
            }
            break;
            
        case WeaponType::CYBOR_PLASMA:
            // Cybor plasma rifle - minimal recoil
            for (int i = 0; i < 20; i++) {
                float x = (i * 0.1f) * (i % 2 == 0 ? 1.0f : -1.0f);
                float y = i * 0.05f;
                m_recoilPattern.push_back(glm::vec2(x, y));
            }
            break;
            
        default:
            // Default recoil pattern
            for (int i = 0; i < 10; i++) {
                float x = (i * 0.1f) * (i % 2 == 0 ? 1.0f : -1.0f);
                float y = i * 0.1f;
                m_recoilPattern.push_back(glm::vec2(x, y));
            }
            break;
    }
}

bool CyborWeapon::Fire(const glm::vec3& origin, const glm::vec3& direction) {
    if (!HasAmmo() || m_isReloading) {
        if (!HasAmmo()) {
            std::cout << "Weapon empty! Reload needed." << std::endl;
        }
        return false;
    }

    if (!ProcessFireRate()) {
        return false;
    }

    // Calculate spread based on accuracy
    glm::vec3 spreadDirection = CalculateSpread(direction);
    
    // Apply Cybor enhancements
    float finalDamage = m_damage;
    if (m_cyborModeEnabled) {
        finalDamage *= m_cyborDamageMultiplier;
    }

    // Consume ammo
    m_currentAmmo--;

    // Apply recoil
    ApplyRecoil();

    std::cout << m_name << " fired! Ammo: " << m_currentAmmo << "/" << m_maxAmmo 
              << " (Reserve: " << m_reserveAmmo << ")" << std::endl;

    return true;
}

void CyborWeapon::Reload() {
    if (CanReload() && !m_isReloading) {
        StartReload();
    }
}

void CyborWeapon::Update(float deltaTime) {
    // Update reload progress
    if (m_isReloading) {
        m_reloadProgress += deltaTime / m_reloadTime;
        
        if (m_reloadProgress >= 1.0f) {
            FinishReload();
        }
    }

    // Recover recoil
    if (m_currentRecoilIndex > 0) {
        m_currentRecoilIndex = std::max(0, m_currentRecoilIndex - 1);
    }
}

glm::vec3 CyborWeapon::CalculateSpread(const glm::vec3& direction) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Calculate base accuracy
    float accuracy = m_accuracy;
    if (m_cyborModeEnabled) {
        accuracy += m_cyborAccuracyBonus;
    }
    
    // Clamp accuracy
    accuracy = std::clamp(accuracy, 0.0f, 1.0f);
    
    // Calculate spread angle based on accuracy
    float spreadAngle = (1.0f - accuracy) * 0.1f; // 0.1 radians max spread
    
    std::uniform_real_distribution<float> dist(-spreadAngle, spreadAngle);
    
    // Apply random spread
    glm::vec3 spreadDirection = direction;
    spreadDirection.x += dist(gen);
    spreadDirection.y += dist(gen);
    spreadDirection.z += dist(gen);
    
    return glm::normalize(spreadDirection);
}

void CyborWeapon::ApplyRecoil() {
    if (m_currentRecoilIndex < m_recoilPattern.size()) {
        m_currentRecoilIndex++;
    }
}

bool CyborWeapon::ProcessFireRate() {
    static float currentTime = 0.0f;
    currentTime += 0.016f; // Assume 60 FPS
    
    float fireRate = m_fireRate;
    if (m_cyborModeEnabled) {
        fireRate *= (1.0f - m_cyborFireRateBonus);
    }
    
    if (currentTime - m_lastFireTime < fireRate) {
        return false;
    }
    
    m_lastFireTime = currentTime;
    return true;
}

void CyborWeapon::StartReload() {
    m_isReloading = true;
    m_reloadProgress = 0.0f;
    std::cout << "Reloading " << m_name << "..." << std::endl;
}

void CyborWeapon::FinishReload() {
    int ammoNeeded = m_maxAmmo - m_currentAmmo;
    int ammoToAdd = std::min(ammoNeeded, m_reserveAmmo);
    
    m_currentAmmo += ammoToAdd;
    m_reserveAmmo -= ammoToAdd;
    
    m_isReloading = false;
    m_reloadProgress = 0.0f;
    
    std::cout << m_name << " reloaded! Ammo: " << m_currentAmmo << "/" << m_maxAmmo 
              << " (Reserve: " << m_reserveAmmo << ")" << std::endl;
}

void CyborWeapon::SetAmmo(int currentAmmo, int maxAmmo, int reserveAmmo) {
    m_currentAmmo = currentAmmo;
    m_maxAmmo = maxAmmo;
    m_reserveAmmo = reserveAmmo;
}

// Pre-defined weapon configurations
namespace CyborWeapons {
    std::shared_ptr<CyborWeapon> CreateAK47() {
        auto weapon = std::make_shared<CyborWeapon>("AK-47", CyborWeapon::WeaponType::RIFLE);
        weapon->SetDamage(36.0f);
        weapon->SetFireRate(0.1f);
        weapon->SetAccuracy(0.75f);
        weapon->SetRange(150.0f);
        weapon->SetRecoil(1.5f);
        weapon->SetAmmo(30, 30, 90);
        return weapon;
    }

    std::shared_ptr<CyborWeapon> CreateM4A1() {
        auto weapon = std::make_shared<CyborWeapon>("M4A1", CyborWeapon::WeaponType::RIFLE);
        weapon->SetDamage(31.0f);
        weapon->SetFireRate(0.09f);
        weapon->SetAccuracy(0.85f);
        weapon->SetRange(140.0f);
        weapon->SetRecoil(1.2f);
        weapon->SetAmmo(30, 30, 90);
        return weapon;
    }

    std::shared_ptr<CyborWeapon> CreateAWP() {
        auto weapon = std::make_shared<CyborWeapon>("AWP", CyborWeapon::WeaponType::SNIPER);
        weapon->SetDamage(115.0f);
        weapon->SetFireRate(1.5f);
        weapon->SetAccuracy(0.95f);
        weapon->SetRange(300.0f);
        weapon->SetRecoil(3.0f);
        weapon->SetAmmo(10, 10, 30);
        return weapon;
    }

    std::shared_ptr<CyborWeapon> CreateGlock() {
        auto weapon = std::make_shared<CyborWeapon>("Glock-18", CyborWeapon::WeaponType::PISTOL);
        weapon->SetDamage(28.0f);
        weapon->SetFireRate(0.15f);
        weapon->SetAccuracy(0.8f);
        weapon->SetRange(80.0f);
        weapon->SetRecoil(0.8f);
        weapon->SetAmmo(20, 20, 120);
        return weapon;
    }

    std::shared_ptr<CyborWeapon> CreateUSP() {
        auto weapon = std::make_shared<CyborWeapon>("USP-S", CyborWeapon::WeaponType::PISTOL);
        weapon->SetDamage(35.0f);
        weapon->SetFireRate(0.2f);
        weapon->SetAccuracy(0.9f);
        weapon->SetRange(90.0f);
        weapon->SetRecoil(0.6f);
        weapon->SetAmmo(12, 12, 100);
        return weapon;
    }

    std::shared_ptr<CyborWeapon> CreateCyborPlasmaRifle() {
        auto weapon = std::make_shared<CyborWeapon>("Cybor Plasma Rifle", CyborWeapon::WeaponType::CYBOR_PLASMA);
        weapon->SetDamage(45.0f);
        weapon->SetFireRate(0.08f);
        weapon->SetAccuracy(0.92f);
        weapon->SetRange(200.0f);
        weapon->SetRecoil(0.5f);
        weapon->SetAmmo(40, 40, 120);
        weapon->EnableCyborMode(true);
        return weapon;
    }

    std::shared_ptr<CyborWeapon> CreateCyborRailgun() {
        auto weapon = std::make_shared<CyborWeapon>("Cybor Railgun", CyborWeapon::WeaponType::CYBOR_RAILGUN);
        weapon->SetDamage(150.0f);
        weapon->SetFireRate(2.0f);
        weapon->SetAccuracy(0.98f);
        weapon->SetRange(500.0f);
        weapon->SetRecoil(2.5f);
        weapon->SetAmmo(5, 5, 20);
        weapon->EnableCyborMode(true);
        return weapon;
    }
} 