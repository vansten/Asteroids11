#include "Actor.h"

#include "Camera.h"
#include "Core/Engine.h"
#include "Physics/Collider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Light.h"
#include "Rendering/Mesh.h"

Actor::Actor() : _meshRenderer(nullptr), _physicalBody(nullptr), _audioSource(nullptr), _transform(this), _type(DEFAULT_TYPE)
{

}						   

Actor::Actor(const Actor& actor) : _transform(actor._transform), _type(actor._type)
{
	if(actor._meshRenderer != nullptr)
	{
		_meshRenderer = NewObject(MeshRenderer, *(actor._meshRenderer));
	}
	_transform.SetOwner(this);
}

Actor::~Actor()
{

}

void Actor::Initialize(ResourceManager& resourceManager)
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

	if(_meshRenderer)
	{
		Memory::GetInstance()->Deallocate<MeshRenderer>(_meshRenderer);
		_meshRenderer = nullptr;
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
	if(_meshRenderer)
	{
		_meshRenderer->Render(camera, _transform.GetModelToWorldMatrix(), graphics, ambientLight, directionalLight);
	}
}

void Actor::SetTransform(const Transform& transform)
{
	_transform = transform;
	_transform.SetOwner(this);
}

void Actor::UpdateSize(const glm::vec3& newScale)
{
	auto& it = _colliders.begin();
	for(; it != _colliders.end(); ++it)
	{
		(*it)->UpdateSize(newScale);
	}
}

void Actor::OnModelMatrixUpdated()
{}

void Actor::OnTrigger(Actor* other)
{
	//Do nothing, it's default callback
}

bool Actor::IsVisibleByCamera() const
{
	Scene* scene = GetScene();
	if(!scene)
	{
		return false;
	}

	Camera* camera = scene->GetCamera();
	if(!camera)
	{
		return false;
	}

	const glm::mat4& modelMatrix = _transform.GetModelToWorldMatrix();

	//Check all corners
	return	camera->IsPointVisible(glm::vec3(0.0f, 0.0f, 0.5f), modelMatrix) ||		//Top corner
			camera->IsPointVisible(glm::vec3(0.0f, 0.0f, -0.5f), modelMatrix) ||	//Bottom corner
			camera->IsPointVisible(glm::vec3(0.5f, 0.0f, 0.0f), modelMatrix) ||		//Right corner
			camera->IsPointVisible(glm::vec3(-0.5f, 0.0f, 0.0f), modelMatrix);		//Left corner
}

Scene* Actor::GetScene() const
{
	return Engine::GetInstance()->GetCurrentScene();
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