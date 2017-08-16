#include "Input.h"

GLFWwindow* Input::_window = nullptr;
float Input::_wheel = 0.0f;

void Input::SetWheel(GLFWwindow * window, double wheelX, double wheelY)
{
	_wheel = (float)wheelY;
}

void Input::Initialize(GLFWwindow* window)
{
	_window = window;
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(_window, &Input::SetWheel);
}

void Input::Shutdown()
{
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_FALSE);
	glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, GL_FALSE);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetScrollCallback(_window, nullptr);
}

bool Input::GetKey(unsigned int keyCode)
{
	return glfwGetKey(_window, keyCode) == GLFW_PRESS;
}

bool Input::GetMouseButton(unsigned int buttonCode)
{
	return glfwGetMouseButton(_window, buttonCode) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	static double x, y;
	glfwGetCursorPos(_window, &x, &y);
	return glm::vec2(x, y);
}

float Input::GetMouseWheelDelta()
{
	float w = _wheel;
	_wheel = 0.0f;
	return w;
}
