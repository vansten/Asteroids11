#pragma once

#include "GL/glew.h"
#include "Shader.h"
#include "Graphics.h"

class Light;

class Mesh
{
	friend class MeshRenderer;

protected:
	GLuint _vertexBuffer;
	GLuint _normalBuffer;
	glm::vec3* _vertexBufferData;
	glm::vec3* _normalBufferData;
	GLsizeiptr _vertexBufferSize;

	GLuint _indexBuffer;
	GLuint* _indexBufferData;
	GLsizeiptr _indexBufferSize;

	std::string _name;

public:
	Mesh();
	Mesh(const Mesh& mesh);
	virtual ~Mesh();

	virtual void Initialize();
	void Shutdown();

	inline const std::string& GetName()
	{
		return _name;
	}
};

