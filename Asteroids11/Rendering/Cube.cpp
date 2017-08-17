#include "Cube.h"

#include "Core/Memory.h"
#include "Core/ResourceManager.h"

Cube::Cube(float size, Shader* shader, const glm::vec4& color) : Mesh(shader, color), _size(size)
{
	_name = CUBE;
}

Cube::Cube(const Cube& cube) : Mesh(cube), _size(cube._size)
{
	_name = CUBE;
}

Cube::~Cube()
{

}

void Cube::Initialize()
{
	_vertexBufferSize = 8;
	_vertexBufferData = (glm::vec3*)Memory::GetInstance()->Allocate(sizeof(glm::vec3) * _vertexBufferSize, "glm::vec3*", __FILE__, __LINE__);
	_normalBufferData = (glm::vec3*)Memory::GetInstance()->Allocate(sizeof(glm::vec3) * _vertexBufferSize, "glm::vec3*", __FILE__, __LINE__);

	float hSize = 0.5f * _size;
	_vertexBufferData[0] = glm::vec3(-hSize, -hSize, hSize);
	_vertexBufferData[1] = glm::vec3(hSize, -hSize, hSize);
	_vertexBufferData[2] = glm::vec3(hSize, hSize, hSize);
	_vertexBufferData[3] = glm::vec3(-hSize, hSize, hSize);
	_vertexBufferData[4] = glm::vec3(-hSize, -hSize, -hSize);
	_vertexBufferData[5] = glm::vec3(hSize, -hSize, -hSize);
	_vertexBufferData[6] = glm::vec3(hSize, hSize, -hSize);
	_vertexBufferData[7] = glm::vec3(-hSize, hSize, -hSize);

	_normalBufferData[0] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[1] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[2] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[3] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[4] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[5] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[6] = glm::vec3(0.0f, -1.0f, 0.0f);
	_normalBufferData[7] = glm::vec3(0.0f, -1.0f, 0.0f);
	
	_indexBufferSize = 36;
	_indexBufferData = (GLuint*)Memory::GetInstance()->Allocate(sizeof(GLuint) * _indexBufferSize, "GLuint*", __FILE__, __LINE__);
	_indexBufferData[0] = 0; _indexBufferData[1] = 1; _indexBufferData[2] =  2;
	_indexBufferData[3] = 2; _indexBufferData[4] = 3; _indexBufferData[5] =  0;
	_indexBufferData[6] = 1; _indexBufferData[7] = 5; _indexBufferData[8] =  6;
	_indexBufferData[9] = 6; _indexBufferData[10] = 2; _indexBufferData[11] =  1;
	_indexBufferData[12] = 7; _indexBufferData[13] = 6; _indexBufferData[14] =  5;
	_indexBufferData[15] = 5; _indexBufferData[16] = 4; _indexBufferData[17] =  7;
	_indexBufferData[18] = 4; _indexBufferData[19] = 0; _indexBufferData[20] =  3;
	_indexBufferData[21] = 3; _indexBufferData[22] = 7; _indexBufferData[23] =  4;
	_indexBufferData[24] = 4; _indexBufferData[25] = 5; _indexBufferData[26] =  1;
	_indexBufferData[27] = 1; _indexBufferData[28] = 0; _indexBufferData[29] =  4;
	_indexBufferData[30] = 3; _indexBufferData[31] = 2; _indexBufferData[32] =  6;
	_indexBufferData[33] = 6; _indexBufferData[34] = 7; _indexBufferData[35] =  3;
	
	Mesh::Initialize();
}
