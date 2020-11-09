#include <iostream>
#include <windows.h>
#include <vector>

#include <glad/glad.h>

#include "Window.h"
#include "Camera.h"
#include "GameObject.h"
#include "Texture.h"
#include "Renderer.h"
#include "Shader.h"

#include "Scene.h"


// enable optimus! -> Use better GPU - Works only for NVidia || But it also locks at 60FPS
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float r = 0.0f;

double scroll_xoffset = 0.0f, scroll_yoffset = 0.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll_xoffset = xoffset;
    scroll_yoffset = yoffset;
}

Window window;

int main()
{
    window.Init();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene scene;
    window.Run();

    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}