/*
 * Cybor's Counter Strike v2.5 - Console Version
 * Created by: Abhishek Tiwary (2025)
 * 
 * A simplified console version for testing game logic
 */

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

// Simplified game classes
class SimpleEngine {
public:
    SimpleEngine() : m_running(true), m_lastTime(std::chrono::high_resolution_clock::now()) {}
    
    bool IsRunning() const { return m_running; }
    void Stop() { m_running = false; }
    
    float GetDeltaTime() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_lastTime);
        float dt = duration.count() / 1000000.0f;
        m_lastTime = currentTime;
        return dt;
    }
    
    void PollEvents() {
        // Simulate event polling
    }
    
private:
    bool m_running;
    std::chrono::high_resolution_clock::time_point m_lastTime;
};

class SimplePlayer {
public:
    SimplePlayer() : m_health(100.0f), m_armor(100.0f), m_kills(0), m_deaths(0) {
        m_position.x = 0.0f;
        m_position.y = 1.8f;
        m_position.z = 0.0f;
    }
    
    void Update(float deltaTime) {
        // Simple player update logic
    }
    
    void TakeDamage(float damage) {
        if (m_armor > 0.0f) {
            float armorAbsorption = std::min(m_armor, damage * 0.5f);
            m_armor -= armorAbsorption;
            damage -= armorAbsorption;
        }
        
        if (damage > 0.0f) {
            m_health -= damage;
        }
        
        std::cout << "Player took " << damage << " damage. Health: " << m_health << ", Armor: " << m_armor << std::endl;
        
        if (m_health <= 0.0f) {
            m_deaths++;
            std::cout << "Player died! Deaths: " << m_deaths << std::endl;
        }
    }
    
    bool IsAlive() const { return m_health > 0.0f; }
    float GetHealth() const { return m_health; }
    float GetArmor() const { return m_armor; }
    int GetKills() const { return m_kills; }
    int GetDeaths() const { return m_deaths; }
    
private:
    float m_health, m_armor;
    struct { float x, y, z; } m_position;
    int m_kills, m_deaths;
};

class SimpleBot {
public:
    SimpleBot(const std::string& name, int difficulty) 
        : m_name(name), m_difficulty(difficulty), m_health(100.0f), m_alive(true) {}
    
    void Update(float deltaTime, const SimplePlayer& player) {
        if (!m_alive) return;
        
        // Simple bot AI logic
        static float aiTimer = 0.0f;
        aiTimer += deltaTime;
        
        if (aiTimer > 2.0f) {
            if (player.IsAlive()) {
                std::cout << m_name << " is engaging player!" << std::endl;
            }
            aiTimer = 0.0f;
        }
    }
    
    void TakeDamage(float damage) {
        m_health -= damage;
        std::cout << m_name << " took " << damage << " damage. Health: " << m_health << std::endl;
        
        if (m_health <= 0.0f) {
            m_alive = false;
            std::cout << m_name << " was eliminated!" << std::endl;
        }
    }
    
    bool IsAlive() const { return m_alive; }
    const std::string& GetName() const { return m_name; }
    
private:
    std::string m_name;
    int m_difficulty;
    float m_health;
    bool m_alive;
};

class SimpleGameManager {
public:
    SimpleGameManager() : m_currentMission(0), m_totalMissions(5), m_playerScore(0), m_enemiesKilled(0) {
        InitializeBots();
    }
    
    void Update(float deltaTime) {
        // Update player
        m_player.Update(deltaTime);
        
        // Update bots
        for (auto& bot : m_bots) {
            if (bot->IsAlive()) {
                bot->Update(deltaTime, m_player);
            }
        }
        
        // Check win conditions
        CheckWinConditions();
        
        // Update mission timer
        m_missionTimer += deltaTime;
    }
    
    void StartCampaign() {
        std::cout << "\n=== STARTING CYBOR'S COUNTER STRIKE CAMPAIGN ===" << std::endl;
        m_currentMission = 0;
        m_playerScore = 0;
        m_enemiesKilled = 0;
        LoadNextMission();
    }
    
    void LoadNextMission() {
        if (m_currentMission >= m_totalMissions) {
            std::cout << "\n=== CAMPAIGN COMPLETED! ===" << std::endl;
            std::cout << "Final Score: " << m_playerScore << std::endl;
            std::cout << "Enemies Killed: " << m_enemiesKilled << std::endl;
            return;
        }
        
        m_currentMission++;
        m_missionTimer = 0.0f;
        
        std::cout << "\n=== MISSION " << m_currentMission << "/" << m_totalMissions << " ===" << std::endl;
        
        // Reset bots
        for (auto& bot : m_bots) {
            bot->TakeDamage(-100.0f); // Heal bots
        }
        
        // Add more bots based on mission difficulty
        int botCount = 2 + m_currentMission;
        for (int i = m_bots.size(); i < botCount; i++) {
            std::string botName = "CyborBot_" + std::to_string(i + 1);
            m_bots.push_back(std::make_unique<SimpleBot>(botName, m_currentMission));
        }
        
        std::cout << "Mission started with " << m_bots.size() << " enemies!" << std::endl;
    }
    
