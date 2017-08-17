#pragma once

#include "GL/glew.h"
#include "Shader.h"
#include "Graphics.h"

class Mesh
{
protected:
	GLuint _vertexBuffer;
	GLuint _normalBuffer;
	glm::vec3* _vertexBufferData;
	glm::vec3* _normalBufferData;
	GLsizeiptr _vertexBufferSize;

	GLuint _indexBuffer;
	GLuint* _indexBufferData;
	GLsizeiptr _indexBufferSize;
	
	glm::vec4 _color;

	Shader* _shader;

	std::string _name;

public:
	Mesh(Shader* shader, const glm::vec4& color);
	Mesh(const Mesh& mesh);
	virtual ~Mesh();

	virtual void Initialize();
	void Shutdown();

	void Render(const class Camera& camera, const glm::mat4& modelMatrix, Graphics* graphics);

	inline void SetColor(const glm::vec4& color)
	{
		_color = color;
	}

	inline const glm::vec4& GetColor() const
	{
		return _color;
	}

	inline const std::string& GetName()
	{
		return _name;
	}
};

