#include "Mesh.h"

#include "Gameplay/Camera.h"

Mesh::Mesh(Shader* shader, const glm::vec4& color) : _shader(shader), _color(color), _vertexBufferData(nullptr), _indexBufferData(nullptr)
{
}

Mesh::Mesh(const Mesh& mesh) : _shader(mesh._shader), _color(mesh._color), _vertexBufferData(nullptr), _indexBufferData(nullptr)
{
	Shutdown();

	_vertexBufferSize = mesh._vertexBufferSize;
	_vertexBufferData = (GLfloat*)Memory::GetInstance()->Allocate(sizeof(GLfloat) * _vertexBufferSize * 3, "GLfloat", __FILE__, __LINE__);
	memcpy(_vertexBufferData, mesh._vertexBufferData, _vertexBufferSize * 3 * sizeof(GLfloat));

	_indexBufferSize = mesh._indexBufferSize;
	_indexBufferData = (GLuint*)Memory::GetInstance()->Allocate(sizeof(GLfloat) * _indexBufferSize, "GLuint", __FILE__, __LINE__);
	memcpy(_indexBufferData, mesh._indexBufferData, _indexBufferSize * sizeof(GLuint));

	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * 3 * sizeof(GLfloat), _vertexBufferData, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * 3 * sizeof(GLfloat), _vertexBufferData, GL_STATIC_DRAW);

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

	if(_vertexBufferData)
	{
		Memory::GetInstance()->Deallocate(_vertexBufferData);
		_vertexBufferData = nullptr;
		_vertexBufferSize = 0;

		glDeleteBuffers(1, &_vertexBuffer);
	}
}

void Mesh::Render(const Camera& camera, const glm::mat4& modelMatrix, Graphics* graphics)
{
	graphics->SetShader(_shader->GetProgramID());
	graphics->DrawIndexed(camera, modelMatrix, _color, _vertexBuffer, _indexBuffer, _indexBufferSize);
}
