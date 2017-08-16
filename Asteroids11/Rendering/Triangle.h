#pragma once

#include "Mesh.h"
#include "glm/glm.hpp"

class Triangle : public Mesh
{
protected:
	glm::vec3 _v1;
	glm::vec3 _v2;
	glm::vec3 _v3;

public:
	Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, Shader* shader, const glm::vec4& color);
	Triangle(const Triangle& triangle);
	virtual ~Triangle();

	virtual void Initialize();
};

