#include "Actor.h"

#include "Camera.h"
#include "Physics/Collider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Light.h"
#include "Rendering/Mesh.h"

Actor::Actor() : _mesh(nullptr), _physicalBody(nullptr), _audioSource(nullptr), _transform(this)
{

}						   

Actor::Actor(const Actor& actor) : _transform(actor._transform), _mesh(actor._mesh)
{
	_transform.SetOwner(this);
}

Actor::~Actor()
{

}

void Actor::Shutdown()
{
	auto& it = _colliders.begin();
	for(; it != _colliders.end(); ++it)
	{
		Memory::GetInstance()->Deallocate<Collider>((*it));
	}
	_colliders.clear();

	if(_mesh)
	{
		_mesh = nullptr;
	}

	if(_physicalBody != nullptr)
	{
		Memory::GetInstance()->Deallocate<PhysicalBody>(_physicalBody);
		_physicalBody = nullptr;
	}

	if(_audioSource != nullptr)
	{
		Memory::GetInstance()->Deallocate<AudioSource>(_audioSource);
		_audioSource = nullptr;
	}
}

void Actor::Update(float deltaTime)
{
	_transform.Update();
}

void Actor::PreSimulate()
{
	if(_physicalBody)
	{
		_physicalBody->PreSimulation();
	}
}

void Actor::PostSimulate()
{
	if(_physicalBody)
	{
		_physicalBody->PostSimulation();
	}
}

void Actor::Render(const Camera& camera, Graphics* graphics, Light* ambientLight, Light* directionalLight)
{
	if(_mesh)
	{
		_mesh->Render(camera, _transform.GetModelToWorldMatrix(), graphics, ambientLight, directionalLight);
	}
}

void Actor::SetTransform(const Transform& transform)
{
	_transform = transform;
	_transform.SetOwner(this);
}

void Actor::OnModelMatrixUpdated()
{}

PhysicalBody* Actor::CreateRigidbody(bool useGravity)
{
	if(_physicalBody != nullptr)
	{
		Memory::GetInstance()->Deallocate<PhysicalBody>(_physicalBody);
		_physicalBody = nullptr;
	}

	_physicalBody = NewObject(PhysicalBody, this);
	_physicalBody->SetUseGravity(useGravity);
	return reinterpret_cast<PhysicalBody*>(_physicalBody);
}