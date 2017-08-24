#include "Ship.h"

#include "Projectile.h"
#include "Core/Engine.h"
#include "Core/Input.h"
#include "Physics/BoxCollider.h"
#include "Physics/PhysicalBody.h"

Ship::Ship() : Actor()
{
}

void Ship::ProcessTransform(float deltaTime)
{
	float xAxis = (float)Input::GetKey(GLFW_KEY_A) - (float)Input::GetKey(GLFW_KEY_D);
	glm::vec3 position = _transform.GetPosition();
	position.x = glm::clamp(position.x + _speed * xAxis * deltaTime, -_xRestriction, _xRestriction);
	_transform.SetPosition(position);
}

void Ship::ProcessShooting(float deltaTime)
{
	_shootTimer += deltaTime;
	if(_shootTimer < _shootCooldown)
	{
		return;
	}

	bool key = Input::GetKey(GLFW_KEY_SPACE);

	if(_canShoot && key)
	{
		_shootTimer = 0.0f;
		_canShoot = false;

		Projectile* projectile = _projectiles.GetObject();

		if(projectile)
		{
			_shootAS->Play();
			projectile->Shoot(this, _transform.GetPosition() + MathHelper::Forward * _transform.GetScale().z * 0.65f, 14.0f);
		}
	}

	if(!_canShoot && !key)
	{
		_canShoot = true;
	}
}

void Ship::Initialize(ResourceManager& resourceManager)
{
	Actor::Initialize(resourceManager);

	MeshRenderer* meshRenderer = CreateMeshRenderer(resourceManager.GetMesh(CUBE), resourceManager.GetShader("Shaders/basicVS.glsl", "Shaders/basicFS.glsl"));
	meshRenderer->SetColor(glm::vec4(0.75f, 0.15f, 0.35f, 1.0f));

	_transform.SetScale(glm::vec3(0.3f, 0.1f, 0.8f));

	AddCollider(NewObject(BoxCollider, this, true, _transform.GetScale()));
	PhysicalBody* physicalBody = CreateRigidbody(false);

	_type = SHIP_TYPE;

	_speed = 10.0f;

	_projectiles.Initialize(GetScene(), 16);

	_destroyAS = CreateAudioSource(resourceManager.GetAudioClip("Audio/shipDestroy.wav"));
	_shootAS = CreateAudioSource(resourceManager.GetAudioClip("Audio/shipShoot.wav"));
	_shootAS->SetVolume(0.4f);

	_shootCooldown = _shootAS->GetClipLength();
	_shootTimer = _shootCooldown;
	_canShoot = true;

	_transform.SetPosition(glm::vec3(0.0f, 0.0f, -5.5f + _transform.GetScale().z * 0.5f));
	_xRestriction = 7.5f;
}

void Ship::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if(IsPendingKill())
	{
		return;
	}

	ProcessTransform(deltaTime);
	ProcessShooting(deltaTime);
}

void Ship::OnTrigger(Actor* other)
{
	if(other && other->GetType() == ASTEROID_TYPE)
	{
		DestroyInTime(_destroyAS->GetClipLength());
	}
}

void Ship::OnKill()
{
	Engine::GetInstance()->LoadMenuScene();
}

void Ship::ReturnProjectile(Projectile* projectile)
{
	_projectiles.ReturnToPool(projectile);
}

void Ship::DestroyInTime(float time)
{
	_destroyAS->Play();
	Kill(time);
}
