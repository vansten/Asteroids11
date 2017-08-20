#include "Ship.h"

#include "Projectile.h"
#include "Core/Engine.h"
#include "Core/Input.h"
#include "Physics/BoxCollider.h"
#include "Physics/PhysicalBody.h"

Ship::Ship() : Actor(), _shoot(false), _xRestriction(7.0f)
{
}

void Ship::ProcessTransform(float deltaTime)
{
	float xAxis = (float)Input::GetKey(GLFW_KEY_A) - (float)Input::GetKey(GLFW_KEY_D);
	glm::vec3 position = _transform.GetPosition();
	position.x = glm::clamp(position.x + _speed * deltaTime * xAxis, -_xRestriction, _xRestriction);
	_transform.SetPosition(position);
}

void Ship::ProcessShooting()
{
	if(!_shoot && Input::GetKey(GLFW_KEY_SPACE))
	{
		_shoot = true;

		Projectile* projectile = _projectiles.GetObject();

		if(projectile)
		{
			projectile->Shoot(this, _transform.GetPosition() + MathHelper::Forward * _transform.GetScale().z * 0.65f, 10.0f);
		}
	}

	if(_shoot && !Input::GetKey(GLFW_KEY_SPACE))
	{
		_shoot = false;
	}
}

void Ship::Initialize(ResourceManager& resourceManager)
{
	MeshRenderer* meshRenderer = CreateMeshRenderer(resourceManager.GetMesh(CUBE), resourceManager.GetShader("Shaders/basicVS.glsl", "Shaders/basicFS.glsl"));
	meshRenderer->SetColor(glm::vec4(0.75f, 0.15f, 0.35f, 1.0f));

	AddCollider(NewObject(BoxCollider, this, true, _transform.GetScale()));
	PhysicalBody* physicalBody = CreateRigidbody(false);

	_type = SHIP_TYPE;

	_speed = 5.0f;

	_projectiles.Initialize(GetScene(), 16);
}

void Ship::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	ProcessTransform(deltaTime);
	ProcessShooting();
}

void Ship::OnTrigger(Actor* other)
{
	if(other && other->GetType() == ASTEROID_TYPE)
	{
		Scene* scene = GetScene();
		if(scene)
		{
			scene->Reload();
		}
	}
}

void Ship::ReturnProjectile(Projectile* projectile)
{
	_projectiles.ReturnToPool(projectile);
}
