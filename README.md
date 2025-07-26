# Cybor's Counter Strike v2.5

**Cybor's Counter Strike** is a modern, advanced tactical first-person shooter (FPS) game inspired by the classic Counter-Strike 1.2, reimagined with cutting-edge C++ and OpenGL technology. This project is designed to deliver a high-performance, immersive FPS experience, blending classic gameplay mechanics with innovative features and AI-driven tactical systems.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Game Modes](#game-modes)
- [AI & Bots](#ai--bots)
- [Audio System](#audio-system)
- [Networking](#networking)
- [Build & Run](#build--run)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

**Cybor's Counter Strike** is a full-featured FPS game that brings together the nostalgia of classic tactical shooters and the power of modern game development. The game is built from the ground up in C++17, leveraging OpenGL for graphics, GLEW and GLFW for window/context management, and GLM for mathematics. The architecture is modular, extensible, and designed for both single-player and multiplayer experiences.

The project includes a robust game engine, advanced AI for bots, a campaign system, audio and networking subsystems, and a flexible build system using CMake. It is suitable for both players and developers interested in FPS mechanics, AI, and game engine design.

---

## Features

- **Modern C++17 Codebase:** Clean, modular, and extensible code using best practices.
- **OpenGL Rendering:** High-performance graphics with support for modern OpenGL features.
- **Advanced AI Bots:** Intelligent enemy and ally bots with tactical behaviors, pathfinding, and adaptive difficulty.
- **Campaign Mode:** Multi-mission campaign with increasing difficulty, unique objectives, and diverse maps.
- **Audio System:** Immersive sound effects and background music using a custom audio engine.
- **Networking:** Foundation for multiplayer gameplay, including network manager and basic synchronization.
- **Custom Game Engine:** Handles rendering, input, timing, and game loop management.
- **Flexible Build System:** CMake-based, cross-platform, and easy to extend.
- **Console and Graphical Versions:** Includes both a console-based and (optionally) graphical version of the game.
- **Launcher:** Simple launcher executable to start the game easily.
- **Extensive Logging and Debugging:** Console output for game state, AI actions, and errors.

---

## Architecture

The project is organized into several core modules:

- **Engine:** The heart of the game, managing the main loop, rendering, input, and timing.
- **Game:** Contains all gameplay logic, including the game manager, player, bots, weapons, and campaign system.
- **Audio:** Manages sound effects and background music, providing an immersive experience.
- **Network:** Lays the groundwork for multiplayer support, including network initialization and updates.
- **Assets:** (Optional) Directory for game assets such as textures, models, and audio files.

Each module is designed for separation of concerns, making the codebase easy to navigate and extend.

---

## Game Modes

- **Campaign Mode:** Progress through a series of missions, each with unique objectives and increasing difficulty. Missions include classic scenarios such as compound assaults, urban warfare, industrial infiltration, arctic base assaults, and a final showdown.
- **Bot Skirmish:** Face off against AI-controlled bots with varying intelligence and tactics.
- **(Planned) Multiplayer:** The network manager is designed to be extended for online play.

---

## AI & Bots

Cybor's Counter Strike features advanced AI bots that:

- Use tactical decision-making to engage the player and each other.
- Adapt their behavior based on mission difficulty and player actions.
- Support team-based play (Counter-Terrorists vs. Terrorists).
- Can be extended with new behaviors, pathfinding, and squad tactics.

---

## Audio System

The custom audio system provides:

- Background music playback (e.g., `cybor_theme.wav`).
- Realistic sound effects for weapons, footsteps, and environment.
- Easy integration of new audio assets.

---

## Networking

The network manager initializes and updates network connections, providing a foundation for:

- Multiplayer gameplay (future extension).
- Synchronization of player and bot states.
- Potential for LAN or online matches.

---

## Build & Run

### Prerequisites

- **C++17 Compiler** (GCC, Clang, MSVC)
- **CMake 3.16+**
- **OpenGL, GLEW, GLFW, GLM** (for graphical version)
- **(Optional) MinGW or MSYS2 for Windows builds**

### Building the Project

1. Clone the repository:
   ```sh
   git clone https://github.com/CyborPunk-2077/Cybor-s-Counter-Strike-.git
   cd Cybor-s-Counter-Strike-
   ```
2. Configure and build:
   ```sh
   cmake -S . -B build
   cmake --build build
   ```
3. Run the launcher:
   - On Windows: Double-click `CyborCounterStrike_Launcher.exe` or run from terminal.

### Note

- The graphical version requires OpenGL and related libraries. The console version can run without them.
- If you encounter missing assets or libraries, ensure all dependencies are installed and available in your PATH.

---

## Project Structure

```
Cybor-s-Counter-Strike-/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── main_simple.cpp
├── main_simple_fixed.cpp
├── launcher.cpp
├── src/
│   ├── Audio/
│   ├── Engine/
│   ├── Game/
│   └── Network/
└── build/
```

- **src/Audio/**: Audio system source code
- **src/Engine/**: Game engine core
- **src/Game/**: Game logic, player, bots, weapons, campaign
- **src/Network/**: Networking code
- **build/**: Build artifacts (not tracked in git)

---

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes with clear messages.
4. Submit a pull request describing your changes.

---

## License

This project is open source and available under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Credits

- **Lead Developer:** Abhishek Tiwary (2025)
- **Inspiration:** Classic Counter-Strike and the open-source game development community.

---

## Repository

- GitHub: [https://github.com/CyborPunk-2077/Cybor-s-Counter-Strike-](https://github.com/CyborPunk-2077/Cybor-s-Counter-Strike-)

---

This project is a testament to modern C++ game development, AI, and FPS design. Whether you are a player, developer, or enthusiast, Cybor's Counter Strike offers a rich platform for learning, playing, and contributing to the world of tactical shooters.
