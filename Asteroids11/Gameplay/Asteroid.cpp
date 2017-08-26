#include "Asteroid.h"

#include "AsteroidSpawner.h"
#include "Core/MathHelper.h"
#include "Physics/BoxCollider.h"

Asteroid::Asteroid() : Actor()
{

}

void Asteroid::ProcessTransform(float deltaTime)
{
	_transform.Translate(_direction * deltaTime * _speed);
	_transform.Rotate(_rotation * deltaTime * _speed);

	//TODO: Change this check to !IsVisibleByCamera() or something like that
	if(!IsVisibleByCamera())
	{
		Destroy(false);
	}
}

void Asteroid::Initialize(ResourceManager& resourceManager)
{
	Actor::Initialize(resourceManager);

	MeshRenderer* meshRenderer = CreateMeshRenderer(resourceManager.GetMesh(CUBE), resourceManager.GetShader("Shaders/basicVS.glsl", "Shaders/basicFS.glsl"));
	meshRenderer->SetColor(glm::vec4(
		MathHelper::RandomRange(0.35f, 0.65f),
		MathHelper::RandomRange(0.35f, 0.65f),
		MathHelper::RandomRange(0.35f, 0.65f),
		1.0f
	));

	_type = ASTEROID_TYPE;

	AddCollider(NewObject(BoxCollider, this, true, glm::vec3(1.0f, 1.0f, 1.0f)));
	CreateRigidbody(false);

	_destroyAS = CreateAudioSource(resourceManager.GetAudioClip("Audio/asteroidDestroy.wav"));

	SetEnabled(false);
}

void Asteroid::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if(IsPendingKill())
	{
		return;
	}

	ProcessTransform(deltaTime);
}

void Asteroid::Shoot(AsteroidSpawner* spawner, const glm::vec3& initialPosition, float speed)
{
	_spawner = spawner;
	_transform.SetPosition(initialPosition);

	glm::vec3 directionRotation(0.0f, glm::radians(MathHelper::RandomRange(-25.0f, 25.0f)), 0.0f);
	_direction = glm::vec3(glm::toMat4(glm::quat(directionRotation)) * glm::vec4(-MathHelper::Forward, 0.0f));
	_rotation = glm::vec3(
		glm::radians(MathHelper::RandomRange(-60.0f, 60.0f)),
		glm::radians(MathHelper::RandomRange(-60.0f, 60.0f)),
		glm::radians(MathHelper::RandomRange(-60.0f, 60.0f))
	);
	glm::vec3 scale(
		MathHelper::RandomRange(0.2f, 1.2f),
		MathHelper::RandomRange(0.05f, 0.15f),
		MathHelper::RandomRange(0.2f, 1.2f)
	);
	_transform.SetScale(scale);
	_speed = speed;
	SetEnabled(true);
}

void Asteroid::Destroy(bool byProjectile)
{
	_spawner->ReturnAsteroid(this, byProjectile);
	if(byProjectile)
	{
		float time = _destroyAS->GetClipLength();
		_destroyAS->Play();
		Kill(time);
	}
	else
	{
		SetEnabled(false);
	}
}
