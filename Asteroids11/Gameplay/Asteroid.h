#pragma once

#include "Actor.h"

class AsteroidSpawner;

class Asteroid : public Actor
{
protected:
	glm::vec3 _direction;
	glm::vec3 _rotation;
	float _speed;

	AsteroidSpawner* _spawner;
	AudioSource* _destroyAS;

public:
	Asteroid();

protected:
	void ProcessTransform(float deltaTime);

public:
	virtual void Initialize(ResourceManager& resourceManager) override;
	virtual void Update(float deltaTime) override;

	void Shoot(AsteroidSpawner* spawner, const glm::vec3& initialPosition, float speed);
	void Destroy(bool byProjectile);
};