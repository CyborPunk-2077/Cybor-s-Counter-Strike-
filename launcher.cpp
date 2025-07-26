#include <iostream>
#include <string>
#include <cstdlib>

int main() {
    std::cout << "=================================================" << std::endl;
    std::cout << "  CYBOR'S COUNTER STRIKE v2.5 - GAME LAUNCHER" << std::endl;
    std::cout << "  Advanced Tactical FPS Experience" << std::endl;
    std::cout << "  Created by: Abhishek Tiwary (2025)" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Launching Cybor's Counter Strike..." << std::endl;
    std::cout << "Please wait while the game initializes..." << std::endl;
    std::cout << std::endl;

    // Try to launch the console version first
    std::cout << "Attempting to launch console version..." << std::endl;
    
    // Check if console executable exists
    std::string consoleExe = "CyborCounterStrike_Console.exe";
    
    std::cout << "Starting " << consoleExe << "..." << std::endl;
    
    // Launch the console version
    int result = system(consoleExe.c_str());
    
    if (result != 0) {
        std::cout << "Console version failed to start. Error code: " << result << std::endl;
        std::cout << "Please make sure the game files are properly installed." << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Thank you for playing Cybor's Counter Strike!" << std::endl;
    std::cout << "Press any key to exit..." << std::endl;
    std::cin.get();
    
    return 0;
} 