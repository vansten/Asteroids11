#include "Graphics.h"

#include <stdio.h>

#include "Gameplay/Camera.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize()
{
	if(!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(1024, 768, "Asteroids11", nullptr, nullptr);

	if(!_window)
	{
		printf("Failed to open GLFWwindow\n");
		return false;
	}

	glfwMakeContextCurrent(_window);

	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		return false;
	}
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	return true;
}

void Graphics::Shutdown()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1 ,&_vao);
	_vao = 0;
	glfwTerminate();
}

void Graphics::BeginDraw()
{
	_lastProgramID = -1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Graphics::EndDraw()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glfwSwapBuffers(_window);
}

void Graphics::DrawVertexArray(const Camera& camera, const glm::mat4& modelMatrix, GLuint vertexBuffer, GLuint normalBuffer, GLsizeiptr vertexBufferSize)
{
	GLuint mvpID = glGetUniformLocation(_lastProgramID, "MVP");
	glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3,
		(void*)0
	);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexBufferSize);
}

void Graphics::DrawIndexed(const Camera& camera, const glm::mat4& modelMatrix, const glm::vec4& color, GLuint vertexBuffer, GLuint indexBuffer, GLuint normalBuffer, GLsizeiptr indicesCount)
{
	GLuint mvpID = glGetUniformLocation(_lastProgramID, "MVP");
	glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	GLuint colorID = glGetUniformLocation(_lastProgramID, "Color");
	glUniform4fv(colorID, 1, &color[0]);

	GLuint modelMatrixID = glGetUniformLocation(_lastProgramID, "ModelMatrix");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, (GLsizei) indicesCount, GL_UNSIGNED_INT, (void*)0);
}

void Graphics::SetShader(GLuint id)
{
	if(_lastProgramID != id)
	{
		_lastProgramID = id;
		glUseProgram(id);
	}
}

void Graphics::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(_window, title.c_str());
}
