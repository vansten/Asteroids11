#include "Scene.h"

#include "Engine.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Gameplay/Asteroid.h"
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

	Ship* ship = NewObject(Ship);
	ship->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, -4.5f));
	ship->GetTransform().SetScale(glm::vec3(0.3f, 0.1f, 0.8f));
	_actors.push_back(ship);

	Asteroid* asteroid = NewObject(Asteroid);
	_actors.push_back(asteroid);

	_camera = NewObject(Camera, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f), CameraSettings(60.0f, 1024.0f / 768.0f, 0.1f, 100.0f));
	
	//TODO: Add a asteroid spawner class and object

	auto& it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		(*it)->Initialize(resourceManager);
	}

	asteroid->Shoot(glm::vec3(MathHelper::RandomRange(-5.0f, 5.0f), 0.0f, 4.5f), MathHelper::RandomRange(0.5f, 4.0f));

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
	if(_pendingActors.size() > 0)
	{
		auto& it = _pendingActors.begin();
		for(; it != _pendingActors.end(); ++it)
		{
			_actors.push_back((*it));
		}
		_pendingActors.clear();
	}

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
