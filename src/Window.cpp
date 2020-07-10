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
}
#include "Physics.h"

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

        glm::vec4 clearColor = currentScene.GetCamera().GetClearColor();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::mat4(1.0f);
        glm::vec2 cameraClipping = currentScene.GetCamera().GetClipping();
        projection = glm::perspective(glm::radians(currentScene.GetCamera().GetFOV()), (float)winWidth / (float)winHeight, cameraClipping.x, cameraClipping.y);
        view = currentScene.GetCamera().GetMatrix();
        // pass transformation matrices to the shader
        currentScene.GetCamera().GetShader().SetMat4("projection", projection);
        currentScene.GetCamera().GetShader().SetMat4("view", view);

        currentScene.SetMousePos(glm::vec2(mousePosX, mousePosY), winWidth, winHeight);
        currentScene.Update(deltaTime, currentScene.GetCamera().GetShader(), projection, view);

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
