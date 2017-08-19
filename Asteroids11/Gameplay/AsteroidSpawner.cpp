#include "AsteroidSpawner.h"

#include "Asteroid.h"
#include "Core/Engine.h"
#include "Core/Scene.h"

AsteroidSpawner::AsteroidSpawner() : Actor()
{

}

void AsteroidSpawner::SpawnAsteroid()
{
	Asteroid* asteroidToShoot = nullptr;

	if(_asteroids.size() > 0)
	{
		asteroidToShoot = _asteroids.at(0);
		_asteroids.erase(_asteroids.begin());
	}
	else
	{
		Scene* scene = GetScene();
		if(scene)
		{
			asteroidToShoot = scene->SpawnActor<Asteroid>();
		}
	}

	if(asteroidToShoot)
	{
		asteroidToShoot->Shoot(this, glm::vec3(MathHelper::RandomRange(-6.0f, 6.0f), 0.0f, _transform.GetPosition().z), MathHelper::RandomRange(2.0f, 4.0f));
	}
}

void AsteroidSpawner::Initialize(ResourceManager& resourceManager)
{
	Actor::Initialize(resourceManager);

	Scene* scene = GetScene();
	if(scene)
	{
		for(int i = 0; i < 5; ++i)
		{
			_asteroids.push_back(scene->SpawnActor<Asteroid>());
		}
	}

	_maxCooldown = 4.0f;
	_minCooldown = 0.3f;
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
	_asteroids.push_back(asteroid);
	if(byProjectile)
	{
		_currentCooldown = glm::clamp(_currentCooldown - _cooldownDecreaseRate, _minCooldown, _maxCooldown);
	}
}
