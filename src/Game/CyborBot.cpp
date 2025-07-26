#include "CyborBot.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>

CyborBot::CyborBot(const std::string& name, Team team, BotDifficulty difficulty)
    : m_name(name), m_team(team), m_difficulty(difficulty), m_currentState(BotState::IDLE),
      m_position(0.0f), m_velocity(0.0f), m_forward(0.0f, 0.0f, -1.0f),
      m_right(1.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f), m_target(0.0f),
      m_yaw(-90.0f), m_pitch(0.0f),
      m_health(100.0f), m_maxHealth(100.0f), m_armor(100.0f),
      m_viewDistance(50.0f), m_fieldOfView(90.0f), m_reactionTime(0.5f), m_accuracy(0.7f), m_movementSpeed(3.0f),
      m_currentWaypointIndex(0), m_hasPath(false),
      m_stateTimer(0.0f), m_lastShotTime(0.0f), m_lastSeenPlayerTime(0.0f), m_playerVisible(false),
      m_cyborAIEnabled(false), m_cyborIntelligenceLevel(1.0f), m_cyborPredictionAccuracy(0.8f), m_cyborTacticalAwareness(0.7f),
      m_communicationRange(30.0f) {
    
    // Set AI properties based on difficulty
    switch (difficulty) {
        case BotDifficulty::EASY:
            m_accuracy = 0.5f;
            m_reactionTime = 1.0f;
            m_movementSpeed = 2.5f;
            break;
        case BotDifficulty::NORMAL:
            m_accuracy = 0.7f;
            m_reactionTime = 0.5f;
            m_movementSpeed = 3.0f;
            break;
        case BotDifficulty::HARD:
            m_accuracy = 0.8f;
            m_reactionTime = 0.3f;
            m_movementSpeed = 3.5f;
            break;
        case BotDifficulty::EXPERT:
            m_accuracy = 0.9f;
            m_reactionTime = 0.2f;
            m_movementSpeed = 4.0f;
            break;
        case BotDifficulty::CYBOR_ELITE:
            m_accuracy = 0.95f;
            m_reactionTime = 0.1f;
            m_movementSpeed = 4.5f;
            m_cyborAIEnabled = true;
            m_cyborIntelligenceLevel = 2.0f;
            break;
    }
}

CyborBot::~CyborBot() {
}

bool CyborBot::Initialize(const glm::vec3& spawnPosition) {
    m_position = spawnPosition;
    m_health = m_maxHealth;
    m_armor = 100.0f;
    m_currentState = BotState::PATROLLING;
    
    // Initialize with a basic weapon
    m_currentWeapon = CyborWeapons::CreateGlock();
    m_weapons.push_back(m_currentWeapon);
    
    // Set up patrol waypoints around spawn
    m_waypoints = {
        spawnPosition + glm::vec3(10.0f, 0.0f, 0.0f),
        spawnPosition + glm::vec3(0.0f, 0.0f, 10.0f),
        spawnPosition + glm::vec3(-10.0f, 0.0f, 0.0f),
        spawnPosition + glm::vec3(0.0f, 0.0f, -10.0f)
    };
    
    std::cout << "Cybor Bot " << m_name << " initialized at position (" 
              << spawnPosition.x << ", " << spawnPosition.y << ", " << spawnPosition.z << ")" << std::endl;
    return true;
}

void CyborBot::Update(float deltaTime, const glm::vec3& playerPosition) {
    if (!IsAlive()) return;

    m_stateTimer += deltaTime;
    
    // Update AI
    UpdateAI(deltaTime, playerPosition);
    
    // Update movement
    UpdateMovement(deltaTime);
    
    // Update combat
    UpdateCombat(deltaTime);
    
    // Update vision
    UpdateVision(playerPosition);
    
    // Update Cybor AI if enabled
    if (m_cyborAIEnabled) {
        UpdateCyborTacticalAnalysis(deltaTime);
    }
}

