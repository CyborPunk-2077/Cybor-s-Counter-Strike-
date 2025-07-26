#include "CyborGameManager.h"
#include <iostream>
#include <algorithm>

CyborGameManager::CyborGameManager(CyborEngine* engine) 
    : m_engine(engine), m_gameState(GameState::MENU),
      m_currentMission(0), m_totalMissions(5),
      m_playerScore(0), m_enemiesKilled(0), m_matchTime(0.0f), m_roundTime(0.0f),
      m_playerTeam(Team::CYBOR_COUNTER_TERRORISTS),
      m_cyborTacticalMode(false), m_cyborAIIntelligence(1.0f) {
}

CyborGameManager::~CyborGameManager() {
    Shutdown();
}

bool CyborGameManager::Initialize() {
    std::cout << "Initializing Cybor Game Manager..." << std::endl;

    // Initialize player
    m_player = std::make_unique<CyborPlayer>();
    if (!m_player->Initialize(glm::vec3(0.0f, 1.8f, 0.0f))) {
        std::cerr << "Failed to initialize player!" << std::endl;
        return false;
    }

    // Initialize campaign
    InitializeCampaign();

    // Set initial game state
    m_gameState = GameState::MENU;

    std::cout << "Cybor Game Manager initialized successfully!" << std::endl;
    return true;
}

void CyborGameManager::InitializeCampaign() {
    // Define campaign missions
    m_campaignMaps = {
        "cybor_compound",
        "cybor_urban_warfare", 
        "cybor_industrial_complex",
        "cybor_arctic_base",
        "cybor_final_assault"
    };

    m_currentMission = 0;
    m_totalMissions = m_campaignMaps.size();

    std::cout << "Campaign initialized with " << m_totalMissions << " missions" << std::endl;
}

void CyborGameManager::Update(float deltaTime) {
    switch (m_gameState) {
        case GameState::MENU:
            // Handle menu input
            if (m_engine->IsKeyPressed(GLFW_KEY_ENTER)) {
                StartCampaign();
            }
            break;

        case GameState::LOADING:
            // Simulate loading
            SetGameState(GameState::PLAYING);
            break;

        case GameState::PLAYING:
            ProcessGameLogic(deltaTime);
            break;

        case GameState::PAUSED:
            if (m_engine->IsKeyPressed(GLFW_KEY_P)) {
                SetGameState(GameState::PLAYING);
            }
            break;

        case GameState::GAME_OVER:
            if (m_engine->IsKeyPressed(GLFW_KEY_R)) {
                RestartMatch();
            }
            break;

        case GameState::CAMPAIGN_COMPLETE:
            std::cout << "Campaign completed! Final score: " << m_playerScore << std::endl;
            break;
    }

    // Update Cybor AI intelligence
    if (m_cyborTacticalMode) {
        UpdateCyborIntelligence(deltaTime);
    }
}

void CyborGameManager::ProcessGameLogic(float deltaTime) {
    m_matchTime += deltaTime;
    m_roundTime += deltaTime;

    // Handle pause
    if (m_engine->IsKeyPressed(GLFW_KEY_P)) {
        SetGameState(GameState::PAUSED);
        return;
    }

    // Update player
    if (m_player) {
        m_player->Update(deltaTime, m_engine);

        // Check if player died
        if (!m_player->IsAlive()) {
            EndMatch(Team::CYBOR_TERRORISTS);
            return;
        }
    }

    // Update bots
    UpdateBots(deltaTime);

    // Update current map
    if (m_currentMap) {
        // Map-specific logic would go here
    }

    // Check win conditions
    CheckWinConditions();

    // Handle user input
    HandlePlayerInput(deltaTime);
}

void CyborGameManager::UpdateBots(float deltaTime) {
    for (auto& bot : m_bots) {
        if (bot && bot->IsAlive()) {
            glm::vec3 playerPos = m_player ? m_player->GetPosition() : glm::vec3(0);
            bot->Update(deltaTime, playerPos);

            // Check if bot can see and should attack player
            if (bot->IsPlayerVisible() && bot->GetTeam() != (CyborBot::Team)m_playerTeam) {
                bot->EngageTarget(playerPos);
            }
        }
    }

    // Remove dead bots
    m_bots.erase(
        std::remove_if(m_bots.begin(), m_bots.end(),
            [](const std::unique_ptr<CyborBot>& bot) {
                return !bot || !bot->IsAlive();
            }),
        m_bots.end()
    );
}

