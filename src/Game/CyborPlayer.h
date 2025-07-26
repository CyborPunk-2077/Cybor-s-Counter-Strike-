#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Engine/CyborEngine.h"
#include "CyborWeapon.h"
#include <memory>

/*
 * CyborPlayer - Advanced tactical player character
 * Enhanced with Cybor augmentations and Counter-Strike gameplay mechanics
 */
class CyborPlayer {
public:
    enum class MovementState {
        IDLE,
        WALKING,
        RUNNING,
        CROUCHING,
        JUMPING,
        CYBOR_ENHANCED_MODE
    };

    enum class StanceState {
        STANDING,
        CROUCHING,
        PRONE
    };

public:
    CyborPlayer();
    ~CyborPlayer();

    bool Initialize(const glm::vec3& spawnPosition);
    void Update(float deltaTime, CyborEngine* engine);
    void Render();

    // Movement and physics
    void ProcessMovement(float deltaTime, CyborEngine* engine);
    void ProcessMouseLook(const glm::vec2& mouseDelta, float sensitivity = 0.002f);
    void Jump();
    void Crouch(bool crouching);

    // Combat system
    void Shoot();
    void Reload();
    void SwitchWeapon(int weaponIndex);
    void PickupWeapon(std::shared_ptr<CyborWeapon> weapon);

    // Health and armor system
    void TakeDamage(float damage, const glm::vec3& hitDirection = glm::vec3(0));
    void Heal(float amount);
    void AddArmor(float amount);
    bool IsAlive() const { return m_health > 0.0f; }

    // Getters
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetVelocity() const { return m_velocity; }
    glm::vec3 GetForward() const { return m_forward; }
    glm::vec3 GetRight() const { return m_right; }
    glm::vec3 GetUp() const { return m_up; }
    glm::mat4 GetViewMatrix() const;

    float GetHealth() const { return m_health; }
    float GetArmor() const { return m_armor; }
    int GetMoney() const { return m_money; }
    int GetKills() const { return m_kills; }
    int GetDeaths() const { return m_deaths; }

    MovementState GetMovementState() const { return m_movementState; }
    StanceState GetStanceState() const { return m_stanceState; }

    // Current weapon
    std::shared_ptr<CyborWeapon> GetCurrentWeapon() const;

    // Cybor enhancements
    void EnableCyborMode(bool enable) { m_cyborModeEnabled = enable; }
    void SetCyborEnhancement(float level) { m_cyborEnhancementLevel = level; }
    bool IsCyborModeEnabled() const { return m_cyborModeEnabled; }

private:
    // Position and orientation
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;

    float m_yaw;   // Horizontal rotation
    float m_pitch; // Vertical rotation

    // Player stats
    float m_health;
    float m_maxHealth;
    float m_armor;
    float m_maxArmor;
    int m_money;
    int m_kills;
    int m_deaths;

    // Movement properties
    MovementState m_movementState;
    StanceState m_stanceState;
    float m_walkSpeed;
    float m_runSpeed;
    float m_crouchSpeed;
    float m_jumpHeight;
    float m_mouseSensitivity;
    bool m_isOnGround;
    bool m_isCrouching;

    // Weapon system
    std::vector<std::shared_ptr<CyborWeapon>> m_weapons;
    int m_currentWeaponIndex;
    float m_shootCooldown;

    // Physics
    float m_gravity;
    glm::vec3 m_groundNormal;

    // Cybor enhancements
    bool m_cyborModeEnabled;
    float m_cyborEnhancementLevel;
    float m_cyborSpeedMultiplier;
    float m_cyborHealthRegenRate;

    // Private methods
    void UpdateOrientation();
    void UpdatePhysics(float deltaTime);
    void HandleCollisions();
    void RegenerateHealth(float deltaTime);
    void ProcessInput(CyborEngine* engine, float deltaTime);
};