void CyborBot::UpdateAI(float deltaTime, const glm::vec3& playerPosition) {
    float distanceToPlayer = DistanceToPlayer(playerPosition);
    
    // Check if player is visible
    bool canSeePlayer = CanSeeTarget(playerPosition);
    if (canSeePlayer) {
        m_playerVisible = true;
        m_lastSeenPlayerTime = 0.0f;
        m_lastKnownPlayerPosition = playerPosition;
        
        // Switch to engage state
        if (m_currentState != BotState::ENGAGING) {
            m_currentState = BotState::ENGAGING;
            m_stateTimer = 0.0f;
        }
    } else {
        m_lastSeenPlayerTime += deltaTime;
        
        // If we haven't seen player for a while, start searching
        if (m_lastSeenPlayerTime > 5.0f && m_currentState == BotState::ENGAGING) {
            m_currentState = BotState::SEARCHING;
            m_stateTimer = 0.0f;
        }
    }
    
    // Update state-specific behavior
    switch (m_currentState) {
        case BotState::IDLE:
            UpdateIdleBehavior(deltaTime);
            break;
        case BotState::PATROLLING:
            UpdatePatrolBehavior(deltaTime);
            break;
        case BotState::SEARCHING:
            UpdateSearchBehavior(deltaTime);
            break;
        case BotState::ENGAGING:
            UpdateEngageBehavior(deltaTime, playerPosition);
            break;
        case BotState::RETREATING:
            UpdateRetreatBehavior(deltaTime);
            break;
        case BotState::DEFENDING:
            UpdateDefendBehavior(deltaTime);
            break;
        case BotState::CYBOR_TACTICAL_MODE:
            ExecuteCyborTactics();
            break;
    }
}

void CyborBot::UpdateMovement(float deltaTime) {
    if (m_hasPath) {
        glm::vec3 direction = glm::normalize(m_destination - m_position);
        m_velocity = direction * m_movementSpeed;
        m_position += m_velocity * deltaTime;
        
        // Check if we've reached destination
        if (IsAtDestination()) {
            m_hasPath = false;
            m_velocity = glm::vec3(0.0f);
        }
    }
}

void CyborBot::UpdateCombat(float deltaTime) {
    if (m_currentWeapon) {
        m_currentWeapon->Update(deltaTime);
    }
}

void CyborBot::UpdateVision(const glm::vec3& playerPosition) {
    float distance = DistanceToPlayer(playerPosition);
    
    if (distance <= m_viewDistance) {
        glm::vec3 directionToPlayer = GetDirectionToPlayer(playerPosition);
        float dotProduct = glm::dot(m_forward, directionToPlayer);
        float angle = glm::acos(dotProduct);
        
        if (angle <= glm::radians(m_fieldOfView / 2.0f)) {
            // Check line of sight (simplified)
            m_playerVisible = true;
        } else {
            m_playerVisible = false;
        }
    } else {
        m_playerVisible = false;
    }
}

void CyborBot::UpdateIdleBehavior(float deltaTime) {
    // Switch to patrolling after a short time
    if (m_stateTimer > 2.0f) {
        m_currentState = BotState::PATROLLING;
        m_stateTimer = 0.0f;
    }
}

void CyborBot::UpdatePatrolBehavior(float deltaTime) {
    if (!m_hasPath && m_currentWaypointIndex < m_waypoints.size()) {
        m_destination = m_waypoints[m_currentWaypointIndex];
        m_hasPath = true;
    }
    
    if (IsAtDestination()) {
        m_currentWaypointIndex = (m_currentWaypointIndex + 1) % m_waypoints.size();
        m_hasPath = false;
    }
}

void CyborBot::UpdateSearchBehavior(float deltaTime) {
    // Search around last known player position
    if (!m_hasPath) {
        glm::vec3 searchPoint = m_lastKnownPlayerPosition + glm::vec3(
            (rand() % 20) - 10.0f,
            0.0f,
            (rand() % 20) - 10.0f
        );
        m_destination = searchPoint;
        m_hasPath = true;
    }
    
    // Return to patrolling after search time
    if (m_stateTimer > 10.0f) {
        m_currentState = BotState::PATROLLING;
        m_stateTimer = 0.0f;
    }
}

