#pragma once

#include "Actor.h"

class Ship;

class Projectile : public Actor
{
protected:
	Ship* _ownerShip;
	float _speed;

public:
	Projectile();

protected:
	void ProcessTransform(float deltaTime);

public:
	virtual void Initialize(ResourceManager& resourceManager);

	virtual void Update(float deltaTime);

	virtual void OnTrigger(Actor* other);

	void Destroy();
	void Shoot(Ship* ownerShip, const glm::vec3& initialPosition, float speed);
};