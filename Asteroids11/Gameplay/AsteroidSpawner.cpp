#include "AsteroidSpawner.h"

#include "Asteroid.h"
#include "Core/Engine.h"
#include "Core/Scene.h"

AsteroidSpawner::AsteroidSpawner() : Actor(), _asteroidsCountText(nullptr)
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

	_asteroidsCount = 0;

	_asteroidDestroyedTimer = 0.0f;

	_normalColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_enlargedColor = glm::vec4(0.7f, 0.2f, 0.05f, 1.0f);
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

	if(_asteroidDestroyedTimer > 0.0f)
	{
		_asteroidDestroyedTimer -= deltaTime;
		if(_asteroidDestroyedTimer <= 0.0f)
		{
			_asteroidDestroyedTimer = 0.0f;
		}

		if(_asteroidsCountText)
		{
			_asteroidsCountText->SetFontSize(20 + (unsigned int)(10 * _asteroidDestroyedTimer));
			_asteroidsCountText->SetColor(MathHelper::LerpVec4(_normalColor, _enlargedColor, _asteroidDestroyedTimer));
		}
	}
}

void AsteroidSpawner::ReturnAsteroid(Asteroid* asteroid, bool byProjectile)
{
	_asteroids.ReturnToPool(asteroid);
	if(byProjectile)
	{
		_currentCooldown = glm::clamp(_currentCooldown - _cooldownDecreaseRate, _minCooldown, _maxCooldown);
		_asteroidsCount += 1;
		if(_asteroidsCountText)
		{
			_asteroidsCountText->SetText(std::to_string(_asteroidsCount));
		}
		_asteroidDestroyedTimer = 1.0f;
	}
}