void CyborGameManager::CheckWinConditions() {
    // Count alive enemy bots
    int aliveEnemies = 0;
    for (const auto& bot : m_bots) {
        if (bot && bot->IsAlive() && bot->GetTeam() != (CyborBot::Team)m_playerTeam) {
            aliveEnemies++;
        }
    }

    // Player wins if all enemies are dead
    if (aliveEnemies == 0 && m_gameState == GameState::PLAYING) {
        EndMatch(m_playerTeam);
    }

    // Check time limit (15 minutes per round)
    if (m_roundTime > 900.0f) {
        EndMatch(Team::CYBOR_TERRORISTS); // Terrorists win on time
    }
}

void CyborGameManager::HandlePlayerInput(float deltaTime) {
    // Enable/disable Cybor tactical mode
    if (m_engine->IsKeyPressed(GLFW_KEY_T)) {
        EnableCyborTacticalMode(!m_cyborTacticalMode);
        std::cout << "Cybor Tactical Mode: " << (m_cyborTacticalMode ? "ENABLED" : "DISABLED") << std::endl;
    }

    // Debug: Spawn enemy bot
    if (m_engine->IsKeyPressed(GLFW_KEY_B)) {
        glm::vec3 spawnPos = m_player->GetPosition() + glm::vec3(10.0f, 0.0f, 0.0f);
        SpawnBot(Team::CYBOR_TERRORISTS, spawnPos);
    }
}

void CyborGameManager::StartCampaign() {
    std::cout << "Starting Cybor's Counter Strike Campaign..." << std::endl;
    m_currentMission = 0;
    m_playerScore = 0;
    m_enemiesKilled = 0;
    LoadNextMission();
}

void CyborGameManager::LoadNextMission() {
    if (m_currentMission >= m_totalMissions) {
        SetGameState(GameState::CAMPAIGN_COMPLETE);
        return;
    }

    std::string mapName = m_campaignMaps[m_currentMission];
    std::cout << "Loading Mission " << (m_currentMission + 1) << ": " << mapName << std::endl;

    StartMatch(mapName);
    m_currentMission++;
}

void CyborGameManager::StartMatch(const std::string& mapName) {
    std::cout << "Starting match on map: " << mapName << std::endl;

    // Reset match statistics
    m_matchTime = 0.0f;
    m_roundTime = 0.0f;

    // Clear existing bots
    m_bots.clear();

    // Spawn bots based on mission
    int botCount = 3 + m_currentMission; // Increase difficulty
    for (int i = 0; i < botCount; i++) {
        glm::vec3 spawnPos(
            (rand() % 20) - 10.0f,  // Random X: -10 to 10
            1.8f,                   // Y: Standard height
            (rand() % 20) - 10.0f   // Random Z: -10 to 10
        );
        SpawnBot(Team::CYBOR_TERRORISTS, spawnPos);
    }

    // Set objective based on mission
    switch (m_currentMission) {
        case 0:
            m_currentObjective = "Eliminate all Cybor Terrorists in the compound";
            break;
        case 1:
            m_currentObjective = "Secure the urban warfare zone";
            break;
        case 2:
            m_currentObjective = "Infiltrate the industrial complex";
            break;
        case 3:
            m_currentObjective = "Assault the arctic base";
            break;
        case 4:
            m_currentObjective = "Complete the final assault mission";
            break;
        default:
            m_currentObjective = "Eliminate all enemy forces";
            break;
    }

    SetGameState(GameState::PLAYING);
    std::cout << "Mission Objective: " << m_currentObjective << std::endl;
}

void CyborGameManager::EndMatch(Team winningTeam) {
    std::cout << "Match ended! Winner: ";

    if (winningTeam == m_playerTeam) {
        std::cout << "CYBOR COUNTER-TERRORISTS (Player)" << std::endl;
        m_playerScore += 1000;

        // Load next mission
        if (m_currentMission < m_totalMissions) {
            LoadNextMission();
        } else {
            SetGameState(GameState::CAMPAIGN_COMPLETE);
        }
    } else {
        std::cout << "CYBOR TERRORISTS (AI)" << std::endl;
        SetGameState(GameState::GAME_OVER);
    }

    std::cout << "Match Statistics:" << std::endl;
    std::cout << "- Time: " << (int)m_matchTime << " seconds" << std::endl;
    std::cout << "- Enemies Killed: " << m_enemiesKilled << std::endl;
    std::cout << "- Player Score: " << m_playerScore << std::endl;
}

