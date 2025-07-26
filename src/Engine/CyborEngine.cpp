#include "CyborEngine.h"
#include <iostream>
#include <stdexcept>

CyborEngine::CyborEngine() 
    : m_window(nullptr), m_windowWidth(0), m_windowHeight(0),
      m_deltaTime(0.0f), m_fps(0.0f), 
      m_cyborModeEnabled(false), m_cyborIntensity(1.0f) {
    m_lastTime = std::chrono::high_resolution_clock::now();
    m_lastMousePos = glm::vec2(0.0f);
    m_mouseDelta = glm::vec2(0.0f);
}

CyborEngine::~CyborEngine() {
    Shutdown();
}

bool CyborEngine::Initialize(const std::string& title, int width, int height) {
    m_windowTitle = title;
    m_windowWidth = width;
    m_windowHeight = height;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA for Cybor quality

    // Create window
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    InitializeOpenGL();
    SetupCallbacks();

    std::cout << "Cybor Engine initialized with OpenGL " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void CyborEngine::InitializeOpenGL() {
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable face culling for performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Enable multisampling for anti-aliasing
    glEnable(GL_MULTISAMPLE);

    // Set clear color to Cybor theme
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    // Setup viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    // Initialize projection matrix
    m_projectionMatrix = glm::perspective(
        glm::radians(75.0f), 
        (float)m_windowWidth / (float)m_windowHeight, 
        0.1f, 
        1000.0f
    );
}

void CyborEngine::SetupCallbacks() {
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetCursorPosCallback(m_window, MouseCallback);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

    // Lock cursor for FPS controls
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CyborEngine::Update(float deltaTime) {
    m_deltaTime = deltaTime;
    UpdateFPS();

    // Update mouse delta
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    glm::vec2 currentMousePos(xpos, ypos);
    m_mouseDelta = currentMousePos - m_lastMousePos;
    m_lastMousePos = currentMousePos;
}

void CyborEngine::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply Cybor visual enhancements if enabled
    if (m_cyborModeEnabled) {
        float intensity = m_cyborIntensity;
        glClearColor(0.05f * intensity, 0.1f * intensity, 0.15f * intensity, 1.0f);
    }
}

void CyborEngine::EndFrame() {
    glfwSwapBuffers(m_window);
}

void CyborEngine::PollEvents() {
    glfwPollEvents();
}

bool CyborEngine::IsRunning() const {
    return !glfwWindowShouldClose(m_window);
}

float CyborEngine::GetDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_lastTime);
    float dt = duration.count() / 1000000.0f;
    m_lastTime = currentTime;
    return dt;
}

bool CyborEngine::IsKeyPressed(int key) const {
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool CyborEngine::IsMouseButtonPressed(int button) const {
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

glm::vec2 CyborEngine::GetMousePosition() const {
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

glm::vec2 CyborEngine::GetMouseDelta() const {
    return m_mouseDelta;
}

void CyborEngine::SetViewMatrix(const glm::mat4& view) {
    m_viewMatrix = view;
}

void CyborEngine::SetProjectionMatrix(const glm::mat4& projection) {
    m_projectionMatrix = projection;
}

void CyborEngine::UpdateFPS() {
    static float fpsTimer = 0.0f;
    static int frameCount = 0;

    frameCount++;
    fpsTimer += m_deltaTime;

    if (fpsTimer >= 1.0f) {
        m_fps = frameCount / fpsTimer;
        frameCount = 0;
        fpsTimer = 0.0f;

        // Update window title with FPS
        std::string title = m_windowTitle + " - FPS: " + std::to_string((int)m_fps);
        glfwSetWindowTitle(m_window, title.c_str());
    }
}

void CyborEngine::Shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

// Static callback implementations
void CyborEngine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void CyborEngine::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    CyborEngine* engine = static_cast<CyborEngine*>(glfwGetWindowUserPointer(window));
    // Mouse handling is done in Update method
}

void CyborEngine::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    CyborEngine* engine = static_cast<CyborEngine*>(glfwGetWindowUserPointer(window));
    if (engine) {
        engine->m_windowWidth = width;
        engine->m_windowHeight = height;
        engine->m_projectionMatrix = glm::perspective(
            glm::radians(75.0f), 
            (float)width / (float)height, 
            0.1f, 
            1000.0f
        );
    }
}
