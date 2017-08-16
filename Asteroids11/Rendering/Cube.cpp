#include "Cube.h"

#include "Core/Memory.h"

Cube::Cube(float size, Shader* shader, const glm::vec4& color) : Mesh(shader, color), _size(size)
{

}

Cube::Cube(const Cube& cube) : Mesh(cube), _size(cube._size)
{

}

Cube::~Cube()
{

}

void Cube::Initialize()
{
	_vertexBufferSize = 8;
	_vertexBufferData = (GLfloat*)Memory::GetInstance()->Allocate(sizeof(GLfloat) * _vertexBufferSize * 3, "GLfloat*", __FILE__, __LINE__);

	float hSize = 0.5f * _size;
	_vertexBufferData[0] = -hSize;	_vertexBufferData[1] = -hSize;	_vertexBufferData[2] = hSize;
	_vertexBufferData[3] = hSize;	_vertexBufferData[4] = -hSize;	_vertexBufferData[5] = hSize;
	_vertexBufferData[6] = hSize;	_vertexBufferData[7] = hSize;	_vertexBufferData[8] = hSize;
	_vertexBufferData[9] = -hSize;	_vertexBufferData[10] = hSize;	_vertexBufferData[11] = hSize;
	_vertexBufferData[12] = -hSize; _vertexBufferData[13] = -hSize; _vertexBufferData[14] = -hSize;
	_vertexBufferData[15] = hSize;	_vertexBufferData[16] = -hSize; _vertexBufferData[17] = -hSize;
	_vertexBufferData[18] = hSize;	_vertexBufferData[19] = hSize;	_vertexBufferData[20] = -hSize;
	_vertexBufferData[21] = -hSize; _vertexBufferData[22] = hSize;	_vertexBufferData[23] = -hSize;
	
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
