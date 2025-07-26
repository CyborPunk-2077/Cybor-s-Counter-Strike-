#include "CyborPlayer.h"
#include <iostream>
#include <algorithm>

CyborPlayer::CyborPlayer()
    : m_position(0.0f), m_velocity(0.0f), m_forward(0.0f, 0.0f, -1.0f),
      m_right(1.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f),
      m_yaw(-90.0f), m_pitch(0.0f),
      m_health(100.0f), m_maxHealth(100.0f), m_armor(100.0f), m_maxArmor(100.0f),
      m_money(800), m_kills(0), m_deaths(0),
      m_movementState(MovementState::IDLE), m_stanceState(StanceState::STANDING),
      m_walkSpeed(2.5f), m_runSpeed(5.0f), m_crouchSpeed(1.5f), m_jumpHeight(2.0f),
      m_mouseSensitivity(0.002f), m_isOnGround(true), m_isCrouching(false),
      m_currentWeaponIndex(0), m_shootCooldown(0.0f),
      m_gravity(-9.81f), m_groundNormal(0.0f, 1.0f, 0.0f),
      m_cyborModeEnabled(false), m_cyborEnhancementLevel(1.0f),
      m_cyborSpeedMultiplier(1.0f), m_cyborHealthRegenRate(0.0f) {
}

CyborPlayer::~CyborPlayer() {
}

bool CyborPlayer::Initialize(const glm::vec3& spawnPosition) {
    m_position = spawnPosition;
    m_health = m_maxHealth;
    m_armor = m_maxArmor;
    m_velocity = glm::vec3(0.0f);
    m_isOnGround = true;
    m_isCrouching = false;

    // Initialize weapons
    // TODO: Add weapon initialization here
    m_currentWeaponIndex = 0;

    std::cout << "Cybor Player initialized at position (" 
              << spawnPosition.x << ", " << spawnPosition.y << ", " << spawnPosition.z << ")" << std::endl;
    return true;
}

void CyborPlayer::Update(float deltaTime, CyborEngine* engine) {
    if (!IsAlive()) return;

    ProcessInput(engine, deltaTime);
    ProcessMovement(deltaTime, engine);
    UpdatePhysics(deltaTime);
    UpdateOrientation();

    // Update shoot cooldown
    if (m_shootCooldown > 0.0f) {
        m_shootCooldown -= deltaTime;
    }

    // Cybor health regeneration
    if (m_cyborModeEnabled && m_cyborHealthRegenRate > 0.0f) {
        RegenerateHealth(deltaTime);
    }
}

void CyborPlayer::ProcessInput(CyborEngine* engine, float deltaTime) {
    if (!engine) return;

    // Movement input
    glm::vec3 moveDirection(0.0f);
    
    if (engine->IsKeyPressed(GLFW_KEY_W)) moveDirection += m_forward;
    if (engine->IsKeyPressed(GLFW_KEY_S)) moveDirection -= m_forward;
    if (engine->IsKeyPressed(GLFW_KEY_A)) moveDirection -= m_right;
    if (engine->IsKeyPressed(GLFW_KEY_D)) moveDirection += m_right;

    // Normalize movement direction
    if (glm::length(moveDirection) > 0.0f) {
        moveDirection = glm::normalize(moveDirection);
        
        // Determine movement speed
        float speed = m_walkSpeed;
        if (engine->IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            speed = m_runSpeed;
            m_movementState = MovementState::RUNNING;
        } else if (glm::length(moveDirection) > 0.0f) {
            m_movementState = MovementState::WALKING;
        }

        // Apply Cybor speed multiplier
        if (m_cyborModeEnabled) {
            speed *= m_cyborSpeedMultiplier;
            m_movementState = MovementState::CYBOR_ENHANCED_MODE;
        }

        // Apply movement
        m_velocity += moveDirection * speed * deltaTime;
    } else {
        m_movementState = MovementState::IDLE;
    }

    // Jump
    if (engine->IsKeyPressed(GLFW_KEY_SPACE) && m_isOnGround) {
        Jump();
    }

    // Crouch
    if (engine->IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        Crouch(true);
    } else {
        Crouch(false);
    }

    // Mouse look
    glm::vec2 mouseDelta = engine->GetMouseDelta();
    ProcessMouseLook(mouseDelta, m_mouseSensitivity);

    // Shooting
    if (engine->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && m_shootCooldown <= 0.0f) {
        Shoot();
    }

    // Reload
    if (engine->IsKeyPressed(GLFW_KEY_R)) {
        Reload();
    }

    // Weapon switching
    if (engine->IsKeyPressed(GLFW_KEY_1)) SwitchWeapon(0);
    if (engine->IsKeyPressed(GLFW_KEY_2)) SwitchWeapon(1);
    if (engine->IsKeyPressed(GLFW_KEY_3)) SwitchWeapon(2);

    // Cybor mode toggle
    if (engine->IsKeyPressed(GLFW_KEY_C)) {
        EnableCyborMode(!m_cyborModeEnabled);
        std::cout << "Cybor Mode: " << (m_cyborModeEnabled ? "ENABLED" : "DISABLED") << std::endl;
    }
}

