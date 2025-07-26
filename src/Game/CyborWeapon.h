#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

/*
 * CyborWeapon - Advanced tactical weapon system
 * Implements Counter-Strike inspired ballistics with Cybor enhancements
 */
class CyborWeapon {
public:
    enum class WeaponType {
        PISTOL,
        RIFLE,
        SMG,
        SHOTGUN,
        SNIPER,
        GRENADE,
        KNIFE,
        CYBOR_PLASMA,
        CYBOR_RAILGUN
    };

    enum class FireMode {
        SINGLE,
        BURST,
        AUTO,
        CYBOR_ENHANCED
    };

public:
    CyborWeapon(const std::string& name, WeaponType type);
    ~CyborWeapon();

    // Weapon actions
    bool Fire(const glm::vec3& origin, const glm::vec3& direction);
    void Reload();
    void Update(float deltaTime);

    // Weapon properties
    void SetDamage(float damage) { m_damage = damage; }
    void SetFireRate(float fireRate) { m_fireRate = fireRate; }
    void SetAccuracy(float accuracy) { m_accuracy = accuracy; }
    void SetRange(float range) { m_range = range; }
    void SetRecoil(float recoil) { m_recoilAmount = recoil; }

    // Ammo management
    void SetAmmo(int currentAmmo, int maxAmmo, int reserveAmmo);
    bool HasAmmo() const { return m_currentAmmo > 0; }
    bool CanReload() const { return m_currentAmmo < m_maxAmmo && m_reserveAmmo > 0; }

    // Getters
    std::string GetName() const { return m_name; }
    WeaponType GetType() const { return m_type; }
    FireMode GetFireMode() const { return m_fireMode; }
    float GetDamage() const { return m_damage; }
    float GetAccuracy() const { return m_accuracy; }
    float GetRange() const { return m_range; }
    int GetCurrentAmmo() const { return m_currentAmmo; }
    int GetMaxAmmo() const { return m_maxAmmo; }
    int GetReserveAmmo() const { return m_reserveAmmo; }
    bool IsReloading() const { return m_isReloading; }
    float GetReloadProgress() const { return m_reloadProgress; }

    // Cybor enhancements
    void EnableCyborMode(bool enable) { m_cyborModeEnabled = enable; }
    void SetCyborDamageMultiplier(float multiplier) { m_cyborDamageMultiplier = multiplier; }
    void SetCyborAccuracyBonus(float bonus) { m_cyborAccuracyBonus = bonus; }

private:
    // Basic properties
    std::string m_name;
    WeaponType m_type;
    FireMode m_fireMode;

    // Combat stats
    float m_damage;
    float m_fireRate;
    float m_accuracy;
    float m_range;
    float m_recoilAmount;

    // Ammo system
    int m_currentAmmo;
    int m_maxAmmo;
    int m_reserveAmmo;

    // State management
    float m_lastFireTime;
    bool m_isReloading;
    float m_reloadTime;
    float m_reloadProgress;

    // Recoil pattern (Counter-Strike style)
    std::vector<glm::vec2> m_recoilPattern;
    int m_currentRecoilIndex;
    float m_recoilRecoveryRate;

    // Cybor enhancements
    bool m_cyborModeEnabled;
    float m_cyborDamageMultiplier;
    float m_cyborAccuracyBonus;
    float m_cyborFireRateBonus;

    // Audio and visual effects
    std::string m_fireSound;
    std::string m_reloadSound;
    std::string m_emptySound;

    // Private methods
    glm::vec3 CalculateSpread(const glm::vec3& direction);
    void ApplyRecoil();
    bool ProcessFireRate();
    void StartReload();
    void FinishReload();
};

// Pre-defined weapon configurations
namespace CyborWeapons {
    std::shared_ptr<CyborWeapon> CreateAK47();
    std::shared_ptr<CyborWeapon> CreateM4A1();
    std::shared_ptr<CyborWeapon> CreateAWP();
    std::shared_ptr<CyborWeapon> CreateGlock();
    std::shared_ptr<CyborWeapon> CreateUSP();
    std::shared_ptr<CyborWeapon> CreateCyborPlasmaRifle();
    std::shared_ptr<CyborWeapon> CreateCyborRailgun();
}
