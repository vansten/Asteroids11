#pragma once

#include "Actor.h"
#include "ActorsPool.h"

class Asteroid;

class AsteroidSpawner : public Actor
{
protected:
	float _minCooldown;
	float _maxCooldown;
	float _currentCooldown;
	float _cooldownDecreaseRate;

	float _timer;

	ActorsPool<Asteroid> _asteroids;

public:
	AsteroidSpawner();

protected:
	void SpawnAsteroid();

public:
	virtual void Initialize(ResourceManager& resourceManager) override;

	virtual void Update(float deltaTime) override;

	void ReturnAsteroid(Asteroid* asteroid, bool byProjectile);
};