void CyborBot::UpdateEngageBehavior(float deltaTime, const glm::vec3& playerPosition) {
    // Rotate towards player
    RotateTowards(playerPosition, deltaTime);
    
    // Move towards player if too far
    float distance = DistanceToPlayer(playerPosition);
    if (distance > 10.0f) {
        m_destination = playerPosition;
        m_hasPath = true;
    } else {
        // Stop and shoot
        m_hasPath = false;
        m_velocity = glm::vec3(0.0f);
        
        // Shoot at player
        if (m_stateTimer - m_lastShotTime > m_reactionTime) {
            Shoot(playerPosition);
            m_lastShotTime = m_stateTimer;
        }
    }
    
    // Take cover if health is low
    if (m_health < 30.0f) {
        TakeCover();
    }
}

void CyborBot::UpdateRetreatBehavior(float deltaTime) {
    // Move away from last known player position
    if (!m_hasPath) {
        glm::vec3 retreatDirection = glm::normalize(m_position - m_lastKnownPlayerPosition);
        m_destination = m_position + retreatDirection * 20.0f;
        m_hasPath = true;
    }
    
    // Return to normal behavior after retreat
    if (m_stateTimer > 5.0f) {
        m_currentState = BotState::PATROLLING;
        m_stateTimer = 0.0f;
    }
}

void CyborBot::UpdateDefendBehavior(float deltaTime) {
    // Stay in position and watch for enemies
    m_hasPath = false;
    m_velocity = glm::vec3(0.0f);
    
    // Rotate slowly to scan area
    m_yaw += 0.5f * deltaTime;
    UpdateOrientation();
}

void CyborBot::Shoot(const glm::vec3& target) {
    if (m_currentWeapon && m_currentWeapon->CanShoot()) {
        glm::vec3 direction = glm::normalize(target - m_position);
        
        // Apply accuracy
        if (m_cyborAIEnabled) {
            direction = CalculateSpread(direction);
        }
        
        if (m_currentWeapon->Fire(m_position, direction)) {
            std::cout << m_name << " fired at target!" << std::endl;
        }
    }
}

void CyborBot::TakeDamage(float damage, const glm::vec3& hitDirection) {
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
    
    std::cout << m_name << " took " << damage << " damage. Health: " << m_health << std::endl;
    
    // React to damage
    if (m_health < 50.0f) {
        m_currentState = BotState::RETREATING;
        m_stateTimer = 0.0f;
    }
    
    if (!IsAlive()) {
        std::cout << m_name << " was eliminated!" << std::endl;
    }
}

void CyborBot::EngageTarget(const glm::vec3& targetPosition) {
    m_target = targetPosition;
    m_currentState = BotState::ENGAGING;
    m_stateTimer = 0.0f;
}

void CyborBot::TakeCover() {
    glm::vec3 coverPosition = FindCoverPosition();
    m_destination = coverPosition;
    m_hasPath = true;
    m_currentState = BotState::DEFENDING;
    m_stateTimer = 0.0f;
}

void CyborBot::Retreat() {
    m_currentState = BotState::RETREATING;
    m_stateTimer = 0.0f;
}

bool CyborBot::CanSeeTarget(const glm::vec3& targetPosition) {
    float distance = DistanceToPlayer(targetPosition);
    return distance <= m_viewDistance && m_playerVisible;
}

void CyborBot::MoveTo(const glm::vec3& destination) {
    m_destination = destination;
    m_hasPath = true;
}

void CyborBot::Patrol() {
    m_currentState = BotState::PATROLLING;
    m_stateTimer = 0.0f;
}

void CyborBot::StopMovement() {
    m_hasPath = false;
    m_velocity = glm::vec3(0.0f);
}

