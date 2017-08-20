#include "AsteroidSpawner.h"

#include "Asteroid.h"
#include "Core/Engine.h"
#include "Core/Scene.h"

AsteroidSpawner::AsteroidSpawner() : Actor()
{

}

void AsteroidSpawner::SpawnAsteroid()
{
	Asteroid* asteroidToShoot = _asteroids.GetObject();

	if(asteroidToShoot)
	{
		asteroidToShoot->Shoot(this, glm::vec3(MathHelper::RandomRange(-6.0f, 6.0f), 0.0f, _transform.GetPosition().z), MathHelper::RandomRange(2.0f, 4.0f));
	}
}

void AsteroidSpawner::Initialize(ResourceManager& resourceManager)
{
	Actor::Initialize(resourceManager);

	_asteroids.Initialize(GetScene(), 8);

	_maxCooldown = 3.0f;
	_minCooldown = 0.1f;
	_cooldownDecreaseRate = 0.1f;
	_currentCooldown = _maxCooldown;
	_timer = _currentCooldown - 0.5f;
}

void AsteroidSpawner::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	_timer += deltaTime;
	if(_timer >= _currentCooldown)
	{
		SpawnAsteroid();
		_timer = 0.0f;
	}
}

void AsteroidSpawner::ReturnAsteroid(Asteroid* asteroid, bool byProjectile)
{
	_asteroids.ReturnToPool(asteroid);
	if(byProjectile)
	{
		_currentCooldown = glm::clamp(_currentCooldown - _cooldownDecreaseRate, _minCooldown, _maxCooldown);
	}
}
