#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "CyborWeapon.h"

/*
 * CyborBot - Advanced AI bot system for tactical gameplay
 * Features sophisticated AI behaviors and Cybor tactical intelligence
 */
class CyborBot {
public:
    enum class BotDifficulty {
        EASY,
        NORMAL,
        HARD,
        EXPERT,
        CYBOR_ELITE
    };

    enum class BotState {
        IDLE,
        PATROLLING,
        SEARCHING,
        ENGAGING,
        RETREATING,
        DEFENDING,
        CYBOR_TACTICAL_MODE
    };

    enum class Team {
        TERRORIST,
        COUNTER_TERRORIST,
        CYBOR_ENHANCED
    };

public:
    CyborBot(const std::string& name, Team team, BotDifficulty difficulty);
    ~CyborBot();

    bool Initialize(const glm::vec3& spawnPosition);
    void Update(float deltaTime, const glm::vec3& playerPosition);
    void Render();

    // AI Behavior
    void SetTarget(const glm::vec3& target);
    void SetWaypoints(const std::vector<glm::vec3>& waypoints);
    void AddWaypoint(const glm::vec3& waypoint);

    // Combat AI
    void EngageTarget(const glm::vec3& targetPosition);
    void TakeCover();
    void Retreat();
    bool CanSeeTarget(const glm::vec3& targetPosition);

    // Movement AI
    void MoveTo(const glm::vec3& destination);
    void Patrol();
    void StopMovement();

    // Combat system
    void Shoot(const glm::vec3& target);
    void Reload();
    void SwitchWeapon();
    void TakeDamage(float damage, const glm::vec3& hitDirection = glm::vec3(0));

    // Getters
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetForward() const { return m_forward; }
    Team GetTeam() const { return m_team; }
    BotDifficulty GetDifficulty() const { return m_difficulty; }
    BotState GetState() const { return m_currentState; }
    float GetHealth() const { return m_health; }
    bool IsAlive() const { return m_health > 0.0f; }
    bool IsPlayerVisible() const { return m_playerVisible; }

    // Cybor AI enhancements
    void EnableCyborAI(bool enable);
    void SetCyborIntelligence(float level);
    void UpdateCyborTacticalAnalysis(float deltaTime);

private:
    // Basic properties
    std::string m_name;
    Team m_team;
    BotDifficulty m_difficulty;
    BotState m_currentState;

    // Position and movement
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_target;

    float m_yaw;
    float m_pitch;

    // Health and combat
    float m_health;
    float m_maxHealth;
    float m_armor;
    std::shared_ptr<CyborWeapon> m_currentWeapon;
    std::vector<std::shared_ptr<CyborWeapon>> m_weapons;

    // AI properties
    float m_viewDistance;
    float m_fieldOfView;
    float m_reactionTime;
    float m_accuracy;
    float m_movementSpeed;

    // Pathfinding and navigation
    std::vector<glm::vec3> m_waypoints;
    int m_currentWaypointIndex;
    glm::vec3 m_destination;
    bool m_hasPath;

    // State management
    float m_stateTimer;
    float m_lastShotTime;
    float m_lastSeenPlayerTime;
    bool m_playerVisible;
    glm::vec3 m_lastKnownPlayerPosition;

    // Cybor AI enhancements
    bool m_cyborAIEnabled;
    float m_cyborIntelligenceLevel;
    float m_cyborPredictionAccuracy;
    float m_cyborTacticalAwareness;

    // Communication and coordination
    std::vector<glm::vec3> m_knownEnemyPositions;
    float m_communicationRange;

    // Private AI methods
    void UpdateAI(float deltaTime, const glm::vec3& playerPosition);
    void UpdateMovement(float deltaTime);
    void UpdateCombat(float deltaTime);
    void UpdateVision(const glm::vec3& playerPosition);

    // State-specific behaviors
    void UpdateIdleBehavior(float deltaTime);
    void UpdatePatrolBehavior(float deltaTime);
    void UpdateSearchBehavior(float deltaTime);
    void UpdateEngageBehavior(float deltaTime, const glm::vec3& playerPosition);
    void UpdateRetreatBehavior(float deltaTime);
    void UpdateDefendBehavior(float deltaTime);

    // Utility functions
    float DistanceToPlayer(const glm::vec3& playerPosition);
    glm::vec3 GetDirectionToPlayer(const glm::vec3& playerPosition);
    bool IsAtDestination(float threshold = 1.0f);
    void RotateTowards(const glm::vec3& target, float deltaTime);
    glm::vec3 FindCoverPosition();

    // Cybor-specific AI
    void PredictPlayerMovement(const glm::vec3& playerPosition, const glm::vec3& playerVelocity);
    void AnalyzeTacticalSituation();
    void ExecuteCyborTactics();
};
