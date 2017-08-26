#include "Graphics.h"

#include <stdio.h>

#include "Core/UITransform.h"
#include "Gameplay/Camera.h"
#include "Light.h"

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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_screenSettings.Width = 800.0f;
	_screenSettings.Height = 600.0f;

	_window = glfwCreateWindow((int)_screenSettings.Width, (int)_screenSettings.Height, "Asteroids11", nullptr, nullptr);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//Disable byte-aligment restriction

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

void Graphics::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::Present()
{
	glfwSwapBuffers(_window);
}

void Graphics::BeginGeometry()
{
	_lastProgramID = -1;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Graphics::EndGeometry()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Graphics::BeginUI()
{
	_lastProgramID = -1;
	glEnableVertexAttribArray(0);
}

void Graphics::EndUI()
{
	glDisableVertexAttribArray(0);
}

void Graphics::DrawIndexed(const Camera& camera, Light* ambientLight, Light* directionalLight, const glm::mat4& modelMatrix, const glm::vec4& color, GLuint vertexBuffer, GLuint indexBuffer, GLuint normalBuffer, GLsizeiptr indicesCount)
{
	GLuint mvpID = glGetUniformLocation(_lastProgramID, "MVP");
	glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	GLuint colorID = glGetUniformLocation(_lastProgramID, "Color");
	glUniform4fv(colorID, 1, &color[0]);

	GLuint modelMatrixID = glGetUniformLocation(_lastProgramID, "ModelMatrix");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

	if(ambientLight)
	{
		GLuint ambientLightID = glGetUniformLocation(_lastProgramID, "AmbientLight");
		const glm::vec4& ambientColor = ambientLight->GetColor();
		glUniform4fv(ambientLightID, 1, &ambientColor[0]);
	}

	if(directionalLight)
	{
		GLuint directionalLightID = glGetUniformLocation(_lastProgramID, "DirectionalLight");
		const glm::vec4& directionalColor = directionalLight->GetColor();
		glUniform4fv(directionalLightID, 1, &directionalColor[0]);

		GLuint directionalForwardID = glGetUniformLocation(_lastProgramID, "DirectionalForward");
		glm::vec4 forward = glm::vec4(directionalLight->GetTransform().GetForward(), 0.0f);
		glUniform4fv(directionalForwardID, 1, &forward[0]);
	}

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Graphics::DrawText(const Camera& camera, const std::string& text, std::map<GLuint, Character>& characterSet, const UITransform& uiTransform, const glm::vec4& textColor, GLuint fontSize, GLuint vertexBufferIndex)
{
	GLuint textColorID = glGetUniformLocation(_lastProgramID, "textColor");
	glUniform4fv(textColorID, 1, &textColor[0]);
	glActiveTexture(GL_TEXTURE0);

	GLuint projectionID = glGetUniformLocation(_lastProgramID, "projection");
	const glm::mat4& projection = camera.GetUIProjectionMatrix();
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIndex);

	glm::vec2 position = uiTransform.GetScreenPosition();
	const glm::vec2& size = uiTransform.GetSize();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	size_t stringSize = text.size();
	GLchar c;
	for(size_t i = 0; i < stringSize; ++i)
	{
		c = text[i];
		if(c == '\n')
		{
			position.x = uiTransform.GetScreenPosition().x;
			position.y -= fontSize;
		}
		else
		{
			Character character = characterSet[c];

			GLfloat xPos = position.x + character.Bearing.x * size.x;
			GLfloat yPos = position.y - (character.Size.y - character.Bearing.y) * size.y;

			GLfloat width = character.Size.x * size.x;
			GLfloat height = character.Size.y * size.y;

			glm::vec4 vertices[6] = {
				glm::vec4(xPos, yPos + height, 0.0, 0.0),
				glm::vec4(xPos, yPos, 0.0, 1.0),
				glm::vec4(xPos + width, yPos, 1.0, 1.0),

				glm::vec4(xPos, yPos + height, 0.0, 0.0),
				glm::vec4(xPos + width, yPos, 1.0, 1.0),
				glm::vec4(xPos + width, yPos + height, 1.0, 0.0),
			};
			glBindTexture(GL_TEXTURE_2D, character.TextureID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			position.x += (character.Advance >> 6) * size.x;
		}
	}
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
