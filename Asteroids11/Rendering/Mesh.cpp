#include "Mesh.h"

#include "Gameplay/Camera.h"
#include "Light.h"

Mesh::Mesh() : _vertexBufferData(nullptr), _indexBufferData(nullptr)
{
}

Mesh::Mesh(const Mesh& mesh) : _vertexBufferData(nullptr), _indexBufferData(nullptr)
{
	Shutdown();

	_vertexBufferSize = mesh._vertexBufferSize;
	_vertexBufferData = (glm::vec3*)Memory::GetInstance()->Allocate(sizeof(glm::vec3) * _vertexBufferSize, "glm::vec3*", __FILE__, __LINE__);
	memcpy(_vertexBufferData, mesh._vertexBufferData, _vertexBufferSize * sizeof(glm::vec3));

	_normalBufferData = (glm::vec3*)Memory::GetInstance()->Allocate(sizeof(glm::vec3) * _vertexBufferSize, "glm::vec3*", __FILE__, __LINE__);
	memcpy(_normalBufferData, mesh._normalBufferData, _vertexBufferSize * sizeof(glm::vec3));

	_indexBufferSize = mesh._indexBufferSize;
	_indexBufferData = (GLuint*)Memory::GetInstance()->Allocate(sizeof(GLfloat) * _indexBufferSize, "GLuint", __FILE__, __LINE__);
	memcpy(_indexBufferData, mesh._indexBufferData, _indexBufferSize * sizeof(GLuint));

	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * sizeof(glm::vec3), _vertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * sizeof(glm::vec3), _normalBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBufferSize * sizeof(GLuint), _indexBufferData, GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
	Shutdown();
}

void Mesh::Initialize()
{
	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * sizeof(glm::vec3), _vertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * sizeof(glm::vec3), _normalBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBufferSize * sizeof(GLuint), _indexBufferData, GL_STATIC_DRAW);
}

void Mesh::Shutdown()
{
	if(_indexBufferData)
	{
		Memory::GetInstance()->Deallocate(_indexBufferData);
		_indexBufferData = nullptr;
		_indexBufferSize = 0;

		glDeleteBuffers(1, &_indexBuffer);
	}

	if(_normalBufferData)
	{
		Memory::GetInstance()->Deallocate(_normalBufferData);
		_normalBufferData = nullptr;

		glDeleteBuffers(1, &_normalBuffer);
	}

	if(_vertexBufferData)
	{
		Memory::GetInstance()->Deallocate(_vertexBufferData);
		_vertexBufferData = nullptr;
		_vertexBufferSize = 0;

		glDeleteBuffers(1, &_vertexBuffer);
	}
}