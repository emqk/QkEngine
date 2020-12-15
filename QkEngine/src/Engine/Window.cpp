#include "Window.h"
#include "Scene.h"
#include "InputManager.h"
#include "Editor.h"
#include "Gizmos.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include <iostream>

Window* Window::instance = nullptr;

Window::Window()
{
}

Window::~Window()
{
}

void Window::Init()
{
    instance = this;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    winWidth = mode->width;
    winHeight = mode->height;
    std::cout << "Screen size: " << winWidth << " x " << winHeight << "\n";

    window = glfwCreateWindow(winWidth, winHeight, "Qk Engine 0.01", Window::IsItBuild() ? primaryMonitor : nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Run()
{
    if(!IsItBuild())
        Editor::Init(window);

    Gizmos::Init();

    while (!glfwWindowShouldClose(window) && !shouldBeClosed)
    {
        glfwPollEvents();
        InputManager::Update();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (shouldResetDeltaTime)
        {
            deltaTime = 0;
            shouldResetDeltaTime = false;
        }

        Scene& currentScene = Scene::GetCurrentScene();

        RefreshMousePosition(window);
        ProcessInput();
        currentScene.GetCamera().Update(deltaTime);

        Renderer::Pre();


        glm::vec4 clearColor = currentScene.GetCamera().GetClearColor();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 projection = glm::mat4(1.0f);
        glm::vec2 cameraClipping = currentScene.GetCamera().GetClipping();
        projection = glm::perspective(glm::radians(currentScene.GetCamera().GetFOV()), (float)winWidth / (float)winHeight, cameraClipping.x, cameraClipping.y);
        glm::mat4 view = currentScene.GetCamera().GetMatrix();
        // refresh transformation matrices
        currentScene.GetCamera().projection = projection;
        currentScene.GetCamera().view = view;

        currentScene.SetMousePos(glm::vec2(mousePosX, mousePosY), Editor::GetViewportSize().x, Editor::GetViewportSize().y);
        currentScene.Update(deltaTime, projection, view);


        Renderer::Post();
        if (!IsItBuild())
            Editor::Update();

        glfwSwapBuffers(window);
    }

    if (!IsItBuild())
    {
        // ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();
}

void Window::Close()
{
    shouldBeClosed = true;
}

void Window::ProcessInput()
{
    
}

void Window::RefreshMousePosition(GLFWwindow* window)
{
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Scene::GetCurrentScene().GetCamera().ReceiveScrollInput(xoffset, yoffset);
}

Window* Window::GetCurrentWindow()
{
    return instance;
}

GLFWwindow& Window::GetGLFWWindow()
{
    return *window;
}

void Window::ResetDeltaTime()
{
    shouldResetDeltaTime = true;
}

void Window::SetCursorMode(const int& mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

glm::vec2 Window::GetCursorPositionNormalized() const
{
    if (IsItBuild())
    {
        return glm::vec2(mousePosX / winWidth, mousePosY / winHeight);
    }
    else
    {
        glm::vec2 windowPos = GetGLFWWindowPosition();
        glm::vec2 viewportPos = Editor::GetViewportPosition();
        return glm::vec2(mousePosX - viewportPos.x + windowPos.x, mousePosY - viewportPos.y + windowPos.y) /= Editor::GetViewportSize();
    }
}

glm::vec2 Window::GetWindowSize() const
{
    return glm::vec2(winWidth, winHeight);
}

glm::vec2 Window::GetGLFWWindowPosition() const
{
    int x = 0, y = 0;
    glfwGetWindowPos(window, &x, &y);
    return glm::vec2(x, y);
}

bool Window::IsItBuild()
{
    return isItBuild;
}
