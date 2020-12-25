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
	void Close();

	void ProcessInput();

	static Window* GetCurrentWindow();
	GLFWwindow& GetGLFWWindow();

	void ResetDeltaTime();

	void SetCursorMode(const int& mode);
	glm::vec2 GetCursorPositionNormalized() const;

	glm::vec2 GetWindowSize() const;
	glm::vec2 GetGLFWWindowPosition() const;

	static bool IsItBuild();

private:
	void RefreshMousePosition(GLFWwindow* window);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	GLFWwindow* window;

	int winWidth = 800;
	int winHeight = 600;

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	double mousePosX, mousePosY;

	static Window* instance;
	static constexpr bool isItBuild = false;
	bool shouldResetDeltaTime = false;
	bool shouldBeClosed = false;
};