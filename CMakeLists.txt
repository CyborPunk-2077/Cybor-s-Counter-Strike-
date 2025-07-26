cmake_minimum_required(VERSION 3.16)
project(CyborCounterStrike VERSION 2.5)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages - temporarily commented out
# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(glfw3 REQUIRED)
# find_package(glm REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    main.cpp
    src/Engine/CyborEngine.cpp
    src/Game/CyborGameManager.cpp
    src/Game/CyborPlayer.cpp
    src/Game/CyborWeapon.cpp
    src/Game/CyborBot.cpp
    src/Audio/CyborAudioSystem.cpp
    src/Network/CyborNetworkManager.cpp
)

# Create graphical game executable - commented out due to OpenGL dependencies
# add_executable(CyborCounterStrike ${SOURCES})

# Create console version executable - commented out due to OpenGL dependencies
# add_executable(CyborCounterStrike_Console main_simple.cpp src/Engine/CyborEngine.cpp src/Game/CyborGameManager.cpp src/Game/CyborPlayer.cpp src/Game/CyborWeapon.cpp src/Game/CyborBot.cpp src/Audio/CyborAudioSystem.cpp src/Network/CyborNetworkManager.cpp)

# Create simple launcher executable
add_executable(CyborCounterStrike_Launcher launcher.cpp)

# Link libraries - temporarily commented out
# target_link_libraries(CyborCounterStrike
#     OpenGL::GL
#     GLEW::GLEW
#     glfw
#     glm
# )

# Set compiler flags - commented out since targets are commented out
# if(MSVC)
#     target_compile_options(CyborCounterStrike PRIVATE /W4)
# else()
#     target_compile_options(CyborCounterStrike PRIVATE -Wall -Wextra)
# endif()

# Copy assets to build directory - commented out since assets directory doesn't exist
# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}) 