void CyborGameManager::RestartMatch() {
    std::cout << "Restarting match..." << std::endl;

    // Reset player
    if (m_player) {
        m_player->Initialize(glm::vec3(0.0f, 1.8f, 0.0f));
    }

    // Restart current mission
    m_currentMission--;
    LoadNextMission();
}

void CyborGameManager::SpawnBot(Team team, const glm::vec3& position) {
    std::string botName = "CyborBot_" + std::to_string(m_bots.size() + 1);
    CyborBot::Team botTeam = (team == Team::CYBOR_TERRORISTS) ? 
        CyborBot::Team::TERRORIST : CyborBot::Team::COUNTER_TERRORIST;

    CyborBot::BotDifficulty difficulty = CyborBot::BotDifficulty::NORMAL;

    // Increase difficulty based on mission
    if (m_currentMission >= 2) difficulty = CyborBot::BotDifficulty::HARD;
    if (m_currentMission >= 4) difficulty = CyborBot::BotDifficulty::EXPERT;

    auto bot = std::make_unique<CyborBot>(botName, botTeam, difficulty);
    if (bot->Initialize(position)) {
        if (m_cyborTacticalMode) {
            bot->EnableCyborAI(true);
            bot->SetCyborIntelligence(m_cyborAIIntelligence);
        }
        m_bots.push_back(std::move(bot));
        std::cout << "Spawned " << botName << " at position (" 
                  << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }
}

void CyborGameManager::UpdateCyborIntelligence(float deltaTime) {
    // Gradually increase AI intelligence over time
    m_cyborAIIntelligence = std::min(2.0f, m_cyborAIIntelligence + deltaTime * 0.1f);

    // Update all bots with new intelligence level
    for (auto& bot : m_bots) {
        if (bot) {
            bot->SetCyborIntelligence(m_cyborAIIntelligence);
        }
    }
}

void CyborGameManager::Render() {
    // Render player
    if (m_player) {
        m_player->Render();
    }

    // Render bots
    for (auto& bot : m_bots) {
        if (bot) {
            bot->Render();
        }
    }

    // Render map
    if (m_currentMap) {
        // Map rendering would go here
    }

    // Render UI
    RenderHUD();
}

void CyborGameManager::RenderHUD() {
    // This would integrate with a UI system to display:
    // - Health and armor
    // - Ammo count
    // - Mini-map
    // - Objectives
    // - Score
    // - Cybor mode status

    // For now, we'll just output to console periodically
    static float hudUpdateTimer = 0.0f;
    hudUpdateTimer += 0.016f; // Assume 60 FPS

    if (hudUpdateTimer >= 5.0f) { // Update every 5 seconds
        if (m_gameState == GameState::PLAYING && m_player) {
            std::cout << "\n=== CYBOR HUD ===" << std::endl;
            std::cout << "Health: " << (int)m_player->GetHealth() << "/100" << std::endl;
            std::cout << "Armor: " << (int)m_player->GetArmor() << "/100" << std::endl;
            std::cout << "Enemies: " << m_bots.size() << std::endl;
            std::cout << "Score: " << m_playerScore << std::endl;
            std::cout << "Mission: " << m_currentMission << "/" << m_totalMissions << std::endl;
            std::cout << "Objective: " << m_currentObjective << std::endl;
            if (m_cyborTacticalMode) {
                std::cout << "CYBOR MODE: ACTIVE (Intelligence: " << m_cyborAIIntelligence << ")" << std::endl;
            }
            std::cout << "=================" << std::endl;
        }
        hudUpdateTimer = 0.0f;
    }
}

void CyborGameManager::Shutdown() {
    std::cout << "Shutting down Cybor Game Manager..." << std::endl;

    m_bots.clear();
    m_player.reset();
    m_currentMap.reset();
    m_gameMode.reset();
}
