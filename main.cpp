/*
 * Cybor's Counter Strike v2.5 - Advanced Tactical FPS
 * Created by: Abhishek Tiwary (2025)
 * 
 * A comprehensive Counter-Strike 1.2 inspired first-person shooter
 * Built with modern C++ and OpenGL for maximum performance and Cybor innovation
 */

#include "src/Engine/CyborEngine.h"
#include "src/Game/CyborGameManager.h"
#include "src/Audio/CyborAudioSystem.h"
#include "src/Network/CyborNetworkManager.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=================================================" << std::endl;
    std::cout << "  CYBOR'S COUNTER STRIKE v2.5" << std::endl;
    std::cout << "  Advanced Tactical FPS Experience" << std::endl;
    std::cout << "  Created by: Abhishek Tiwary (2025)" << std::endl;
    std::cout << "=================================================" << std::endl;

    try {
        // Initialize Cybor Engine
        auto cyborEngine = std::make_unique<CyborEngine>();
        if (!cyborEngine->Initialize("Cybor's Counter Strike", 1920, 1080)) {
            std::cerr << "Failed to initialize Cybor Engine!" << std::endl;
            return -1;
        }

        // Initialize Game Manager
        auto gameManager = std::make_unique<CyborGameManager>(cyborEngine.get());
        if (!gameManager->Initialize()) {
            std::cerr << "Failed to initialize Cybor Game Manager!" << std::endl;
            return -1;
        }

        // Initialize Audio System
        auto audioSystem = std::make_unique<CyborAudioSystem>();
        audioSystem->Initialize();
        audioSystem->PlayBackgroundMusic("assets/audio/cybor_theme.wav");

        // Initialize Network Manager for multiplayer
        auto networkManager = std::make_unique<CyborNetworkManager>();
        networkManager->Initialize();

        std::cout << "Cybor's Counter Strike initialized successfully!" << std::endl;
        std::cout << "Loading Cybor tactical systems..." << std::endl;

        // Main game loop
        while (cyborEngine->IsRunning()) {
            float deltaTime = cyborEngine->GetDeltaTime();

            // Update systems
            cyborEngine->Update(deltaTime);
            gameManager->Update(deltaTime);
            networkManager->Update(deltaTime);
            audioSystem->Update(deltaTime);

            // Render frame
            cyborEngine->BeginFrame();
            gameManager->Render();
            cyborEngine->EndFrame();

            cyborEngine->PollEvents();
        }

        // Cleanup
        std::cout << "Shutting down Cybor's Counter Strike..." << std::endl;
        audioSystem->Shutdown();
        networkManager->Shutdown();
        gameManager->Shutdown();
        cyborEngine->Shutdown();

    } catch (const std::exception& e) {
        std::cerr << "Cybor Engine Error: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "Thank you for playing Cybor's Counter Strike!" << std::endl;
    return 0;
}
