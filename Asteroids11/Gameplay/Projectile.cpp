#include "Projectile.h"

#include "Asteroid.h"
#include "Ship.h"
#include "Physics/BoxCollider.h"

Projectile::Projectile() : Actor()
{}

void Projectile::ProcessTransform(float deltaTime)
{
	glm::vec3 position = _transform.GetPosition();
	position += MathHelper::Forward * deltaTime * _speed;
	_transform.SetPosition(position);

	if(!IsVisibleByCamera())
	{
		Destroy();
	}
}

void Projectile::Initialize(ResourceManager& resourceManager)
{
	Actor::Initialize(resourceManager);

	MeshRenderer* meshRenderer = CreateMeshRenderer(resourceManager.GetMesh(CUBE), resourceManager.GetShader("Shaders/basicVS.glsl", "Shaders/basicFS.glsl"));
	meshRenderer->SetColor(glm::vec4(0.95f, 0.05f, 0.05f, 1.0f));
	glm::vec3 scale(0.1f, 0.1f, 0.25f);
	_transform.SetScale(scale);

	_type = PROJECTILE_TYPE;

	AddCollider(NewObject(BoxCollider, this, true, scale));
	CreateRigidbody(false);

	SetEnabled(false);
}

void Projectile::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	ProcessTransform(deltaTime);
}

void Projectile::OnTrigger(Actor* other)
{
	if(other && other->GetType() == ASTEROID_TYPE)
	{
		Asteroid* asteroid = reinterpret_cast<Asteroid*>(other);
		if(asteroid)
		{
			asteroid->Destroy(true);
			Destroy();
		}
	}
}

void Projectile::Destroy()
{
	if(_ownerShip)
	{
		_ownerShip->ReturnProjectile(this);
		SetEnabled(false);
	}
}

void Projectile::Shoot(Ship* ownerShip, const glm::vec3& initialPosition, float speed)
{
	_ownerShip = ownerShip;

	_transform.SetPosition(initialPosition);
	_speed = speed;

	SetEnabled(true);
}
