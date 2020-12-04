#pragma once
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\fwd.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm/glm.hpp>

class Window
{
public:
	Window();
	~Window();

	void Init();
	void Run();

	void ProcessInput();

	static Window* GetCurrentWindow();
	GLFWwindow& GetGLFWWindow();

	void ResetDeltaTime();

	void SetCursorMode(const int& mode);
	glm::vec2 GetCursorPositionOnViewport() const;

	glm::vec2 GetWindowSize() const;
	glm::vec2 GetGLFWWindowPosition() const;

	static bool IsItBuild();

private:
	void RefreshMousePosition(GLFWwindow* window);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	GLFWwindow* window;

	const int winWidth = 1920;
	const int winHeight = 1080;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	//glm::mat4 projection = glm::mat4(1.0f);
	//glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	double mousePosX, mousePosY;

	static Window* instance;
	static constexpr bool isItBuild = false;
	bool shouldResetDeltaTime = false;
};