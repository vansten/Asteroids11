#include "Scene.h"

#include "Engine.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Gameplay/Ship.h"
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Cube.h"
#include "Rendering/Graphics.h"
#include "Rendering/Light.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

bool Scene::Initialize(ResourceManager& resourceManager)
{
	_directionalLight = NewObject(Light);
	_directionalLight->GetTransform().SetRotation(glm::vec3(glm::pi<float>() * 0.4f, 0.0f, 0.0f));
	_directionalLight->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.8f);

	_ambientLight = NewObject(Light);
	_ambientLight->SetColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

	//Actor* ship = SpawnActor();
	Actor* ship = NewObject(Ship, 5.0f);
	ship->SetMesh(resourceManager.GetMesh(CUBE));
	ship->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, -4.5f));
	ship->GetTransform().SetScale(glm::vec3(0.3f, 0.1f, 0.8f));
	ship->GetMesh()->SetColor(glm::vec4(0.75f, 0.15f, 0.35f, 1.0f));
	_actors.push_back(ship);

	_camera = NewObject(Camera, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f), CameraSettings(60.0f, 1024.0f / 768.0f, 0.1f, 100.0f));

	return true;
}

void Scene::Shutdown()
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		(*it)->Shutdown();
		Memory::GetInstance()->Deallocate<Actor>(*it);
		(*it) = nullptr;
	}
	_actors.clear();

	if(_camera)
	{
		_camera->Shutdown();
		Memory::GetInstance()->Deallocate<Camera>(_camera);
		_camera = nullptr;
	}

	if(_ambientLight)
	{
		Memory::GetInstance()->Deallocate<Light>(_ambientLight);
		_ambientLight = nullptr;
	}

	if(_directionalLight)
	{
		Memory::GetInstance()->Deallocate<Light>(_directionalLight);
		_directionalLight = nullptr;
	}
}

void Scene::Update(GLfloat deltaTime)
{
	if(_camera)
	{
		_camera->Update(deltaTime);
	}

	//Lights are updated only for theirs transform update
	if(_ambientLight)
	{
		_ambientLight->Update();
	}

	if(_directionalLight)
	{
		_directionalLight->Update();
	}

	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->Update(deltaTime);
		}
	}
}

void Scene::PreSimulate()
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->PreSimulate();
		}
	}
}

void Scene::PostSimulate()
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->PostSimulate();
		}
	}
}

void Scene::Render(Graphics* graphics)
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->Render(*_camera, graphics, _ambientLight, _directionalLight);
		}
	}
}

Actor* Scene::SpawnActor(const Transform& transform)
{
	Actor* actor = NewObject(Actor);
	actor->SetTransform(transform);
	_actors.push_back(actor);

	return actor;
}

Actor* Scene::SpawnActor(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	Actor* actor = NewObject(Actor);
	actor->GetTransform().SetPosition(position);
	actor->GetTransform().SetRotation(rotation);
	actor->GetTransform().SetScale(scale);
	_actors.push_back(actor);
	return actor;
}
