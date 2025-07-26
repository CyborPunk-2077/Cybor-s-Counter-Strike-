# Cybor's Counter Strike

Cybor's Counter Strike (CSC) is an advanced tactical FPS game inspired by Counter Strike 1.2, written in modern C++ with OpenGL and Cybor enhancements.

Author: Abhishek Tiwary (2025).

## Features
- Modern C++ engine with OpenGL rendering (CyborEngine)
- Single-player campaign with 5 missions
- Intelligent AI bots with Cybor tactical AI
- LAN multiplayer support via CyborNetworkManager
- Advanced audio, weapon, and physics systems
- Cybor Tactical Mode for enhanced gameplay

## Building
Requires CMake, OpenGL, GLEW, GLFW, GLM, and a C++17 compiler.

```
mkdir build && cd build
cmake ..
make -j4
./CyborCounterStrike
```

## Folder Structure
- src/Engine: Core engine
- src/Game: Gameplay logic, player, bots, weapons
- src/Audio: Audio system
- src/Network: Networking system
- assets/: Placeholder for assets (maps, textures, audio)

Enjoy next-level Cybor FPS action!