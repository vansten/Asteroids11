#include "Triangle.h"

#include "Core/Memory.h"

Triangle::Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, Shader* shader, const glm::vec4& color) : _v1(v1), _v2(v2), _v3(v3), Mesh(shader, color)
{
}

Triangle::Triangle(const Triangle& triangle) : _v1(triangle._v1), _v2(triangle._v2), _v3(triangle._v3), Mesh(triangle)
{
}

Triangle::~Triangle()
{

}

void Triangle::Initialize()
{
	_vertexBufferSize = 3;
	_vertexBufferData = (GLfloat*)Memory::GetInstance()->Allocate(sizeof(GLfloat) * _vertexBufferSize * 3, "GLfloat*", __FILE__, __LINE__);
	_vertexBufferData[0] = _v1.x; _vertexBufferData[1] = _v1.y; _vertexBufferData[2] = _v1.z;
	_vertexBufferData[3] = _v2.x; _vertexBufferData[4] = _v2.y; _vertexBufferData[5] = _v2.z;
	_vertexBufferData[6] = _v3.x; _vertexBufferData[7] = _v3.y; _vertexBufferData[8] = _v3.z;

	_indexBufferData = (GLuint*)Memory::GetInstance()->Allocate(sizeof(GLuint) * _indexBufferSize, "GLuint*", __FILE__, __LINE__);
	_indexBufferData[0] = 0;
	_indexBufferData[1] = 1;
	_indexBufferData[2] = 2;

	_indexBufferSize = 3;

	Mesh::Initialize();
}
