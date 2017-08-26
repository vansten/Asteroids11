#pragma once

#include "Actor.h"
#include "ActorsPool.h"

class Asteroid;
class UIActor;

class AsteroidSpawner : public Actor
{
protected:
	float _minCooldown;
	float _maxCooldown;
	float _currentCooldown;
	float _cooldownDecreaseRate;

	float _timer;

	ActorsPool<Asteroid> _asteroids;

	UIActor* _asteroidsCountText;
	unsigned int _asteroidsCount;
	float _asteroidDestroyedTimer;
	glm::vec4 _normalColor;
	glm::vec4 _enlargedColor;

public:
	AsteroidSpawner();

protected:
	void SpawnAsteroid();

public:
	virtual void Initialize(ResourceManager& resourceManager) override;

	virtual void Update(float deltaTime) override;

	void ReturnAsteroid(Asteroid* asteroid, bool byProjectile);

	inline void SetAsteroidsCountText(UIActor* asteroidsCountText)
	{
		_asteroidsCountText = asteroidsCountText;
	}
};