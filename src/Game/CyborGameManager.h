#pragma once

#include "../Engine/CyborEngine.h"
#include "CyborPlayer.h"
#include "CyborWeaponSystem.h"
#include "CyborMap.h"
#include "CyborBot.h"
#include "CyborGameMode.h"
#include <vector>
#include <memory>
#include <string>

/*
 * CyborGameManager - Central game logic and state management
 * Handles campaign progression, bot AI, and tactical gameplay
 */
class CyborGameManager {
public:
    enum class GameState {
        MENU,
        LOADING,
        PLAYING,
        PAUSED,
        GAME_OVER,
        CAMPAIGN_COMPLETE
    };

    enum class Team {
        NONE,
        CYBOR_TERRORISTS,
        CYBOR_COUNTER_TERRORISTS
    };

public:
    CyborGameManager(CyborEngine* engine);
    ~CyborGameManager();

    bool Initialize();
    void Update(float deltaTime);
    void Render();
    void Shutdown();

    // Game state management
    void SetGameState(GameState state) { m_gameState = state; }
    GameState GetGameState() const { return m_gameState; }

    // Campaign management
    void StartCampaign();
    void LoadNextMission();
    bool IsCampaignComplete() const { return m_currentMission >= m_totalMissions; }

    // Match management
    void StartMatch(const std::string& mapName);
    void EndMatch(Team winningTeam);
    void RestartMatch();

    // Bot management
    void SpawnBot(Team team, const glm::vec3& position);
    void UpdateBots(float deltaTime);
    size_t GetBotCount() const { return m_bots.size(); }

    // Player management
    CyborPlayer* GetPlayer() { return m_player.get(); }

    // Game statistics
    int GetPlayerScore() const { return m_playerScore; }
    int GetEnemiesKilled() const { return m_enemiesKilled; }
    float GetMatchTime() const { return m_matchTime; }

    // Cybor tactical features
    void EnableCyborTacticalMode(bool enable) { m_cyborTacticalMode = enable; }
    void UpdateCyborIntelligence(float deltaTime);

private:
    CyborEngine* m_engine;
    GameState m_gameState;

    // Game objects
    std::unique_ptr<CyborPlayer> m_player;
    std::unique_ptr<CyborMap> m_currentMap;
    std::vector<std::unique_ptr<CyborBot>> m_bots;
    std::unique_ptr<CyborGameMode> m_gameMode;

    // Campaign system
    int m_currentMission;
    int m_totalMissions;
    std::vector<std::string> m_campaignMaps;
    std::string m_currentObjective;

    // Match statistics
    int m_playerScore;
    int m_enemiesKilled;
    float m_matchTime;
    float m_roundTime;
    Team m_playerTeam;

    // Cybor enhancements
    bool m_cyborTacticalMode;
    float m_cyborAIIntelligence;

    // Private methods
    void InitializeCampaign();
    void ProcessGameLogic(float deltaTime);
    void CheckWinConditions();
    void HandlePlayerInput(float deltaTime);
    void UpdateUI();
    void RenderHUD();
};
