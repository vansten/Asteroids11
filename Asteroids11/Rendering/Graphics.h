#pragma once

#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")

class Camera;
class Light;
class UITransform;

class Graphics
{
protected:
	GLFWwindow* _window;
	GLuint _lastProgramID;
	GLuint _vao;

public:
	Graphics();
	~Graphics();

	bool Initialize();
	void Shutdown();

	void Clear();
	void Present();
	void BeginGeometry();
	void EndGeometry();
	void BeginUI();
	void EndUI();
	void DrawIndexed(const Camera& camera, Light* ambientLight, Light* directionalLight, const glm::mat4& modelMatrix, const glm::vec4& color, GLuint vertexBuffer, GLuint indexBuffer, GLuint normalBuffer, GLsizeiptr indicesCount);
	void DrawText(const Camera& camera, const std::string& text, const UITransform& uiTransform, const glm::vec4& textColor, GLuint vertexBufferIndex);
	void SetShader(GLuint id);
	
	void SetTitle(const std::string& title);

	inline GLFWwindow* GetWindow() const { return _window; }
};

