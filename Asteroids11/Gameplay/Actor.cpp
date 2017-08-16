#include "Actor.h"

#include "Camera.h"
#include "Physics/Collider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Mesh.h"

Actor::Actor() : _mesh(nullptr), _physicalBody(nullptr), _audioSource(nullptr)
{

}						   

Actor::Actor(const Actor& actor) : _transform(actor._transform), _mesh(actor._mesh)
{

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
		_mesh->Shutdown();
		Memory::GetInstance()->Deallocate<Mesh>(_mesh);
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

void Actor::Render(const Camera& camera, Graphics* graphics)
{
	if(_mesh)
	{
		_mesh->Render(camera, _transform.GetModelToWorldMatrix(), graphics);
	}
}

void Actor::SetTransform(const Transform& transform)
{
	_transform = transform;
}

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