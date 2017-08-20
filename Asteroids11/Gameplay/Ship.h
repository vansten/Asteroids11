#pragma once

#include "Actor.h"
#include "ActorsPool.h"

class Projectile;

class Ship : public Actor
{
protected:
	float _xRestriction;
	float _speed;

	ActorsPool<Projectile> _projectiles;
	AudioSource* _shootAS;
	AudioSource* _destroyAS;

	float _shootTimer;
	float _shootCooldown;
	bool _canShoot;

public:
	Ship();

protected:
	void ProcessTransform(float deltaTime);
	void ProcessShooting(float deltaTime);

public:
	virtual void Initialize(ResourceManager& resourceManager) override;

	virtual void Update(float deltaTime) override;

	virtual void OnTrigger(Actor* other) override;
	virtual void OnKill() override;

	void ReturnProjectile(Projectile* projectile);
	void DestroyInTime(float time);
};