void CyborBot::EnableCyborAI(bool enable) {
    m_cyborAIEnabled = enable;
    if (enable) {
        m_currentState = BotState::CYBOR_TACTICAL_MODE;
        std::cout << m_name << " Cybor AI enabled!" << std::endl;
    }
}

void CyborBot::SetCyborIntelligence(float level) {
    m_cyborIntelligenceLevel = level;
    m_accuracy = std::min(0.95f, m_accuracy + (level - 1.0f) * 0.1f);
    m_reactionTime = std::max(0.05f, m_reactionTime - (level - 1.0f) * 0.1f);
}

void CyborBot::UpdateCyborTacticalAnalysis(float deltaTime) {
    // Enhanced tactical awareness
    m_cyborTacticalAwareness = std::min(1.0f, m_cyborTacticalAwareness + deltaTime * 0.1f);
    
    // Predict player movement
    // This would be implemented with more sophisticated prediction algorithms
}

void CyborBot::ExecuteCyborTactics() {
    // Enhanced Cybor tactical behavior
    if (m_cyborIntelligenceLevel > 1.5f) {
        // Use advanced tactics like flanking, coordinated attacks, etc.
        std::cout << m_name << " executing Cybor tactical maneuvers!" << std::endl;
    }
}

// Utility functions
float CyborBot::DistanceToPlayer(const glm::vec3& playerPosition) {
    return glm::length(playerPosition - m_position);
}

glm::vec3 CyborBot::GetDirectionToPlayer(const glm::vec3& playerPosition) {
    return glm::normalize(playerPosition - m_position);
}

bool CyborBot::IsAtDestination(float threshold) {
    return glm::length(m_position - m_destination) < threshold;
}

void CyborBot::RotateTowards(const glm::vec3& target, float deltaTime) {
    glm::vec3 direction = glm::normalize(target - m_position);
    
    // Calculate target yaw and pitch
    float targetYaw = atan2(direction.z, direction.x);
    float targetPitch = asin(direction.y);
    
    // Smooth rotation
    float rotationSpeed = 2.0f;
    m_yaw = glm::mix(m_yaw, targetYaw, rotationSpeed * deltaTime);
    m_pitch = glm::mix(m_pitch, targetPitch, rotationSpeed * deltaTime);
    
    UpdateOrientation();
}

void CyborBot::UpdateOrientation() {
    // Calculate forward vector
    m_forward.x = cos(m_yaw) * cos(m_pitch);
    m_forward.y = sin(m_pitch);
    m_forward.z = sin(m_yaw) * cos(m_pitch);
    m_forward = glm::normalize(m_forward);
    
    // Calculate right vector
    m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    // Calculate up vector
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

glm::vec3 CyborBot::FindCoverPosition() {
    // Simple cover finding - find position away from last known player position
    glm::vec3 awayDirection = glm::normalize(m_position - m_lastKnownPlayerPosition);
    return m_position + awayDirection * 15.0f;
}

glm::vec3 CyborBot::CalculateSpread(const glm::vec3& direction) {
    // Apply accuracy-based spread
    std::random_device rd;
    std::mt19937 gen(rd());
    
    float spreadAngle = (1.0f - m_accuracy) * 0.05f;
    std::uniform_real_distribution<float> dist(-spreadAngle, spreadAngle);
    
    glm::vec3 spreadDirection = direction;
    spreadDirection.x += dist(gen);
    spreadDirection.y += dist(gen);
    spreadDirection.z += dist(gen);
    
    return glm::normalize(spreadDirection);
}

void CyborBot::Render() {
    // Bot rendering would go here
    // For now, we'll just output position info periodically
    static float renderTimer = 0.0f;
    renderTimer += 0.016f;
    
    if (renderTimer > 2.0f) {
        std::cout << m_name << " at (" << m_position.x << ", " << m_position.y << ", " << m_position.z 
                  << ") State: " << (int)m_currentState << " Health: " << m_health << std::endl;
        renderTimer = 0.0f;
    }
} 