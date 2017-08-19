#pragma once

#include "Actor.h"

class Asteroid;

class AsteroidSpawner : public Actor
{
protected:
	float _minCooldown;
	float _maxCooldown;
	float _currentCooldown;
	float _cooldownDecreaseRate;

	float _timer;

	std::vector<Asteroid*> _asteroids;

public:
	AsteroidSpawner();

protected:
	void SpawnAsteroid();

public:
	virtual void Initialize(ResourceManager& resourceManager);

	virtual void Update(float deltaTime);

	void ReturnAsteroid(Asteroid* asteroid, bool byProjectile);
};