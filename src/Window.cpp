#include "Window.h"
#include "Scene.h"
#include "InputManager.h"
#include "Editor.h"

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

    window = glfwCreateWindow(winWidth, winHeight, "Qk Engine 0.001", nullptr /*glfwGetPrimaryMonitor()*/, nullptr);
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
    Editor::Init(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        InputManager::Update();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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
        Editor::Update();

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
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

void Window::SetCursorMode(const int& mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

glm::vec2 Window::GetWindowSize() const
{
    return glm::vec2(winWidth, winHeight);
}

glm::vec2 Window::GetGLFWWindowPosition()
{
    int x = 0, y = 0;
    glfwGetWindowPos(window, &x, &y);
    return glm::vec2(x, y);
}
