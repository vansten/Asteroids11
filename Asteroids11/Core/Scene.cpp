#include "Scene.h"

#include "Engine.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Cube.h"
#include "Rendering/Graphics.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

bool Scene::Initialize(ResourceManager& resourceManager)
{
	Actor* ship = SpawnActor();
	ship->SetMesh(resourceManager.GetMesh(CUBE));
	ship->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

	_camera = NewObject(Camera, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f), CameraSettings(60.0f, 1024.0f / 768.0f, 0.1f, 100.0f));
	_actors.push_back(_camera);

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
}

void Scene::Update(GLfloat deltaTime)
{
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
			(*it)->Render(*_camera, graphics);
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
	return SpawnActor(Transform(position, glm::quat(rotation), scale));
}
