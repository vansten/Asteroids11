#pragma once

#include "glm/glm.hpp"

class Graphics;
class Light;
class Mesh;
class Shader;

class MeshRenderer
{
protected:
	Mesh* _mesh;
	Shader* _shader;
	glm::vec4 _color;

public:
	inline MeshRenderer(Shader* shader, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) : _shader(shader), _color(color), _mesh(nullptr)
	{

	}

	inline MeshRenderer(const MeshRenderer& other) : _color(other._color), _mesh(other._mesh)
	{

	}

	inline virtual ~MeshRenderer()
	{
		_mesh = nullptr;
	}

public:
	inline void SetMesh(Mesh* mesh)
	{
		_mesh = mesh;
	}

	inline Mesh* GetMesh() const
	{
		return _mesh;
	}

	inline void SetColor(const glm::vec4& color)
	{
		_color = color;
	}

	inline const glm::vec4& GetColor() const
	{
		return _color;
	}

	void Render(const class Camera& camera, const glm::mat4& modelMatrix, Graphics* graphics, Light* ambientLight, Light* directionalLight);
};