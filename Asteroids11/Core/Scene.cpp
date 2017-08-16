#include "Scene.h"

#include "Engine.h"
#include "ResourceManager.h"
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Cube.h"
#include "Rendering/Graphics.h"
#include "Rendering/Triangle.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

bool Scene::Initialize(ResourceManager& resourceManager)
{
	Cube c(1.0f, resourceManager.GetShader("Shaders/screenTriangleVS.glsl", "Shaders/screenTriangleFS.glsl"), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	c.Initialize();
	
	Actor* actor = SpawnActor();
	actor->SetMesh(c);
	actor->GetTransform().Translate(0.0f, 0.5f, 0.0f);
	BoxCollider* boxCollider = NewObject(BoxCollider, actor, true, glm::vec3(c.GetSize()));
	actor->AddCollider(boxCollider);
	PhysicalBody* physicalBody = actor->CreateRigidbody(false);
	physicalBody->SetKinematic(true);
	
	actor = SpawnActor();
	actor->SetMesh(c);
	actor->GetMesh()->SetColor(glm::vec4(0.12f, 0.65f, 0.32f, 1.0f));
	actor->GetTransform().SetScale(glm::vec3(50.0f, 0.001f, 50.0f));
	boxCollider = NewObject(BoxCollider, actor, false, glm::vec3(c.GetSize() * 50.0f, c.GetSize() * 0.001f, c.GetSize() * 50.0f));
	actor->AddCollider(boxCollider);
	physicalBody = actor->CreateRigidbody(false);
	physicalBody->SetKinematic(true);
	
	c.Shutdown();
	
	_camera = NewObject(Camera, glm::vec3(0.0f, 1.8f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), CameraSettings(60.0f, 1024.0f / 768.0f, 0.1f, 100.0f));
	CapsuleCollider* capsuleCollider = NewObject(CapsuleCollider, actor, false, 1.8f, 0.5f);
	_camera->AddCollider(capsuleCollider);
	physicalBody = _camera->CreateRigidbody(true);
	physicalBody->SetKinematic(true);
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
	for(size_t i = 0; i < _actors.size(); ++i)
	{
		_actors[i]->Update(deltaTime);
	}
}

void Scene::PreSimulate()
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		(*it)->PreSimulate();
	}
}

void Scene::PostSimulate()
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		(*it)->PostSimulate();
	}
}

void Scene::Render(Graphics* graphics)
{
	std::vector<Actor*>::iterator it = _actors.begin();
	for(; it != _actors.end(); ++it)
	{
		(*it)->Render(*_camera, graphics);
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