void CyborPlayer::ProcessMovement(float deltaTime, CyborEngine* engine) {
    // Apply velocity
    m_position += m_velocity * deltaTime;

    // Apply friction
    float friction = 0.9f;
    m_velocity *= friction;

    // Ground collision (simple)
    if (m_position.y < 1.8f) {
        m_position.y = 1.8f;
        m_velocity.y = 0.0f;
        m_isOnGround = true;
    } else {
        m_isOnGround = false;
    }
}

void CyborPlayer::ProcessMouseLook(const glm::vec2& mouseDelta, float sensitivity) {
    m_yaw += mouseDelta.x * sensitivity;
    m_pitch -= mouseDelta.y * sensitivity;

    // Clamp pitch
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    // Wrap yaw
    if (m_yaw > 360.0f) m_yaw -= 360.0f;
    if (m_yaw < -360.0f) m_yaw += 360.0f;
}

void CyborPlayer::UpdateOrientation() {
    // Calculate forward vector
    m_forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward.y = sin(glm::radians(m_pitch));
    m_forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward = glm::normalize(m_forward);

    // Calculate right vector
    m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Calculate up vector
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void CyborPlayer::UpdatePhysics(float deltaTime) {
    // Apply gravity
    if (!m_isOnGround) {
        m_velocity.y += m_gravity * deltaTime;
    }
}

void CyborPlayer::Jump() {
    if (m_isOnGround) {
        m_velocity.y = m_jumpHeight;
        m_isOnGround = false;
        m_movementState = MovementState::JUMPING;
    }
}

void CyborPlayer::Crouch(bool crouching) {
    if (crouching && !m_isCrouching) {
        m_isCrouching = true;
        m_stanceState = StanceState::CROUCHING;
        m_position.y *= 0.7f; // Reduce height
    } else if (!crouching && m_isCrouching) {
        m_isCrouching = false;
        m_stanceState = StanceState::STANDING;
        m_position.y /= 0.7f; // Restore height
    }
}

void CyborPlayer::Shoot() {
    if (m_shootCooldown > 0.0f) return;

    auto currentWeapon = GetCurrentWeapon();
    if (currentWeapon && currentWeapon->CanShoot()) {
        currentWeapon->Shoot();
        m_shootCooldown = currentWeapon->GetFireRate();
        
        std::cout << "Player fired weapon!" << std::endl;
    }
}

void CyborPlayer::Reload() {
    auto currentWeapon = GetCurrentWeapon();
    if (currentWeapon) {
        currentWeapon->Reload();
        std::cout << "Reloading weapon..." << std::endl;
    }
}

void CyborPlayer::SwitchWeapon(int weaponIndex) {
    if (weaponIndex >= 0 && weaponIndex < m_weapons.size()) {
        m_currentWeaponIndex = weaponIndex;
        std::cout << "Switched to weapon " << weaponIndex << std::endl;
    }
}

void CyborPlayer::PickupWeapon(std::shared_ptr<CyborWeapon> weapon) {
    if (weapon) {
        m_weapons.push_back(weapon);
        std::cout << "Picked up weapon: " << weapon->GetName() << std::endl;
    }
}

void CyborPlayer::TakeDamage(float damage, const glm::vec3& hitDirection) {
    if (!IsAlive()) return;

    // Armor absorbs damage first
    float damageToHealth = damage;
    if (m_armor > 0.0f) {
        float armorAbsorption = std::min(m_armor, damage * 0.5f);
        m_armor -= armorAbsorption;
        damageToHealth -= armorAbsorption;
    }

    // Apply remaining damage to health
    if (damageToHealth > 0.0f) {
        m_health -= damageToHealth;
    }

    std::cout << "Player took " << damage << " damage. Health: " << m_health << ", Armor: " << m_armor << std::endl;

    if (!IsAlive()) {
        m_deaths++;
        std::cout << "Player died!" << std::endl;
    }
}

void CyborPlayer::Heal(float amount) {
    m_health = std::min(m_maxHealth, m_health + amount);
}

void CyborPlayer::AddArmor(float amount) {
    m_armor = std::min(m_maxArmor, m_armor + amount);
}

void CyborPlayer::RegenerateHealth(float deltaTime) {
    if (m_health < m_maxHealth) {
        m_health = std::min(m_maxHealth, m_health + m_cyborHealthRegenRate * deltaTime);
    }
}

glm::mat4 CyborPlayer::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

std::shared_ptr<CyborWeapon> CyborPlayer::GetCurrentWeapon() const {
    if (m_currentWeaponIndex >= 0 && m_currentWeaponIndex < m_weapons.size()) {
        return m_weapons[m_currentWeaponIndex];
    }
    return nullptr;
}

void CyborPlayer::Render() {
    // Player rendering would go here
    // For now, we'll just update the engine's view matrix
    // This would typically be done in the game manager
} 