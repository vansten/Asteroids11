#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")

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

	void BeginDraw();
	void EndDraw();
	void DrawVertexArray(const class Camera& camera, const glm::mat4& modelMatrix, GLuint vertexBufferID, GLsizeiptr vertexBufferSize);
	void DrawIndexed(const class Camera& camera, const glm::mat4& modelMatrix, const glm::vec4& color, GLuint vertexBuffer, GLuint indexBuffer, GLsizeiptr indicesCount);
	void SetShader(GLuint id);

	inline GLFWwindow* GetWindow() const { return _window; }
};