    void SimulateCombat() {
        // Simulate some combat events
        static float combatTimer = 0.0f;
        combatTimer += 0.016f; // Assume 60 FPS
        
        if (combatTimer > 1.0f) {
            // Player shoots at random bot
            for (auto& bot : m_bots) {
                if (bot->IsAlive()) {
                    float damage = 25.0f + (rand() % 20);
                    bot->TakeDamage(damage);
                    
                    if (!bot->IsAlive()) {
                        m_enemiesKilled++;
                        m_playerScore += 100;
                    }
                    break;
                }
            }
            
            // Random bot shoots at player
            for (auto& bot : m_bots) {
                if (bot->IsAlive() && m_player.IsAlive()) {
                    float damage = 15.0f + (rand() % 15);
                    m_player.TakeDamage(damage);
                    break;
                }
            }
            
            combatTimer = 0.0f;
        }
    }
    
    void CheckWinConditions() {
        // Count alive enemies
        int aliveEnemies = 0;
        for (const auto& bot : m_bots) {
            if (bot->IsAlive()) aliveEnemies++;
        }
        
        // Mission completed if all enemies dead
        if (aliveEnemies == 0 && m_missionTimer > 5.0f) {
            std::cout << "\n=== MISSION " << m_currentMission << " COMPLETED! ===" << std::endl;
            std::cout << "Score: " << m_playerScore << " | Enemies Killed: " << m_enemiesKilled << std::endl;
            LoadNextMission();
        }
        
        // Mission failed if player dead
        if (!m_player.IsAlive()) {
            std::cout << "\n=== MISSION FAILED! ===" << std::endl;
            std::cout << "Player died. Restarting mission..." << std::endl;
            m_player.TakeDamage(-100.0f); // Heal player
        }
    }
    
    void PrintStatus() {
        std::cout << "\n=== GAME STATUS ===" << std::endl;
        std::cout << "Mission: " << m_currentMission << "/" << m_totalMissions << std::endl;
        std::cout << "Player Health: " << m_player.GetHealth() << " | Armor: " << m_player.GetArmor() << std::endl;
        std::cout << "Score: " << m_playerScore << " | Kills: " << m_enemiesKilled << std::endl;
        
        int aliveBots = 0;
        for (const auto& bot : m_bots) {
            if (bot->IsAlive()) aliveBots++;
        }
        std::cout << "Enemies Remaining: " << aliveBots << std::endl;
        std::cout << "===================" << std::endl;
    }
    
private:
    void InitializeBots() {
        m_bots.push_back(std::make_unique<SimpleBot>("CyborBot_1", 1));
        m_bots.push_back(std::make_unique<SimpleBot>("CyborBot_2", 1));
    }
    
    SimplePlayer m_player;
    std::vector<std::unique_ptr<SimpleBot>> m_bots;
    int m_currentMission, m_totalMissions;
    int m_playerScore, m_enemiesKilled;
    float m_missionTimer = 0.0f;
};

int main() {
    std::cout << "=================================================" << std::endl;
    std::cout << "  CYBOR'S COUNTER STRIKE v2.5 - CONSOLE VERSION" << std::endl;
    std::cout << "  Advanced Tactical FPS Experience" << std::endl;
    std::cout << "  Created by: Abhishek Tiwary (2025)" << std::endl;
    std::cout << "=================================================" << std::endl;

    try {
        auto engine = std::make_unique<SimpleEngine>();
        auto gameManager = std::make_unique<SimpleGameManager>();
        
        std::cout << "Cybor's Counter Strike initialized successfully!" << std::endl;
        std::cout << "Starting campaign..." << std::endl;
        
        gameManager->StartCampaign();
        
        // Main game loop
        while (engine->IsRunning()) {
            float deltaTime = engine->GetDeltaTime();
            
            // Update game
            gameManager->Update(deltaTime);
            gameManager->SimulateCombat();
            
            // Print status every 5 seconds
            static float statusTimer = 0.0f;
            statusTimer += deltaTime;
            if (statusTimer > 5.0f) {
                gameManager->PrintStatus();
                statusTimer = 0.0f;
            }
            
            // Simulate frame rate
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
            
            engine->PollEvents();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "Thank you for playing Cybor's Counter Strike!" << std::endl;
    return 0;
} 