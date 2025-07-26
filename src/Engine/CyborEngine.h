#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <chrono>

/*
 * CyborEngine - Core rendering and window management system
 * Optimized for tactical FPS gameplay with Cybor enhancements
 */
class CyborEngine {
public:
    CyborEngine();
    ~CyborEngine();

    // Engine lifecycle
    bool Initialize(const std::string& title, int width, int height);
    void Update(float deltaTime);
    void BeginFrame();
    void EndFrame();
    void Shutdown();

    // Window management
    bool IsRunning() const;
    void PollEvents();
    GLFWwindow* GetWindow() const { return m_window; }

    // Time management
    float GetDeltaTime();

    // Input handling
    bool IsKeyPressed(int key) const;
    bool IsMouseButtonPressed(int button) const;
    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMouseDelta() const;

    // Graphics utilities
    void SetViewMatrix(const glm::mat4& view);
    void SetProjectionMatrix(const glm::mat4& projection);
    glm::mat4 GetViewMatrix() const { return m_viewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return m_projectionMatrix; }

    // Cybor-specific enhancements
    void EnableCyborMode(bool enable) { m_cyborModeEnabled = enable; }
    void SetCyborIntensity(float intensity) { m_cyborIntensity = intensity; }

private:
    GLFWwindow* m_window;
    int m_windowWidth, m_windowHeight;
    std::string m_windowTitle;

    // Time tracking
    std::chrono::high_resolution_clock::time_point m_lastTime;
    float m_deltaTime;
    float m_fps;

    // Matrices
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    // Input state
    glm::vec2 m_lastMousePos;
    glm::vec2 m_mouseDelta;

    // Cybor enhancements
    bool m_cyborModeEnabled;
    float m_cyborIntensity;

    // Internal methods
    void InitializeOpenGL();
    void SetupCallbacks();
    void UpdateFPS();

    // Static callbacks
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
