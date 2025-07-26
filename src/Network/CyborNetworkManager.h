#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

/*
 * CyborNetworkManager - Advanced networking system
 * Supports LAN multiplayer and Cybor enhanced communication protocols
 */
class CyborNetworkManager {
public:
    enum class NetworkMode {
        SINGLE_PLAYER,
        LAN_SERVER,
        LAN_CLIENT,
        CYBOR_ENHANCED_PROTOCOL
    };

    struct PlayerInfo {
        std::string name;
        std::string ipAddress;
        int ping;
        int score;
        bool isCyborEnhanced;
    };

public:
    CyborNetworkManager();
    ~CyborNetworkManager();

    bool Initialize();
    void Update(float deltaTime);
    void Shutdown();

    // Server functions
    bool StartServer(int port = 27015, int maxPlayers = 16);
    void StopServer();
    bool IsServerRunning() const { return m_isServer && m_serverRunning; }

    // Client functions
    bool ConnectToServer(const std::string& ipAddress, int port = 27015);
    void Disconnect();
    bool IsConnected() const { return m_isClient && m_connected; }

    // Game state synchronization
    void SendPlayerUpdate(const glm::vec3& position, const glm::vec3& rotation);
    void SendWeaponFire(const glm::vec3& origin, const glm::vec3& direction);
    void SendChatMessage(const std::string& message);

    // Player management
    std::vector<PlayerInfo> GetConnectedPlayers() const { return m_connectedPlayers; }
    int GetPlayerCount() const { return m_connectedPlayers.size(); }

    // Network statistics
    int GetPing() const { return m_ping; }
    float GetPacketLoss() const { return m_packetLoss; }

    // Cybor networking enhancements
    void EnableCyborProtocol(bool enable) { m_cyborProtocolEnabled = enable; }
    void SetCyborEncryption(bool enable) { m_cyborEncryption = enable; }
    void BroadcastCyborSignal(const std::string& signal);

private:
    NetworkMode m_networkMode;

    // Server state
    bool m_isServer;
    bool m_serverRunning;
    int m_serverPort;
    int m_maxPlayers;
    std::thread m_serverThread;

    // Client state
    bool m_isClient;
    bool m_connected;
    std::string m_serverIP;
    int m_serverPort_client;
    std::thread m_clientThread;

    // Network statistics
    int m_ping;
    float m_packetLoss;
    float m_bandwidth;

    // Connected players
    std::vector<PlayerInfo> m_connectedPlayers;
    std::mutex m_playersMutex;

    // Cybor enhancements
    bool m_cyborProtocolEnabled;
    bool m_cyborEncryption;
    std::string m_cyborKey;

    // Private methods
    void ServerThreadFunction();
    void ClientThreadFunction();
    void ProcessIncomingPackets();
    void SendPacket(const std::vector<uint8_t>& data, const std::string& destination = "");
    void HandlePlayerConnect(const std::string& playerName, const std::string& ipAddress);
    void HandlePlayerDisconnect(const std::string& ipAddress);

    // Cybor networking
    std::vector<uint8_t> EncryptCyborPacket(const std::vector<uint8_t>& data);
    std::vector<uint8_t> DecryptCyborPacket(const std::vector<uint8_t>& data);
};
