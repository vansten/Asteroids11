#pragma once

#include "Actor.h"

class Projectile;

class Ship : public Actor
{
protected:
	float _xRestriction;
	float _speed;
	bool _shoot;

	std::vector<Projectile*> _projectiles;

public:
	Ship();

protected:
	void ProcessTransform(float deltaTime);
	void ProcessShooting();

public:
	virtual void Initialize(ResourceManager& resourceManager);

	virtual void Update(float deltaTime);

	virtual void OnTrigger(Actor* other);

	void ReturnProjectile(Projectile* projectile);
};