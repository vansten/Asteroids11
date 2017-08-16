#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Input
{
protected:
	static GLFWwindow* _window;
	static float _wheel;

protected:
	static void SetWheel(GLFWwindow* window, double wheelX, double wheelY);

public:
	static void Initialize(GLFWwindow* window);
	static void Shutdown();

	static bool GetKey(unsigned int keyCode);
	static bool GetMouseButton(unsigned int buttonCode);
	static glm::vec2 GetMousePosition();
	static float GetMouseWheelDelta();
};

