#pragma once

#include "Actor.h"

class Asteroid : public Actor
{
protected:
	glm::vec3 _direction;
	glm::vec3 _rotation;
	float _speed;

public:
	Asteroid();

protected:
	void ProcessTransform(float deltaTime);

public:
	virtual void Initialize(ResourceManager& resourceManager);
	virtual void Update(float deltaTime);

	void Shoot(const glm::vec3& initialPosition, float speed);
	void Destroy();
};